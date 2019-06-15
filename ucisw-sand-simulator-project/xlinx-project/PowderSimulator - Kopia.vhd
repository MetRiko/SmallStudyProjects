----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:59:11 03/26/2019 
-- Design Name: 
-- Module Name:    PowderSimulator - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values


-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity PowderSimulator is
    Port ( CLK_50MHz : in STD_LOGIC;
           RxDO : in  STD_LOGIC_VECTOR (7 downto 0);
           PIX_X : in  STD_LOGIC_VECTOR (9 downto 0);
           PIX_Y : in  STD_LOGIC_VECTOR (8 downto 0);
           BTN_HOLD : in STD_LOGIC;
           RGB : out  STD_LOGIC_VECTOR (2 downto 0));
end PowderSimulator;

architecture Behavioral of PowderSimulator is

signal cursor_x : integer := 100;
signal cursor_y : integer := 100;
signal clock_counter : integer := 0;
signal clk_main : STD_LOGIC := '0';
signal clock_grav_counter : integer := 0;
signal clk_grav : STD_LOGIC := '0';
signal conv_pix_x : integer := 0;
signal conv_pix_y : integer := 0;
signal x_i : integer := 0;
signal y_i : integer := 0;
signal f_i : integer := 0;
signal xc_i : integer := 0;
signal yc_i : integer := 0;
signal fc_i : integer := 0;

-- gravity
signal put_sand_flag : STD_LOGIC := '0';
signal put_sand_flag_proc : STD_LOGIC := '0';
signal put_sand_idx : integer := 0;
signal y_plus_idx : integer := 0;
signal grav_i : integer := 0;
signal temp_check : STD_LOGIC := '0';

signal key_w : STD_LOGIC := '0';
signal key_s : STD_LOGIC := '0';
signal key_a : STD_LOGIC := '0';
signal key_d : STD_LOGIC := '0';
signal key_f : STD_LOGIC := '0';
signal key_x : STD_LOGIC := '0';

--type pixel_array is array (integer range <> ) of integer; -- 58x78
--signal pixel_screen : pixel_array (4660 downto 0) := (others => 0);

type pixel_array is array (integer range <> ) of STD_LOGIC; -- 58x78
signal pixel_screen : pixel_array (4660 downto 0);

begin  
   clk_div : process(CLK_50MHz)
   begin
        if (rising_edge(CLK_50MHz)) then
            clock_counter <= clock_counter + 1;
            if( clock_counter > 200000 ) then
               clock_counter <= 0;
               clk_main <= not clk_main;
            end if;
            clock_grav_counter <= clock_grav_counter + 1;
            if( clock_grav_counter > 5000000 ) then
               clock_grav_counter <= 0;
               clk_grav <= not clk_grav;
            end if;
        end if;
   end process clk_div;
   
   input : process(BTN_HOLD, RxDO)
   begin
   
      if(BTN_HOLD = '0') then
         if(RxDO = "00011101") then key_w <= '0';
         elsif(RxDO = "00011011") then key_s <= '0';
         elsif(RxDO = "00011100") then key_a <= '0';
         elsif(RxDO = "00100011") then key_d <= '0';
         elsif(RxDO = "00101011") then key_f <= '0';
         elsif(RxDO = "00100010") then key_x <= '0';
         else
            key_w <= '0';
            key_s <= '0';
            key_a <= '0';
            key_d <= '0';
            key_f <= '0';
            key_x <= '0';
         end if;
      elsif(BTN_HOLD = '1') then
         if(RxDO = "00011101") then key_w <= '1';
         elsif(RxDO = "00011011") then key_s <= '1';
         elsif(RxDO = "00011100") then key_a <= '1';
         elsif(RxDO = "00100011") then key_d <= '1';
         elsif(RxDO = "00101011") then key_f <= '1';
         elsif(RxDO = "00100010") then key_x <= '1'; end if;
      end if;
   end process input;
--   gravity : process
--   begin
--      if(rising_edge(clk_grav)) then
--         --put_sand_flag_proc <= put_sand_flag or put_sand_flag_proc;
--         -- spadanie
--         
--         
--         -- stworzenie nowych
--         if(put_sand_flag = '1') then          
--            pixel_screen(put_sand_idx) <= 1;
--            --put_sand_flag_proc <= '0';
--         else
--            --for i in 0 to 4660 loop 
--               y_plus_idx <= grav_i + 79;
--               --temp_check <= (pixel_screen(grav_i) = 1) and (pixel_screen(y_plus_idx) = 0);
--               --if(y_plus_idx < 660) then
--                  --pixel_screen(grav_i) <= '0';
--                  --pixel_screen(y_plus_idx) <= 1;
--               --else 
--                  --pixel_screen(y_plus_idx) <= 0;
--               --end if;
--               grav_i <= grav_i + 1;
--               if (grav_i > 4660) then grav_i <= 0; end if;
--            --end loop;
--         end if;
--         
--      end if;
--   end process;
   
   cursor : process
      begin
         if(rising_edge(clk_main)) then
            put_sand_flag <= '0';
            if(key_w = '1') then
               --up
               cursor_y <= cursor_y - 1;
               if (cursor_y < 5) then
                  cursor_y <= 5;
               end if;
            elsif(key_s = '1') then
               --down
               cursor_y <= cursor_y + 1;
               if(cursor_y > 470) then
                  cursor_y <= 470;
               end if;
            end if;
            if(key_a = '1') then
               --left
               cursor_x <= cursor_x - 1;
               if(cursor_x < 5) then
                  cursor_x <= 5;
               end if;
            elsif(key_d = '1') then
               --right
               cursor_x <= cursor_x + 1;
               if(cursor_x > 630) then
                  cursor_x <= 630;
               end if;
            elsif(key_f = '1') then
               --put sand               
               
               xc_i <= (cursor_x - 5) / 8;
               yc_i <= (cursor_y - 5) / 8;
               fc_i <= xc_i + 79 * yc_i;              
               put_sand_idx <= fc_i; 
               put_sand_flag <= '1';
               -- pixel_screen(fc_i) <= '1';
               
            elsif(key_x = '1') then
               --remove sand               
               
               xc_i <= (cursor_x - 5) / 8;
               yc_i <= (cursor_y - 5) / 8;
               fc_i <= xc_i + 79 * yc_i;                
               put_sand_idx <= fc_i; 
               put_sand_flag <= '1';
               -- pixel_screen(fc_i) <= '0';
               
            end if;
         end if;
         
         if(rising_edge(clk_grav)) then
            --put_sand_flag_proc <= put_sand_flag or put_sand_flag_proc;
            -- spadanie
         
         
            -- stworzenie nowych
            if(put_sand_flag = '1') then          
               pixel_screen(put_sand_idx) <= '1';
               --put_sand_flag_proc <= '0';
               --for i in 4660 downto 0 loop 
            else
               for i in 2000 downto 0 loop --4580
                  --y_plus_idx <= i + 79;
                  --temp_check <= (pixel_screen(grav_i) = 1) and (pixel_screen(y_plus_idx) = 0);
                  if(pixel_screen(i) = '1' and pixel_screen(i + 79) = '0') then
                     pixel_screen(i) <= '0';
                     pixel_screen(i + 79) <= '1';
                     --for j in 4580 downto 0 loop 
                        --if(j = i + 79) then
                           --pixel_screen(j) <= '1';
                        --end if;
                     --end loop;
                  --else                     
                     --pixel_screen(i + 79) <= '0';
                  end if;
               end loop;
                     --if(y_plus_idx < 660) then
                        --pixel_screen(grav_i) <= '0';
                        --pixel_screen(y_plus_idx) <= 1;
                     --else 
                        --pixel_screen(y_plus_idx) <= 0;
                     --end if;
                     --grav_i <= grav_i + 1;
                     --if (grav_i > 4660) then grav_i <= 0; end if;
                  --end loop;
            
            end if;        
         end if;
         
         
   end process cursor;
         
   render : process(PIX_X, PIX_Y)
      begin
         conv_pix_x <= to_integer(unsigned(PIX_X));
         conv_pix_y <= to_integer(unsigned(PIX_Y));
         
         x_i <= (conv_pix_x - 5) / 8;
         y_i <= (conv_pix_y - 5) / 8;
         f_i <= x_i + 79 * y_i;
         if (cursor_x + 2 < conv_pix_x and cursor_x + 8 > conv_pix_x and cursor_y + 2 < conv_pix_y and cursor_y + 8 > conv_pix_y) then
            RGB <= "000";
         elsif (cursor_x < conv_pix_x and cursor_x + 10 > conv_pix_x and cursor_y < conv_pix_y and cursor_y + 10 > conv_pix_y) then
            RGB <= "111";
         elsif(pixel_screen(f_i) = '1') then
            RGB <= "111";
         else
            RGB <= "000";
         end if;
   end process render;
   
   --put_sand_flag_proc <= put_sand_flag or put_sand_flag_proc;
   
end Behavioral;

