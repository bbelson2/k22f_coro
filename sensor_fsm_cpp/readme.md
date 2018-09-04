# sensor_fsm_cpp

This is the second base-line version of the project. It is written in both pure C and C++. It uses Processor Expert under Kinetic Design Studio.

It will be directly compared to the coroutine version.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: gnu and clang

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. Locate project **sensor_fsm_base** in Project Explorer.
3. Clone it using Copy then Paste. Enter `sensor_fsm_cpp` for the new project name.
4. Select the new project, and Project > Properties > C/C++ Build > Settings.
5. Select Build Artifact, and set Artifact name => sensor_fsm_cpp.

Add a project specific line in the first state of `fsm_execute()` to check that we are running the correct version, e.g.:

```c
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		Term1_Cls();
		Term1_MoveTo(1,1);
		Term1_SendStr("sensor_fsm_cpp initialisation\r\n");
```

Build the new project. 

The **build all** command appears to be rebuilding all files regardless of whether they are up to date. See https://mcuoneclipse.com/2015/06/06/eclipse-project-refresh-policy-broken-incremental-build-with-external-make/. As discussed there, change the ***Properties > Settings > C/C++ Build > Refresh policy***, removing the original resource, and adding this. (This step should be part of the project clone process.)

## Debug

1. Debug Configurations > GDB PEMicro Interface Debugging > right-click > New
2. Rename the new config to sensor_fsm_cpp_Debug_PNE.
3. Debug the new configuration.

# Toolchain

Now we alter the tool chain to use LLVM/clang version 5.0.

If necessary, restart Kinetic Studio with clang in the PATH, e.g.:

```
set PATH=E:\llvm-trunk\install\bin;%PATH%
E:\Freescale\KDS_v3\eclipse\kinetis-design-studio.exe -data E:\Source\K22F
```

Create a project configuration which uses clang as a compiler in place of gcc. (We will also set the C++ compiler settings so that they will be copied into any projects cloned from this.)

1. Project > Build Configurations > Manage...
2. New...
3. Name: DebugLLVM; Copy existing settings from Debug. OK.
4. Select DebugLLVM and Set active.
5. Project > Properties > C/C++ Build > Settings
6. Ensure Configuration = DebugLLVM
7. Tool settings > Cross ARM C++ Compiler 
8. Command => clang++
9. Optimization > Language standard => Toolchain default (GNU ISO 1998 C++)
10. Optimization > ABI version => Toolchain default (2)
11. Optimization > Do not use exceptions => checked
12. Optimization > Do not use RTTI => checked.
13. Optimization > Do not use _cxa_atexit() => checked
14. Optimization > Do not use thread-safe statics => checked.
15. Miscellaneous > Other compiler flags => -fshort-enums -target armv7m-none-eabi "-fcoroutines-ts" "-stdlib=libc++" -std=c++14 
16. Includes. Add the following to the list:
- "C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q1-update\arm-none-eabi\include\c++\6.3.1\arm-none-eabi"
- "C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q1-update\arm-none-eabi\include\c++\6.3.1"
- "C:\Program Files (x86)\GNU Tools ARM Embedded\6 2017-q1-update\arm-none-eabi\include"

## Code changes

Move the code of `fsm.c` to `fsm_cpp.cpp` and remove `fsm.c` from the project.

Add conditional `extern "C"` wrappers to fsm.h: 

```c
#ifdef __cplusplus
extern "C" {
#endif
...
#ifdef __cplusplus
} // extern "C"
#endif
```

In `fsm_cpp.cpp`, wrap term1.h:

```c
extern "C" {
#include "Term1.h"
}
```

Fix type for all calls to `Term1_SendStr()`, e.g.:

```c
Term1_SendStr((void*)"Fatal error: Calibration failed\r\n");
```

## Debug the new configuration

1. Debug Configurations
2. Copy Paste from sensor_fsm_cpp_Debug_PNE to sensor_fsm_cpp_Debug_PNE_clang.
3. Select sensor_fsm_cpp_Debug_PNE_clang
4. Build configuration => DebugLLVM
5. C/C++ Application => DebugLLVM/sensor_fsm_cpp.elf

# Code size

Build | text | data | bss | dec | hex
--- | --- | --- | --- | --- | --- 
GCC | 10888 | 140 | 1232 | 12260 | 2fe4
clang without "-fcoroutines-ts" | 11128 | 140 | 1236 | 12504 | 30d8
clang with "-fcoroutines-ts" | 11128 | 140 | 1236 | 12504 | 30d8

