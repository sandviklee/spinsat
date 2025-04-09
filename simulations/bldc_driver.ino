#include <Servo.h>

Servo forward_esc;
Servo reverse_esc;

const int PWM_pin         = 9;   // Throttle output pin. 1000 is lowest 2000 is highest. 
const int PWM_reverse_pin = 10;  // 'Reverse' pin. 1000 is forward, 2000 is reverse. No other options than those. 

void setup() {
    Serial.begin(9600);

    // Attach both pins as "Servo"
    forward_esc.attach(PWM_pin);                
    reverse_esc.attach(PWM_reverse_pin);

    // Arm the ESC
    Serial.println("Arming ESC... Waiting 10 seconds");
    forward_esc.writeMicroseconds(1000); // Minimum throttle for arming
    reverse_esc.writeMicroseconds(1000); // Neutral position
    delay(10000); // Allow ESC to arm for 10 seconds
    Serial.println("ESC Armed, starting sequence");

    // --------------------------
    // FORWARD
    // --------------------------
    Serial.println("Moving Forward for 5 seconds");
    forward_esc.writeMicroseconds(1200); // Forward throttle
    delay(5000); // Run forward for 5 seconds

    // --------------------------
    // STOP BEFORE REVERSE (5 seconds)
    // --------------------------
    Serial.println("Stopping motor for 5 seconds before reversing");
    forward_esc.writeMicroseconds(1000); // Neutral
    reverse_esc.writeMicroseconds(1000); // Neutral
    delay(5000); // Wait 5 seconds before switching to reverse

    // --------------------------
    // REVERSE
    // --------------------------
    Serial.println("Switching to Reverse Mode");
    reverse_esc.writeMicroseconds(2000); // Activate reverse mode
    forward_esc.writeMicroseconds(1200); // Reverse throttle
    Serial.println("Moving Backward for 5 seconds");
    delay(5000); // Run reverse for 5 seconds

    // --------------------------
    // STOP AFTER REVERSE (5 seconds)
    // --------------------------
    Serial.println("Stopping motor for 5 seconds after reverse");
    forward_esc.writeMicroseconds(1000); // Neutral
    reverse_esc.writeMicroseconds(1000); // Neutral
    delay(5000); // Wait 5 seconds before completing sequence

    // --------------------------
    // FINAL STOP MESSAGE
    // --------------------------
    Serial.println("Motor Stopped Completely");
}

void loop() {
    // Nothing needed in loop since operation is automatic
}
