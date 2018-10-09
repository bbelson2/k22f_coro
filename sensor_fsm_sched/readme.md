# sensor_fsm_sched

This is the baseline version of the project. It is written in both pure C and C++. It uses Processor Expert under Kinetic Design Studio.

It will be directly compared to the base-line C++ version, `sensor_fsm_coro_sched`.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: clang

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. Locate project **sensor_fsm_coro_sched** in Project Explorer.
3. Clone it using Copy then Paste. Enter `sensor_fsm_sched` for the new project name.
4. Select the new project, and Project > Properties > C/C++ Build > Settings.
5. Select Build Artifact, and set Artifact name => sensor_fsm_sched.

Add a project specific line in the first state of `fsm_execute()` to check that we are running the correct version, e.g.:

```c
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		Term1_Cls();
		Term1_MoveTo(1,1);
		Term1_SendStr("sensor_fsm_sched initialisation\r\n");
```

Build the new project. 

## Debug

1. Debug Configurations > GDB PEMicro Interface Debugging > right-click > New
2. Rename the new config to sensor_fsm_sched_DebugLLVM_PNE.
3. Set C/C++ Application => DebugLLVM/sensor_fsm_sched.elf
3. Debug the new configuration.

