# Testing Documentation:

## Testing/Development Updates:

### 09/11/25: Testing the fan RPM calculation + Generic firmware writing

* I've found that setting the ADC counter period to 181.5 cycles is best for ensuring the ADC buffer indexes stay the same - So that the data doesn't get shifted.


* *I've tested and verified the input fan tachometer logic* by simulating an input tachometer signal (simple square wave) from an Arduino board which gets fed into the STM32 nucleo board. 

* So the frequency readings in Hz is very accurate, although I'm worried that the ``PULSES_PER_REVOLUTION`` parameter is incorrect? Will need to double check this.



### 10/11/25: Testing multiple PWM sources and the fan operation

* I've found out the hard way that the ``HAL_TIM_GenerateEvent(...)`` HAL function which generates a software event everytime PWM is called creates a visible phase shift that results in superposition in the PWM waveform and hence, variable and unstable duty cycle + frequency readings.

* The PSC prescaler value is set to 0 and the ARR value is set to 320 along with setting the input TIM1 clock to 8 MHz to ensure an output PWM frequency of 25 kHz (24.98 kHz from oscilloscope).

* Was unsuccessful in testing the fan PWM firmware, apparently the variable DC voltage source was supplying a non-constant input source voltage to the fans (autoregulating the input power?), leading to a fan speed that was oscillating.

* I'm worried about using the CPU to generate the 3 PWM signals, so I'm considering reimplementing everything using DMA.
 
### 11/11/25: Testing the CAN driver via USB-to-CAN v2

* 




