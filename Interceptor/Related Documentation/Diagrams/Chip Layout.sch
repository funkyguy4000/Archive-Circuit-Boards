<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.3">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.05" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
</parts>
<sheets>
<sheet>
<plain>
<wire x1="128.27" y1="144.78" x2="154.94" y2="144.78" width="0.1524" layer="94"/>
<wire x1="154.94" y1="144.78" x2="187.96" y2="144.78" width="0.1524" layer="94"/>
<wire x1="187.96" y1="144.78" x2="187.96" y2="198.12" width="0.1524" layer="94"/>
<wire x1="187.96" y1="198.12" x2="154.94" y2="198.12" width="0.1524" layer="94"/>
<wire x1="154.94" y1="198.12" x2="128.27" y2="198.12" width="0.1524" layer="94"/>
<wire x1="128.27" y1="198.12" x2="128.27" y2="144.78" width="0.1524" layer="94"/>
<text x="152.4" y="195.58" size="1.9304" layer="94" align="top-right">LPC4773xxx144
ARM Cortex-M4
with FPU
</text>
<text x="157.48" y="195.58" size="1.9304" layer="94" align="top-left">LPC4773xxx144
ARM Cortex-M0

</text>
<wire x1="154.94" y1="186.69" x2="154.94" y2="198.12" width="0.1524" layer="94"/>
<wire x1="154.94" y1="144.78" x2="154.94" y2="158.75" width="0.1524" layer="94"/>
<text x="154.94" y="180.34" size="1.4224" layer="92" align="center">Flash

1 MB flash
16 kB EEPROM
</text>
<text x="154.94" y="171.45" size="1.4224" layer="92" align="center">Memory

136kB SRAM</text>
<text x="154.94" y="162.56" size="1.4224" layer="92" align="center">ROM

ROM/OTP</text>
<text x="144.78" y="180.34" size="1.4224" layer="91" align="top-right">Running at 
204 MHz</text>
<text x="144.78" y="168.91" size="1.4224" layer="91" align="top-right">Handles audio
data processing

Runs FHT, 
periodogram,
and other DSP 
algorithms</text>
<text x="182.88" y="161.29" size="1.4224" layer="91" align="center">GPIO</text>
<text x="182.88" y="157.48" size="1.016" layer="91" align="center">GPIO0: 8/16</text>
<text x="182.88" y="154.94" size="1.016" layer="91" align="center">GPIO1: 13/16</text>
<text x="182.88" y="152.4" size="1.016" layer="91" align="center">GPIO2: 16/16</text>
<text x="182.88" y="149.86" size="1.016" layer="91" align="center">GPIO3: 16/16</text>
<text x="182.88" y="147.32" size="1.016" layer="91" align="center">GPIO5: 7/18</text>
<text x="182.88" y="181.61" size="1.4224" layer="91" align="center">I2S</text>
<text x="182.88" y="177.8" size="0.8128" layer="91" align="center">I2S1:
Seperate TX/RX
No MCLK pins</text>
<text x="182.88" y="172.72" size="1.4224" layer="91" align="center">I2C:</text>
<text x="182.88" y="167.64" size="1.016" layer="91" align="center">I2C0:
For codec
configuration</text>
<text x="172.72" y="152.4" size="1.4224" layer="91" align="center">CAN:</text>
<text x="172.72" y="148.59" size="1.016" layer="91" align="center">For custom
lighting
systems</text>
<text x="172.72" y="161.29" size="1.4224" layer="91" align="center">JTAG:</text>
<text x="172.72" y="157.48" size="1.016" layer="91" align="center">Possible Trace
support</text>
<text x="182.88" y="195.58" size="1.4224" layer="91" align="center">USART</text>
<text x="182.88" y="191.77" size="1.016" layer="91" align="center">USART0:
For Dev. Boot</text>
<text x="182.88" y="186.69" size="1.016" layer="91" align="center">USART3:
Debug PC
Communication</text>
<text x="172.72" y="172.72" size="1.4224" layer="91" align="center">USB</text>
<text x="172.72" y="167.64" size="1.016" layer="91" align="center">USB1:
Pre-set as
Device or
Host</text>
<text x="172.72" y="184.15" size="1.4224" layer="91" align="center">SSP:
</text>
<text x="172.72" y="179.07" size="1.016" layer="91" align="center">Act as
SPI
peripheral
</text>
<text x="161.29" y="152.4" size="1.4224" layer="91" align="center">RTC:</text>
<text x="161.29" y="148.59" size="1.016" layer="91" align="center">Yay
Time</text>
</plain>
<instances>
</instances>
<busses>
<bus name="B$1">
<segment>
<wire x1="147.32" y1="186.69" x2="162.56" y2="186.69" width="0.3048" layer="92"/>
<wire x1="162.56" y1="186.69" x2="162.56" y2="175.26" width="0.3048" layer="92"/>
<wire x1="162.56" y1="175.26" x2="162.56" y2="166.37" width="0.3048" layer="92"/>
<wire x1="162.56" y1="166.37" x2="162.56" y2="158.75" width="0.3048" layer="92"/>
<wire x1="162.56" y1="158.75" x2="147.32" y2="158.75" width="0.3048" layer="92"/>
<wire x1="147.32" y1="158.75" x2="147.32" y2="166.37" width="0.3048" layer="92"/>
<wire x1="147.32" y1="166.37" x2="147.32" y2="175.26" width="0.3048" layer="92"/>
<wire x1="147.32" y1="175.26" x2="147.32" y2="186.69" width="0.3048" layer="92"/>
<wire x1="147.32" y1="175.26" x2="162.56" y2="175.26" width="0.1524" layer="92"/>
<wire x1="147.32" y1="166.37" x2="162.56" y2="166.37" width="0.1524" layer="92"/>
</segment>
</bus>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<wire x1="187.96" y1="162.56" x2="177.8" y2="162.56" width="0.1524" layer="91"/>
<wire x1="177.8" y1="162.56" x2="177.8" y2="153.67" width="0.1524" layer="91"/>
<wire x1="177.8" y1="153.67" x2="177.8" y2="144.78" width="0.1524" layer="91"/>
<wire x1="177.8" y1="162.56" x2="177.8" y2="173.99" width="0.1524" layer="91"/>
<wire x1="177.8" y1="173.99" x2="177.8" y2="182.88" width="0.1524" layer="91"/>
<wire x1="177.8" y1="182.88" x2="187.96" y2="182.88" width="0.1524" layer="91"/>
<wire x1="177.8" y1="173.99" x2="187.96" y2="173.99" width="0.1524" layer="91"/>
<wire x1="177.8" y1="153.67" x2="167.64" y2="153.67" width="0.1524" layer="91"/>
<wire x1="167.64" y1="153.67" x2="167.64" y2="144.78" width="0.1524" layer="91"/>
<wire x1="177.8" y1="182.88" x2="177.8" y2="186.69" width="0.1524" layer="91"/>
<wire x1="177.8" y1="186.69" x2="177.8" y2="198.12" width="0.1524" layer="91"/>
<wire x1="177.8" y1="198.12" x2="187.96" y2="198.12" width="0.1524" layer="91"/>
<wire x1="167.64" y1="153.67" x2="167.64" y2="162.56" width="0.1524" layer="91"/>
<wire x1="167.64" y1="162.56" x2="177.8" y2="162.56" width="0.1524" layer="91"/>
<wire x1="177.8" y1="173.99" x2="167.64" y2="173.99" width="0.1524" layer="91"/>
<wire x1="167.64" y1="173.99" x2="167.64" y2="162.56" width="0.1524" layer="91"/>
<wire x1="177.8" y1="186.69" x2="167.64" y2="186.69" width="0.1524" layer="91"/>
<wire x1="167.64" y1="186.69" x2="167.64" y2="173.99" width="0.1524" layer="91"/>
<wire x1="167.64" y1="153.67" x2="154.94" y2="153.67" width="0.1524" layer="91"/>
<wire x1="154.94" y1="153.67" x2="154.94" y2="144.78" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<wire x1="154.94" y1="153.67" x2="154.94" y2="144.78" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="6.3" minversion="6.2.2" severity="warning">
Since Version 6.2.2 text objects can contain more than one line,
which will not be processed correctly with this version.
</note>
</compatibility>
</eagle>
