# xv6-plus-plus
xv6 with new functionalities and optimisations

# Processes and Scheduling
- Added new system calls: `getNumProc`(), `getMaxPid`(), `getProcInfo`(pid, &processInfo), `setprio`(n), `getprio`()
- A new weighted scheduling algorithm
- Detailed problem statement: [here](./processes_and_scheduling/xv6-proc.pdf)

# Concurrency and Synchronization
- Added system calls for 10 shared counters: `ucounter init`(), `ucounter set`(idx, val), `ucounter get`(idx)
- Added userspace spinlocks
- Added userspace condition variables
- Added userspace barrier
- Added userspace semaphores
- Detailed problem statement: [here](./concurrency_and_synchronization/xv6-sync.pdf)

# Memory Management
- Added lazy allocation of heap memory for `sbrk`()
- Added copy-on-write implementation for `fork`() system call
- Detailed problem statement: [here](./memory_management/xv6-mem.pdf)

# Setting up
- Download and extract the xv6 public patch
- Download and install x86 emulator QEMU 
- Copy the respective patch from the above folders replacing the files in xv6 code folder
- Run `make` followed by `make qemu-nox`
- To run any testcase, just type the name of the testcase file without extension in qemu e.g. type `testcase3` and press enter to run testcase3.c
- To exit the qemu, press Ctrl-a followed by x
