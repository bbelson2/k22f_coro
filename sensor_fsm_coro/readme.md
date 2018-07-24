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

# Coroutine instances

A variable number of resumable coroutine instances were added to the project, along with caller functions. Additionally, the callers and the coroutines were varied in content size, so that the costs of the following could be calculated:

- coroutine infrastructure
- coroutine instance
- coroutine calls  

The various code blocks were included and excluded by a single control macro, `TEST_CONTROL`, whose numeric value controlled the inclusions. To prevent the removal of unused code by the optimiser, all included callers were invoked from a shell function `call_tests()`. 

# Code size

## Data

TEST_CONTROL | Infrastructure | couroutines | callers | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- | --- | --- | --- 
0 | 0 | 0 | 0 | 11100 | 140 | 1236 | 12476 | 30bc
1 | 1 | 1 | 1 | 17348 | 144 | 1256 | 18748 | 493c
2 | 1 | 2 | 2 | 18444 | 144 | 1256 | 19844 | 4d84
3 | 1 | 3 | 3 | 19528 | 144 | 1256 | 20928 | 51c0
4 | 1 | 3 | 5 | 19640 | 144 | 1256 | 21040 | 5230
5 | 1 | 3+1 | 5+1 | 20736 | 144 | 1256 | 22136 | 5678

## Results

A preliminary analysis points towards code size costs which are equal to or less than the following:

Item | Bytes
--- | ---
Infrastructure | 5176
Resumable coroutine | 
Invocation of coroutine |
