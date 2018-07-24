# sensor_fsm_base

This is the first base-line version of the project. It is written in pure C and uses Processor Expert under Kinetic Design Studio.

It will not be directly compared to the coroutine version, but stands as a C++-free functional equivalent, for reference.

- Project: Kinetis Design Studio 3.2.0
- SDK: KDS 1.3.0
- Toolchain: clang ??

# Project

Steps to create the project.

1. Start Kinetis Design Studio 3.2.0.
2. File > New > Processor Expert Project
3. Project name: sensor_fsm_base
4. Processor to be used: Processors > Kinetis K > MK20 > MK22F (120 MHz) > MK22FN512xxx12
5. Kinetis SDK: None; Processor Expert: Yes; Project mode: standalone.
6. Target compiler: GNU C Compiler

# Processor Expert Settings

## Processors > Cpu:MK22FN512VDC12

1. Select Clock settings > Clock sources > System oscillator 0
2. System oscillator 0 => checked
3. Select Clock settings > Clock source settings > MCG settings
4. MCG mode => PEE
5. Ensure that MCGFFCLK is 31.25 kHz & PLL output is 100 MHz
6. Select Clock settings > Clock configurations > Clock configuration 0 > System clocks
7. Core clock => 100 MHz; Bus clock => 50 MHz; External bus clock => 50 MHz; Flash clock => 25 MHz
8. Select Common settings > Debug interface (JTAG)
9. JTAG mode => cJTAG/SWD
10. TDO, TCK, TMS all => Unchecked

# Add Components

## ADC

- Component: ADC
- Component repository: Kinetis
- Description: A/D Converter - Legacy High Level Component

1. Name will be AD1.
2. Conversion time => 4 us
3. A/D channels > Channel 0 => ADC1_DP0/ADC0_DP3

## Term

- Component: Term
- Component repository: Kinetis
- Description: ANSI Terminal Protocol - Legacy High Level Component

1. Name will be term1
2. Select Inhr1:AsynchroSerial
3. Channel => UART1
4. Settings > RxD => ADC1_SE5a; TxD => ADC1_SE5a
5. Baud rate => 9600
6. Interrupt service/event > Check and immediately uncheck

## PWMs

- Component: PWM
- Component repository: Kinetis
- Description: Pulse Width Modulation - Legacy High Level Component

### Red

1. Name => Red
2. PWM or PPG device => FTM0_C6V
3. Output pin => PTA1/UART0_RX/FTM0_CH6/JTAG_TDI/EZP_DI
4. Period: 1 ms
5. Starting pulse width: 0 ms
6. Inspect Referenced components > PwmLdd1 > Referenced components > Linked Component TU1
7. Ensure Counter is FTM0_CNT, Counter frequency is 50000 kHz

### Green

Accept shared component TU1

1. Name => Green
2. PWM or PPG device => FTM0_C7V
3. Output pin => PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO
4. Period: 1 ms
5. Starting pulse width: 0 ms
6. Check Referenced components > PwmLdd2 > Referenced components > Linked Component = TU1

### Blue

Accept shared component TU1

1. Name => Blue
2. PWM or PPG device => FTM0_C5V
3. Output pin => ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FBa_AD1/EWM_OUT
4. Period: 1 ms
5. Starting pulse width: 0 ms
6. Check Referenced components > PwmLdd2 > Referenced components > Linked Component = TU1

## TimerInt

- Component: TimerInt
- Component repository: Kinetis
- Description: Periodic interrupt - Legacy High Level Component

Do not accept shared component. Require a new component [Kinetis/TimerUnit_LDD]

1. Name will be TI1.
2. Select Interrupt service/event
3. Periodic interrupt source => PIT_LDVAL0
3. Interrupt period => 20 ms
4. Interrupt priority: low priority (160)
5. Select Initialization
6. Enabled in init. code => No 
7. Select Methods
8. Enable => generate code; Disable => generate code; 
9. Ensure that Events > OnInterrupt is set to TI1_OnInterrupt
10. Select Referenced components > TimerInt_LDD > Referenced components > Linked TimerUnit TU2
11. Ensure PIT, 32 bits, 5000 kHz, Interrupt = INT_PIT0

## Pins1:PinSettings

After adding components, check that 

1. ADC0 > SE6b - Single-ended channel 6b => No pin routed
2. ADC1 > SE4a - Single-ended channel 4a => No pin routed
3. ADC1 > SE5a - Single-ended channel 5a => No pin routed

# Main loop

In main.c, add:

```c
#include "fsm.h"
```

Inside the main(), add:

```c
  for(;;) {
    if (fsm_execute()) {
      __asm ("wfi");
    }
  }
```

This delegates the work to the finite state machine in fsm.h. If the FSM returns non-zero, then no further work is required at this time, and the loop can go to a low power mode, waiting for an interrupt. If false, then there is more work to be done, and the FSM should be called again immediately. 

# Finite state machine

This is declared in fsm.h, as follows:

```c
// Shared state variable
typedef enum {
  FSM_UNINITIALISED = 0,
  FSM_WAIT_CALIBRATION,
  FSM_CALIBRATION_COMPLETE,
  FSM_WAIT_TIMER,
  FSM_TIMER_COMPLETE,
  FSM_WAIT_ADC,
  FSM_ADC_COMPLETE
} fsm_state_t;
extern volatile fsm_state_t fsm_state;

// Shared data buffer
extern word sensor_value;

// Finite state machine
bool fsm_execute();

// FSM transition helper
bool fsm_transition(fsm_state_t from, fsm_state_t to);
```

(Note that we can use `bool` because it is condtionally defined in stdbool.h, for not-C++.)

The state machine is implemented in fsm.c, and contains the following states:

Value | Symbol | Meaning
---   | ---                      | ---
0     | FSM_UNINITIALISED        | Uninitialised
1     | FSM_WAIT_CALIBRATION     | Waiting for ADC calibration to complete
2     | FSM_CALIBRATION_COMPLETE | After calibration, about to start a timer
3     | FSM_WAIT_TIMER           | Waiting for a timer interrupt
4     | FSM_TIMER_COMPLETE       | After timer interrupt, about to request a sensor reading via ADC
5     | FSM_WAIT_ADC             | Waiting on ADC
6     | FSM_ADC_COMPLETE         | After ADC completion. Process value then return to step 3.

The fsm function `fsm_execute()` executes the state machine in ethe main loop. There are also interrupt handlers in Events.c, which handle some of the state transitions, by invoking one of the handlers defined in fsm.c.

```c
void fsm_handle_calibration_complete();
void fsm_handle_timer_interrupt();
void fsm_handle_adc_complete();
```

A utility function `fsm_transition(from, to)` is used to move from one state to another.

The interrupt handlers must deal with all entry states. For example:

```c
void fsm_handle_timer_interrupt() {
	switch (fsm_state) {
	case FSM_UNINITIALISED:
		break;
	case FSM_WAIT_CALIBRATION:
		break;
	case FSM_CALIBRATION_COMPLETE:
		break;
	case FSM_WAIT_TIMER:
		fsm_transition(FSM_WAIT_TIMER, FSM_TIMER_COMPLETE);
		break;
	case FSM_TIMER_COMPLETE:
		break;
	case FSM_WAIT_ADC:
		break;
	case FSM_ADC_COMPLETE:
		break;
	}
}
```
In this instance, only the  state `FSM_WAIT_TIMER` is interesting at this time; none of the other states are handled. Static analysis ('lint') tools will report an error if any entry state is omitted from the list. Compiler optimisation passes will remove the unused cases from the binary code.

# Running the code

In Kinetis Design Studio, build the code with `Ctrl+B`. 

Ensure that the development is connnected to a USB port via the `OpenSDA' connector. Detect the port for the development board in Windows by inspecting Device Manager > Ports (COM & LPT) > OpenSDA.

Start up a terminal by launching PuTTY with a serial connection; the  Serial line should be set to the COM port detected and the speed to 9600 baud.

Then use `F11` to start the debugger. In the *Launch Configuration Selection* dialog, select `sensor_fsm_base_Debug_PNE`.

# Tool chain

Emit linker size info.

1. Project > Properties > C/C++ Build > Settings > Toolchains
2. Check `Print size` (at bottom of page).

