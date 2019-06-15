<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_6" />
        <signal name="XLXN_17" />
        <signal name="CLK50" />
        <signal name="PS2_Clk" />
        <signal name="PS2_Data" />
        <signal name="XLXN_29(2:0)" />
        <signal name="R" />
        <signal name="G" />
        <signal name="B" />
        <signal name="HS" />
        <signal name="VS" />
        <signal name="XLXN_35(9:0)" />
        <signal name="XLXN_37(7:0)" />
        <signal name="XLXN_39(8:0)" />
        <signal name="XLXN_42" />
        <signal name="XLXN_44" />
        <port polarity="Input" name="CLK50" />
        <port polarity="Input" name="PS2_Clk" />
        <port polarity="Input" name="PS2_Data" />
        <port polarity="Output" name="R" />
        <port polarity="Output" name="G" />
        <port polarity="Output" name="B" />
        <port polarity="Output" name="HS" />
        <port polarity="Output" name="VS" />
        <blockdef name="VGADriver">
            <timestamp>2019-3-26T11:22:32</timestamp>
            <rect width="256" x="64" y="-448" height="448" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-416" y2="-416" x1="320" />
            <line x2="384" y1="-352" y2="-352" x1="320" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <rect width="64" x="320" y="-108" height="24" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <rect width="64" x="320" y="-44" height="24" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <blockdef name="PS2_Kbd">
            <timestamp>2019-3-26T11:30:14</timestamp>
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
            <rect width="64" x="320" y="-236" height="24" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
        </blockdef>
        <blockdef name="PowderSimulator">
            <timestamp>2019-4-9T9:54:42</timestamp>
            <line x2="0" y1="96" y2="96" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <rect width="64" x="0" y="-172" height="24" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <rect width="64" x="320" y="-236" height="24" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <rect width="256" x="64" y="-256" height="384" />
        </blockdef>
        <blockdef name="inv">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="160" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="-64" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="0" x1="128" />
            <line x2="64" y1="0" y2="-64" x1="64" />
            <circle r="16" cx="144" cy="-32" />
        </blockdef>
        <block symbolname="VGADriver" name="XLXI_1">
            <blockpin signalname="CLK50" name="CLK_50MHz" />
            <blockpin signalname="XLXN_29(2:0)" name="RGB(2:0)" />
            <blockpin signalname="R" name="VGA_R" />
            <blockpin signalname="G" name="VGA_G" />
            <blockpin signalname="B" name="VGA_B" />
            <blockpin signalname="HS" name="VGA_HS" />
            <blockpin signalname="VS" name="VGA_VS" />
            <blockpin signalname="XLXN_35(9:0)" name="PIX_X(9:0)" />
            <blockpin signalname="XLXN_39(8:0)" name="PIX_Y(8:0)" />
        </block>
        <block symbolname="PS2_Kbd" name="XLXI_2">
            <blockpin signalname="PS2_Clk" name="PS2_Clk" />
            <blockpin signalname="PS2_Data" name="PS2_Data" />
            <blockpin signalname="CLK50" name="Clk_50MHz" />
            <blockpin name="E0" />
            <blockpin signalname="XLXN_42" name="F0" />
            <blockpin name="DO_Rdy" />
            <blockpin signalname="XLXN_37(7:0)" name="DO(7:0)" />
            <blockpin signalname="CLK50" name="Clk_Sys" />
        </block>
        <block symbolname="PowderSimulator" name="XLXI_6">
            <blockpin signalname="CLK50" name="CLK_50MHz" />
            <blockpin signalname="XLXN_37(7:0)" name="RxDO(7:0)" />
            <blockpin signalname="XLXN_35(9:0)" name="PIX_X(9:0)" />
            <blockpin signalname="XLXN_39(8:0)" name="PIX_Y(8:0)" />
            <blockpin signalname="XLXN_29(2:0)" name="RGB(2:0)" />
            <blockpin signalname="XLXN_44" name="BTN_HOLD" />
        </block>
        <block symbolname="inv" name="XLXI_7">
            <blockpin signalname="XLXN_42" name="I" />
            <blockpin signalname="XLXN_44" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1408" y="1648" name="XLXI_1" orien="R0">
        </instance>
        <instance x="704" y="512" name="XLXI_2" orien="R0">
        </instance>
        <branch name="CLK50">
            <wire x2="448" y1="1568" y2="1568" x1="400" />
            <wire x2="1376" y1="1568" y2="1568" x1="448" />
            <wire x2="1376" y1="1568" y2="1824" x1="1376" />
            <wire x2="1408" y1="1824" y2="1824" x1="1376" />
            <wire x2="656" y1="480" y2="480" x1="448" />
            <wire x2="704" y1="480" y2="480" x1="656" />
            <wire x2="448" y1="480" y2="1568" x1="448" />
            <wire x2="704" y1="416" y2="416" x1="656" />
            <wire x2="656" y1="416" y2="480" x1="656" />
            <wire x2="1408" y1="1232" y2="1232" x1="1376" />
            <wire x2="1376" y1="1232" y2="1568" x1="1376" />
        </branch>
        <iomarker fontsize="28" x="400" y="1568" name="CLK50" orien="R180" />
        <branch name="PS2_Clk">
            <wire x2="704" y1="288" y2="288" x1="672" />
        </branch>
        <iomarker fontsize="28" x="672" y="288" name="PS2_Clk" orien="R180" />
        <branch name="PS2_Data">
            <wire x2="704" y1="352" y2="352" x1="672" />
        </branch>
        <iomarker fontsize="28" x="672" y="352" name="PS2_Data" orien="R180" />
        <branch name="XLXN_29(2:0)">
            <wire x2="1408" y1="1616" y2="1616" x1="1392" />
            <wire x2="1392" y1="1616" y2="1712" x1="1392" />
            <wire x2="1856" y1="1712" y2="1712" x1="1392" />
            <wire x2="1856" y1="1712" y2="1824" x1="1856" />
            <wire x2="1856" y1="1824" y2="1824" x1="1792" />
        </branch>
        <instance x="1408" y="2048" name="XLXI_6" orien="R0">
        </instance>
        <branch name="R">
            <wire x2="1824" y1="1232" y2="1232" x1="1792" />
        </branch>
        <iomarker fontsize="28" x="1824" y="1232" name="R" orien="R0" />
        <branch name="G">
            <wire x2="1824" y1="1296" y2="1296" x1="1792" />
        </branch>
        <iomarker fontsize="28" x="1824" y="1296" name="G" orien="R0" />
        <branch name="B">
            <wire x2="1824" y1="1360" y2="1360" x1="1792" />
        </branch>
        <iomarker fontsize="28" x="1824" y="1360" name="B" orien="R0" />
        <branch name="HS">
            <wire x2="1824" y1="1424" y2="1424" x1="1792" />
        </branch>
        <iomarker fontsize="28" x="1824" y="1424" name="HS" orien="R0" />
        <branch name="VS">
            <wire x2="1824" y1="1488" y2="1488" x1="1792" />
        </branch>
        <iomarker fontsize="28" x="1824" y="1488" name="VS" orien="R0" />
        <branch name="XLXN_35(9:0)">
            <wire x2="1408" y1="1952" y2="1952" x1="1344" />
            <wire x2="1344" y1="1952" y2="2240" x1="1344" />
            <wire x2="1920" y1="2240" y2="2240" x1="1344" />
            <wire x2="1808" y1="1552" y2="1552" x1="1792" />
            <wire x2="1920" y1="1552" y2="1552" x1="1808" />
            <wire x2="1920" y1="1552" y2="2240" x1="1920" />
        </branch>
        <branch name="XLXN_37(7:0)">
            <wire x2="1104" y1="288" y2="288" x1="1088" />
            <wire x2="2480" y1="288" y2="288" x1="1104" />
            <wire x2="2480" y1="288" y2="2256" x1="2480" />
            <wire x2="1408" y1="1888" y2="1888" x1="1328" />
            <wire x2="1328" y1="1888" y2="2256" x1="1328" />
            <wire x2="2480" y1="2256" y2="2256" x1="1328" />
        </branch>
        <branch name="XLXN_39(8:0)">
            <wire x2="1408" y1="2016" y2="2016" x1="1360" />
            <wire x2="1360" y1="2016" y2="2224" x1="1360" />
            <wire x2="1872" y1="2224" y2="2224" x1="1360" />
            <wire x2="1808" y1="1616" y2="1616" x1="1792" />
            <wire x2="1872" y1="1616" y2="1616" x1="1808" />
            <wire x2="1872" y1="1616" y2="2224" x1="1872" />
        </branch>
        <branch name="XLXN_42">
            <wire x2="1152" y1="416" y2="416" x1="1088" />
            <wire x2="1152" y1="416" y2="736" x1="1152" />
            <wire x2="1216" y1="736" y2="736" x1="1152" />
        </branch>
        <instance x="1216" y="768" name="XLXI_7" orien="R0" />
        <branch name="XLXN_44">
            <wire x2="1408" y1="2144" y2="2144" x1="1376" />
            <wire x2="1376" y1="2144" y2="2192" x1="1376" />
            <wire x2="2000" y1="2192" y2="2192" x1="1376" />
            <wire x2="2000" y1="736" y2="736" x1="1440" />
            <wire x2="2000" y1="736" y2="2192" x1="2000" />
        </branch>
    </sheet>
</drawing>