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

* Faced a ton of errors when testing the CAN driver, the 'fix' for when it came to displaying the CAN data packets on the oscilloscope was to synchronise the bit rates.

* The bit rates of the USB-to-CAN device and the STM32 nucleo board were both configured to be 250 kbits/s after setting the prescaler (for time quantum) to 16, TQBS1 = 2 times and TQBS2 = 1 time. 

* Despite getting valid CAN messages on the oscilloscope after implementing this change, the canalyzer is no longer detecting any CAN message whatsoever. The error code observed after using ``HAL_CAN_GetError()`` was ``0x00200000``.

* *I have successfully configured the bit timing in CAN and learned how to use the canalyzer to debug CAN messages*


### 12/11/25: Tested newly implemented scheduler + A bunch of firmware dev

* This scheduler is to be used for polling tasks at different rates, likely based on their priorities. It works, not much to say here.

### 13/11/25: Tested pressure sensor and temperature sensor (Bosch)

* Pressure sensor works fine. It outputs 96-98 kPa by default. For reference, the average atmospheric pressure at sea level is 101.325 kPa.

* Hooked up NTC temp sense circuit, and, it *almost* works. It returned a base temperature of 35 deg C where the room I was in felt more like 20-22 deg C. I used an external pull-up res of 10k ohms.

* Moving the sensor closer to a lit candle increased it to 39-42 deg C.

* For now, I'll just do a dumb subtraction, i.e., `recorded_temp - 13` until I find the root cause. Hence why it *almost* works.

* Although now I'm worried that we're not accounting for the external pull-up resistor that needs to be added before the thermistor within the temperature sensor. Will need to check w/ elec team in the near future. This isn't in the altium schematic??

### 15/11/25: Testing the other pressure sensor (MIP) and temperature sensor (A-1325).

* Had to recalibrate the MIP pressure sensor by adjusting the max pressure (in psi) assuming the
desired psi is 101.25 kPa. I'm now reading 101.2 - 101.4 kPa. Seems to be working just fine?

* Although I may need to maybe re-adjust the pressure reading on the Bosch pressure sensor so that
both pressure sensors match well.

* Fixed the temperature scaling issue. Previously in the `sensor_inputs.c` driver, my adc code (0-4095)
output gets scaled to 0-3.3V. The NTC beta and Stein-Hart equations rely on a thermal resistance calculation
involving the supply voltage parameter, which I had previously set to 5V. Both temperature sensors returned
a value of 30-33 deg C despite ambient temp being 20-23 deg C. 

* Changing the supply voltage parameter to 3.3V allowed the function to return the correct values as the
formulas are *radiometric*, meaning it only cares about Vout as a fraction of Vs.

* All sensors seem to be working just fine. The next step would be to test the CAN 
serialization/deserialization logic and double-check the receive logic. Although I should probably go
ask an elec team member for directions on reading CAN messages...

### 16/11/25: Checking CAN serialization logic + Prepare for 1st integration test

* ...

...


## Appendix:

### STM32 CAN Error Codes:

```
#define HAL_CAN_ERROR_NONE            (0x00000000U)  /*!< No error                                             */
#define HAL_CAN_ERROR_EWG             (0x00000001U)  /*!< Protocol Error Warning                               */
#define HAL_CAN_ERROR_EPV             (0x00000002U)  /*!< Error Passive                                        */
#define HAL_CAN_ERROR_BOF             (0x00000004U)  /*!< Bus-off error                                        */
#define HAL_CAN_ERROR_STF             (0x00000008U)  /*!< Stuff error                                          */
#define HAL_CAN_ERROR_FOR             (0x00000010U)  /*!< Form error                                           */
#define HAL_CAN_ERROR_ACK             (0x00000020U)  /*!< Acknowledgment error                                 */
#define HAL_CAN_ERROR_BR              (0x00000040U)  /*!< Bit recessive error                                  */
#define HAL_CAN_ERROR_BD              (0x00000080U)  /*!< Bit dominant error                                   */
#define HAL_CAN_ERROR_CRC             (0x00000100U)  /*!< CRC error                                            */
#define HAL_CAN_ERROR_RX_FOV0         (0x00000200U)  /*!< Rx FIFO0 overrun error                               */
#define HAL_CAN_ERROR_RX_FOV1         (0x00000400U)  /*!< Rx FIFO1 overrun error                               */
#define HAL_CAN_ERROR_TX_ALST0        (0x00000800U)  /*!< TxMailbox 0 transmit failure due to arbitration lost */
#define HAL_CAN_ERROR_TX_TERR0        (0x00001000U)  /*!< TxMailbox 0 transmit failure due to transmit error   */
#define HAL_CAN_ERROR_TX_ALST1        (0x00002000U)  /*!< TxMailbox 1 transmit failure due to arbitration lost */
#define HAL_CAN_ERROR_TX_TERR1        (0x00004000U)  /*!< TxMailbox 1 transmit failure due to transmit error   */
#define HAL_CAN_ERROR_TX_ALST2        (0x00008000U)  /*!< TxMailbox 2 transmit failure due to arbitration lost */
#define HAL_CAN_ERROR_TX_TERR2        (0x00010000U)  /*!< TxMailbox 2 transmit failure due to transmit error   */
#define HAL_CAN_ERROR_TIMEOUT         (0x00020000U)  /*!< Timeout error                                        */
#define HAL_CAN_ERROR_NOT_INITIALIZED (0x00040000U)  /*!< Peripheral not initialized                           */
#define HAL_CAN_ERROR_NOT_READY       (0x00080000U)  /*!< Peripheral not ready                                 */
#define HAL_CAN_ERROR_NOT_STARTED     (0x00100000U)  /*!< Peripheral not started                               */
#define HAL_CAN_ERROR_PARAM           (0x00200000U)  /*!< Parameter error                                      */
```





