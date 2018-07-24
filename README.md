# k22f_coro
Study of code sizes for resumable coroutines using N4680

Project | Description
--- | ---
[sensor_fsm_base](sensor_fsm_base/readme.md) | This is the first base-line version of the project. It is written in pure C and uses Processor Expert under Kinetic Design Studio. It will not be directly compared to the coroutine version, but stands as a C++-free functional equivalent, for reference.
[sensor_fsm_cpp](sensor_fsm_cpp/readme.md) | This is the second base-line version of the project. It is written in both pure C and C++. It can be directly compared to the coroutine version.
[sensor_fsm_coro](sensor_fsm_coro/readme.md) | This is the experimental coroutine version of the project. It is written in both pure C and C++. It can be built with or without a variable quantity of coroutines, used in resumable functions.
