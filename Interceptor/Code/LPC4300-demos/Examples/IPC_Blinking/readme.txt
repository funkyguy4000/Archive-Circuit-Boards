This is advanced inter-process communication demo showing loading M0 program by the M4 core and communication using event interrupts.

To run this demo you need:
1. Build IPC_Blinking_M0 project.
   This will create CM0_image.h file (using post-build command). This file contains M0 binary represented as an array, which can be included in M4 program.
2. Run second instance of IAR EWB and build IPC_Blinking_M4 project.
3. Start debugging IPC_Blinking_M4 and run program or step up to "while (!m0started);" (line 110, M0 code must be loaded by M4 core).
4. Switch back to IPC_Blinking_M0 project, start debugging WITHOUT DOWNLOAD and run the program.
