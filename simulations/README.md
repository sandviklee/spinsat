### BLDC Motor Control


The ESC (Electronic Speed Controller) receives 50 Hz PWM signals with pulse width 1000 - 2000 microseconds, as shown in the Arduino code.

The lowest signal where the reaction wheel will actually move is 1070 microseconds. Our working interval will probably be around 1070 - 1300 microseconds. Going above that will cause vibrations and high current draw.

There are four pins from the ESC to the MCU (Microcontroller Unit).
1. Red wire. 5 V output from ESC to MCU. This is to eliminate the need for an extra power supply to the MCU.
2. Black wire. Ground. Connect to MCU.
3. White wire. PWM throttle pin. This goes from 1000 - 2000 microseconds, where 1000 is min and 2000 is max throttle.
4. Yellow wire. PWM reverse pin. 1000 microseconds means forward, and 2000 microseconds means reverse. Other parameters between 1001-1999 microseconds are not used.

Working operation
1. First the ESC needs to be armed. This means that the MCU sets PWM PIN=1000 microseconds and PWM reverse PIN=1000 microseconds for seven seconds. After this, the ESC will be on.
2. Then you can choose reverse or forward.
3. If you want to change direction, STOP the motor first. Going directly from forward to reverse and opposite can damage the motor.

Arming
PWM throttle PIN 1000 ms
PWM reverse PIN 1000 ms
(7 seconds)

Forward
PWM throttle PIN 1200 ms
PWM reverse PIN 1000 ms

Reverse
PWM throttle PIN 1200 ms
PWM reverse PIN 2000 ms

Stop
PWM throttle PIN 1000 ms
PWM reverse PIN 1000 ms
