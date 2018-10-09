# sensor_fsm_coro_sched

This is the experimental version of the project. It is written in both pure C and C++. It uses Processor Expert under Kinetic Design Studio.

It will be directly compared to the base-line C++ version, `sensor_fsm_coro`.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: clang

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. Locate project **sensor_fsm_coro** in Project Explorer.
3. Clone it using Copy then Paste. Enter `sensor_fsm_coro_sched` for the new project name.
4. Select the new project, and Project > Properties > C/C++ Build > Settings.
5. Select Build Artifact, and set Artifact name => sensor_fsm_coro.

Add a project specific line in the first state of `fsm_execute()` to check that we are running the correct version, e.g.:

```c
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		Term1_Cls();
		Term1_MoveTo(1,1);
		Term1_SendStr("sensor_fsm_coro_sched initialisation\r\n");
```

Build the new project. 

## Debug

1. Debug Configurations > GDB PEMicro Interface Debugging > right-click > New
2. Rename the new config to sensor_fsm_coro_DebugLLVM_PNE.
3. Set C/C++ Application => DebugLLVM/sensor_fsm_coro.elf
3. Debug the new configuration.

# Coroutine infrastructure

A new header file `experimental/future.h` is added: this provides an awaitable.

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
5 | 1 | 3+1 | 5 | 21496 | 144 | 1256 | 22896 | 5970
6 | 1 | 3+1 | 5+1 | 21596 | 144 | 1256 | 22996 | 59d4

## Results

A preliminary analysis points towards code size costs which are equal to or less than the following:

    S1 - S0 = S(Infrastructure) + S(1 coroutine) + S(1 caller)
    S2 - S1 = S(1 coroutine) + S(1 caller)
    (S1 - S0) - (S2 - S1) = S(Infrastructure) = 5176

    S2 - S1 = S(1 coroutine) + S(1 caller) = 1096
    S3 - S2 = S(1 coroutine) + S(1 caller) = 1084
    S(1 coroutine) + S(1 caller) = (1084, 1096]  

    S4 - S3 = S(2 caller) = 112
    S(1 caller) = 56

    S(1 coroutine) = 1084->1096 - 56 = 1028->1040 => 1034

    A large coroutine contains twice as much fat as a standard coroutine, but no extra coroutine-related code.
    S5 - S4 = S(1 large coroutine)
    S(1 large coroutine) - S(1 coroutine) = S(1 coroutine) - S(1 coroutine overhead)
    1856 = 1034 - overhead
    S(Coro Overhead) = 1856 - 1034 = 822

    A large caller contains twice as much fat as a standard caller, but no extra coroutine-related code.
    S6 - s5 = S(1large caller) = 100
    S(large caller) - S(caller) = S(Caller) - S(caller overhead)
    100 - 56 = 56 - S(caller overhead)
    S(caller overhead) = 12


Item | Bytes
--- | ---
Infrastructure | 5176
Resumable coroutine | 822
Invocation of coroutine | 12

# Code size (Release build)

Add a new build ReleaseLLVM, based on DebugLLVM, with:
Properties > C/C++ Build > Settings > Tool Settings > Cross ARM C++ Compiler > Optimazation > Other optimazation flags => -Os

## Data

TEST_CONTROL | Infrastructure | couroutines | callers | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- | --- | --- | --- 
0 | 0 | 0 | 0 | 10684 | 140 | 1236 | 12060 | 2f1c
1 | 1 | 1 | 1 | 11564 | 144 | 1256 | 12964 | 32a4
2 | 1 | 2 | 2 | 11760 | 144 | 1256 | 13160 | 3368
3 | 1 | 3 | 3 | 11956 | 144 | 1256 | 13356 | 342c
4 | 1 | 3 | 5 | 12048 | 144 | 1256 | 13448 | 3488
5 | 1 | 3+1 | 5 | 12344 | 144 | 1256 | 13744 | 35b0
6 | 1 | 3+1 | 5+1 | 12380 | 144 | 1256 | 13780 | 35d4

