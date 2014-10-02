This simple demo shows communication between M4 and M0 cores.
Two flags are used: first for commands from M4 to M0 and second for responses from M0 to M4. They both are located in shared memory.
Inactive flags have zero value. So, when flag becomes non-zero, this means that command or response is received.

Algorithm for simple communication:
From M4 to M0:
1. Set command flag.
2. Wait for response flag becomes non-zero.
3. Clear response flag.
From M0 to M4:
1. Wait for command flag becomes non-zero.
2. Clear command flag.
3. Set response flag.

To run this projects you need:
1. Start debugging IPC_DualDebug_M4 project.
2. Run second instance of IAR EWB and open IPC_DualDebug_M0 project. 
3. Start debugging IPC_DualDebug_M0 project.