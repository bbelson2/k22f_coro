# sensor_fsm_coro

This is the experimental version of the project. It is written in both pure C and C++. It uses Processor Expert under Kinetic Design Studio.

It will be directly compared to the base-line C++ version, `sensor_fsm_cpp`.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: clang

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. Locate project **sensor_fsm_cpp** in Project Explorer.
3. Clone it using Copy then Paste. Enter `sensor_fsm_coro` for the new project name.
4. Select the new project, and Project > Properties > C/C++ Build > Settings.
5. Select Build Artifact, and set Artifact name => sensor_fsm_coro.

Add a project specific line in the first state of `fsm_execute()` to check that we are running the correct version, e.g.:

```c
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		Term1_Cls();
		Term1_MoveTo(1,1);
		Term1_SendStr("sensor_fsm_coro initialisation\r\n");
```

Build the new project. 

## Debug

1. Debug Configurations > GDB PEMicro Interface Debugging > right-click > New
2. Rename the new config to sensor_fsm_coro_DebugLLVM_PNE.
3. Set C/C++ Application => DebugLLVM/sensor_fsm_coro.elf
3. Debug the new configuration.

# Coroutine infrastructure

A new header file `experimental/coroutine.h` is added: this provides a minimal implementation of N4680. A resumable object class `resumable` is added, in `resumable.h`.

# Code size

## Method

## Results

Infrastructure | couroutines | callers | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- | --- | --- 
0 | 0 | 0 | 11128 | 140	| 1236 | 12504 | 30d8
1 | 1 | 1 | 17344 | 144	| 1256 | 18744 | 4938
1 | 2 | 2 | 17384 | 144 | 1256 | 18784 | 4960
1 | 3 | 3 | 17420 | 144 | 1256 | 18820 | 4984
1 | 3 | 5 | 17420 | 144 | 1256 | 18820 | 4984

