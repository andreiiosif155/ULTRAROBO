ULTRA-ROBO (Arduino Robot with Bluetooth & Ultrasonic Obstacle Avoidance)

Abstract:
Implemented a full pipeline in Arduino C++ to control a mobile robot with both manual and autonomous modes:
- Integrated Bluetooth communication for remote driving
- Connected ultrasonic sensor (HC-SR04) + servo for environment scanning
- Developed obstacle avoidance using left/right distance comparison
- Controlled DC motors via L298N driver for speed and direction
- Implemented two autonomous strategies with different distance thresholds

more detailed implementation can be found below:

setup()
- Configures all motor driver pins (enA, enB, 9–12) as outputs
- Initializes servo at center (90°) and ultrasonic pins (TrigPIN, EchoPIN)
- Starts Serial communication for Bluetooth commands

Search()
- Sends ultrasonic pulse using TrigPIN
- Measures echo return time on EchoPIN
- Converts time into distance in cm using formula cm = duration / 58.8

ChangePath / ChangePath1
- Stops robot
- Moves backward briefly
- Rotates servo to scan right then left
- Stores distances in RightDistance and LeftDistance
- Calls CompareDistance or CompareDistance1 to decide best direction

CompareDistance / CompareDistance1
- If right side is clearer → turn right
- If left side is clearer → turn left
- If both blocked → turn around

loop()
- Reads Bluetooth command t
- Executes manual controls:
    > F → forward
    > B → backward
    > L → turn left
    > R → turn right
    > S → stop

- Executes autonomous modes:
    > W → auto mode 1: scans ahead, avoids obstacles at 30–90 cm
    > U → auto mode 2: variant with threshold at 15 cm
    > w / u → stop auto modes

Variables
enA, enB – PWM pins for motor speed
9, 10, 11, 12 – motor direction control pins
TrigPIN, EchoPIN – ultrasonic sensor
SERVO_PIN – servo motor for scanning
RightDistance, LeftDistance – distances detected on each side
Distance – forward distance
t – received Bluetooth command