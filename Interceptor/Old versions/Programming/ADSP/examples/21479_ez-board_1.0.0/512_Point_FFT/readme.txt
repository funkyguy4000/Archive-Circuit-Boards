 Analog Devices, Inc. CrossCore(R) Embedded Studio Application Example
 
 
Project Name: 	512_Point_FFT

Description:  	This project performs 512 point FFT.  The input is given from
		inputcomplx.dat file.  The output can be compared with the Matlab generated output
		FFT512complx.dat. Since the FFT accelerators work with 32 bits the output will be rounded. 
		The 512point FFT is split into 32*16(V*H) matrix V =32, H = 16


Processor: 	ADSP-21479

Tested with:

		ADSP-21479 EZ-BOARD 
 		Analog Devices CrossCore Embedded Studio
    		ADZS-HPUSB-ICE


Instructions:	- Open and build the project.
		- Load project into a debug configuration.
		- Run the program.

		NOTE: steps to below ONLY work for the example when built for debug build configuration.

		- Select Memory from the Window, Show View. 
		- Add memory to monitor by clicking on the "+" icon in the Memory tab.
		- Enter output as the address or expression to monitor and set the memory type to Data(DM) Memory.
		- Click OK.
		- Right click in Data Memory pane to set the format to set the number of columns to 2.
		- Click on the New Renderings tab to set the rendering to Floating point 32 bit.
		- Click on the Add Rendering(s) button and compare the output buffer (output) to the Matlab generated output (FFT512.dat) in the example src folder. They compare 
		up to few decimal points.

Hardware Setup:	No hardware configuration required


Reference:	ADSP-214xx Hardware Reference Manual
		ADSP-21479 EZ-Kit lite Manual

