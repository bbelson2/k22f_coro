# sensor_fsm_coro

This is the experimental version of the project. It is written in both pure C and C++. It uses Processor Expert under Kinetic Design Studio.

It will be directly compared to the base-line C++ version, `sensor_fsm_cpp`.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: clang

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. Locate project `sensor_fsm_cpp` in Project Explorer.
3. Clone it using ***Copy*** then ***Paste***. Enter `sensor_fsm_coro` for the new project name.
4. Select the new project, and ***Project > Properties > C/C++ Build > Settings***.
5. Select ***Build Artifact***, and set ***Artifact name*** => `sensor_fsm_coro`.
6. Select ***Project > Properties > C/C++ Build > Refresh policy***. 
7. Click ***Add Resource...*** and add the new project. Remove the old one from the list. (This repairs the incremental build.)

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

TEST_CONTROL | Infrastructure | coroutines | callers | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- | --- | --- | --- 
0 | 0 | 0 | 0 | 11100 | 140 | 1236 | 12476 | 30bc
1 | 1 | 1 | 1 | 17348 | 144 | 1256 | 18748 | 493c
2 | 1 | 2 | 2 | 18444 | 144 | 1256 | 19844 | 4d84
3 | 1 | 3 | 3 | 19528 | 144 | 1256 | 20928 | 51c0
4 | 1 | 3 | 5 | 19640 | 144 | 1256 | 21040 | 5230
5 | 1 | 3+1 | 5 | 21496 | 144 | 1256 | 22896 | 5970
6 | 1 | 3+1 | 5+1 | 21596 | 144 | 1256 | 22996 | 59d4

21496	    144	   1256	  22896	   5970
21596	    144	   1256	  22996	   59d4

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

    S(1 coroutine) = 1084->1096 - 56 = 1028->1040 => 1034 - 56 = 978

    A large coroutine contains twice as much fat as a standard coroutine, but no extra coroutine-related code.
    S5 - S4 = S(1 large coroutine)
    S(1 large coroutine) - S(1 coroutine) = S(1 coroutine) - S(1 coroutine overhead)
    1856 - 978 = 978 - overhead
    S(Coro Overhead) = (2*978) - 1856 = 100

    A large caller contains twice as much fat as a standard caller, but no extra coroutine-related code.
    S6 - s5 = S(1 large caller) = 100
    S(large caller) - S(caller) = S(Caller) - S(caller overhead)
    100 - 56 = 56 - S(caller overhead)
    S(caller overhead) = 12


Item | Bytes
--- | ---
Infrastructure | 5176
Resumable coroutine | 100
Invocation of coroutine | 12

# Code size (Release build)

Add a new build ReleaseLLVM, based on DebugLLVM, with:
Properties > C/C++ Build > Settings > Tool Settings > Cross ARM C++ Compiler > Optimization > Other optimization flags => -Os

## Data

TEST_CONTROL | Infrastructure | coroutines | callers | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- | --- | --- | --- 
0 | 0 | 0 | 0 | 10684 | 140 | 1236 | 12060 | 2f1c
1 | 1 | 1 | 1 | 11564 | 144 | 1256 | 12964 | 32a4
2 | 1 | 2 | 2 | 11760 | 144 | 1256 | 13160 | 3368
3 | 1 | 3 | 3 | 11956 | 144 | 1256 | 13356 | 342c
4 | 1 | 3 | 5 | 12048 | 144 | 1256 | 13448 | 3488
5 | 1 | 3+1 | 5 | 12344 | 144 | 1256 | 13744 | 35b0
6 | 1 | 3+1 | 5+1 | 12380 | 144 | 1256 | 13780 | 35d4

## Results

A preliminary analysis points towards code size costs which are equal to or less than the following:

    S1 - S0 = S(Infrastructure) + S(1 coroutine) + S(1 caller)
    S2 - S1 = S(1 coroutine) + S(1 caller)
    (S1 - S0) - (S2 - S1) = S(Infrastructure) = 708

    S2 - S1 = S(1 coroutine) + S(1 caller) = 196
    S3 - S2 = S(1 coroutine) + S(1 caller) = 196
    S(1 coroutine) + S(1 caller) = 196  

    S4 - S3 = S(2 caller) = 92
    S(1 caller) = 46

    S(1 coroutine) = 196 - 46 = 150

    A large coroutine contains twice as much fat as a standard coroutine, but no extra coroutine-related code.
    S5 - S4 = S(1 large coroutine)
    S(1 large coroutine) - S(1 coroutine) = S(1 coroutine) - S(1 coroutine overhead)
    296 - 150 = 150 - overhead
    S(Coro Overhead) = 300 - 296 = 4 (unlikely)

    A large caller contains twice as much fat as a standard caller, but no extra coroutine-related code.
    S6 - s5 = S(1 large caller) = 36
    S(large caller) - S(caller) = S(Caller) - S(caller overhead)
    36 - 46 = 46 - S(caller overhead)
    S(caller overhead) = 56


Item | Bytes
--- | ---
Infrastructure | 708
Resumable coroutine | 4
Invocation of coroutine | 56