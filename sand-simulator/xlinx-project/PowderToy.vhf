--------------------------------------------------------------------------------
-- Copyright (c) 1995-2013 Xilinx, Inc.  All rights reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 14.7
--  \   \         Application : sch2hdl
--  /   /         Filename : PowderToy.vhf
-- /___/   /\     Timestamp : 04/09/2019 11:55:28
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: sch2hdl -intstyle ise -family spartan3e -flat -suppress -vhdl C:/Users/lab/ProjektUCiSW-PowderToy/PowderToy.vhf -w C:/Users/lab/ProjektUCiSW-PowderToy/PowderToy.sch
--Design Name: PowderToy
--Device: spartan3e
--Purpose:
--    This vhdl netlist is translated from an ECS schematic. It can be 
--    synthesized and simulated, but it should not be modified. 
--

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;
library UNISIM;
use UNISIM.Vcomponents.ALL;

entity PowderToy is
   port ( CLK50    : in    std_logic; 
          PS2_Clk  : in    std_logic; 
          PS2_Data : in    std_logic; 
          B        : out   std_logic; 
          G        : out   std_logic; 
          HS       : out   std_logic; 
          R        : out   std_logic; 
          VS       : out   std_logic);
end PowderToy;

architecture BEHAVIORAL of PowderToy is
   attribute BOX_TYPE   : string ;
   signal XLXN_29  : std_logic_vector (2 downto 0);
   signal XLXN_35  : std_logic_vector (9 downto 0);
   signal XLXN_37  : std_logic_vector (7 downto 0);
   signal XLXN_39  : std_logic_vector (8 downto 0);
   signal XLXN_42  : std_logic;
   signal XLXN_44  : std_logic;
   component VGADriver
      port ( CLK_50MHz : in    std_logic; 
             RGB       : in    std_logic_vector (2 downto 0); 
             VGA_R     : out   std_logic; 
             VGA_G     : out   std_logic; 
             VGA_B     : out   std_logic; 
             VGA_HS    : out   std_logic; 
             VGA_VS    : out   std_logic; 
             PIX_X     : out   std_logic_vector (9 downto 0); 
             PIX_Y     : out   std_logic_vector (8 downto 0));
   end component;
   
   component PS2_Kbd
      port ( PS2_Clk   : in    std_logic; 
             PS2_Data  : in    std_logic; 
             Clk_50MHz : in    std_logic; 
             E0        : out   std_logic; 
             F0        : out   std_logic; 
             DO_Rdy    : out   std_logic; 
             DO        : out   std_logic_vector (7 downto 0); 
             Clk_Sys   : in    std_logic);
   end component;
   
   component PowderSimulator
      port ( CLK_50MHz : in    std_logic; 
             RxDO      : in    std_logic_vector (7 downto 0); 
             PIX_X     : in    std_logic_vector (9 downto 0); 
             PIX_Y     : in    std_logic_vector (8 downto 0); 
             RGB       : out   std_logic_vector (2 downto 0); 
             BTN_HOLD  : in    std_logic);
   end component;
   
   component INV
      port ( I : in    std_logic; 
             O : out   std_logic);
   end component;
   attribute BOX_TYPE of INV : component is "BLACK_BOX";
   
begin
   XLXI_1 : VGADriver
      port map (CLK_50MHz=>CLK50,
                RGB(2 downto 0)=>XLXN_29(2 downto 0),
                PIX_X(9 downto 0)=>XLXN_35(9 downto 0),
                PIX_Y(8 downto 0)=>XLXN_39(8 downto 0),
                VGA_B=>B,
                VGA_G=>G,
                VGA_HS=>HS,
                VGA_R=>R,
                VGA_VS=>VS);
   
   XLXI_2 : PS2_Kbd
      port map (Clk_Sys=>CLK50,
                Clk_50MHz=>CLK50,
                PS2_Clk=>PS2_Clk,
                PS2_Data=>PS2_Data,
                DO(7 downto 0)=>XLXN_37(7 downto 0),
                DO_Rdy=>open,
                E0=>open,
                F0=>XLXN_42);
   
   XLXI_6 : PowderSimulator
      port map (BTN_HOLD=>XLXN_44,
                CLK_50MHz=>CLK50,
                PIX_X(9 downto 0)=>XLXN_35(9 downto 0),
                PIX_Y(8 downto 0)=>XLXN_39(8 downto 0),
                RxDO(7 downto 0)=>XLXN_37(7 downto 0),
                RGB(2 downto 0)=>XLXN_29(2 downto 0));
   
   XLXI_7 : INV
      port map (I=>XLXN_42,
                O=>XLXN_44);
   
end BEHAVIORAL;


