#include <Servo.h>

Servo forward_esc;
Servo reverse_esc;

const int PWM_pin         = 9;   // Throttle output pin. 1000 is lowest 2000 is highest. 
const int PWM_reverse_pin = 10;  // 'Reverse' pin. 1000 is forward, 2000 is reverse. No other options than those. 

String command = "";  // Stores the incoming serial command

void setup() {
    Serial.begin(9600);

    // Attach both pins as "Servo"
    forward_esc.attach(PWM_pin);                
    reverse_esc.attach(PWM_reverse_pin);

    // Arm the ESC
    Serial.println("Arming ESC...");
    forward_esc.writeMicroseconds(1000); // Minimum throttle for arming
    reverse_esc.writeMicroseconds(1000); // Forward for the reverse pin
    delay(7000); // Allow ESC to arm
    Serial.println("ESC Armed, ready for commands");
}

void loop() {
    // Check Serial for new commands
    if (Serial.available()) {
        Serial.println("Enter: F (forward), B (backward), S (stop)");
        command = Serial.readStringUntil('\n');
        command.trim();
    }

    // --------------------------
    // FORWARD
    // --------------------------
    if (command.equalsIgnoreCase("F")) {
        Serial.println("Switching to Forward Mode");

        // 1) Move throttle to neutral
        forward_esc.writeMicroseconds(1000);
        // 2) Also set 'reverse' pin to neutral
        reverse_esc.writeMicroseconds(1000);
        // 3) Wait a bit so the ESC sees neutral
        delay(500);

        forward_esc.writeMicroseconds(1200);    // forward throttle
        Serial.println("Moving Forward");
    }

    // --------------------------
    // REVERSE
    // --------------------------
    else if (command.equalsIgnoreCase("B")) {
        Serial.println("Switching to Reverse Mode");

        // 1) Move throttle to neutral
        forward_esc.writeMicroseconds(1000);
        // 2) Also set 'reverse' pin to neutral
        reverse_esc.writeMicroseconds(1000);
        // 3) Wait a bit so the ESC sees neutral
        delay(500);

        
        reverse_esc.writeMicroseconds(2000); 
        forward_esc.writeMicroseconds(1200);   // "reverse" throttle
            // your special "reverse" signal
        Serial.println("Moving Backward");
    }

    // --------------------------
    // STOP
    // --------------------------
    else if (command.equalsIgnoreCase("S")) {
        Serial.println("Stopping motor");
        // Just go to neutral on both channels
        forward_esc.writeMicroseconds(1000);
        reverse_esc.writeMicroseconds(1000);
        Serial.println("Motor Stopped");
    }

    // Clear command so it doesn't repeat
    command = "";
}
