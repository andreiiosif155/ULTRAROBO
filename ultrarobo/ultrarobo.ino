#include <Servo.h>

// Pin definitions
#define TrigPIN A1  // ultrasonic trigger
#define EchoPIN A2  // ultrasonic echo
#define SERVO_PIN 7 // servo motor

// Objects
Servo myservo;

// Variables
int pos = 0;
long time;
int i;
int RightDistance, LeftDistance; // distances from left/right scans
float Distance = 0.00;           // measured distance in cm
unsigned long startMilis;
unsigned long currentMillis;
const unsigned long period = 100;  // used in loops
const unsigned long period1 = 300; // used in loops
char t;                            // bluetooth command
int enA = 8;                       // motor enable A
int enB = 13;                      // motor enable B

void setup()
{
    startMilis = millis();
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(9, OUTPUT);  // left forward
    pinMode(10, OUTPUT); // left back
    pinMode(11, OUTPUT); // right forward
    pinMode(12, OUTPUT); // right back

    Serial.begin(9600);
    myservo.attach(SERVO_PIN);

    myservo.write(90); // center servo
    pinMode(TrigPIN, OUTPUT);
    pinMode(EchoPIN, INPUT);
    pinMode(SERVO_PIN, OUTPUT);
}

// measure distance with ultrasonic sensor
int Search(void)
{
    float Duration = 0.0;
    float CM = 0.0;
    digitalWrite(TrigPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPIN, LOW);
    Duration = pulseIn(EchoPIN, HIGH);
    CM = (Duration / 58.8);
    return CM;
}

// stop, move back, scan left/right, pick new path
void ChangePath()
{
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    digitalWrite(12, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(9, HIGH);
    analogWrite(enA, 200);
    analogWrite(enB, 200);

    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    myservo.write(12); // look right
    delay(500);
    RightDistance = Search();
    delay(500);
    myservo.write(160); // look left
    delay(1000);
    LeftDistance = Search();
    delay(500);
    myservo.write(80); // center again
    delay(500);
    CompareDistance();
}

// same as ChangePath but with CompareDistance1
void ChangePath1()
{
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    digitalWrite(12, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(9, HIGH);
    analogWrite(enA, 200);
    analogWrite(enB, 200);

    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);

    myservo.write(12); // look right
    delay(500);
    RightDistance = Search();
    delay(500);
    myservo.write(160); // look left
    delay(1000);
    LeftDistance = Search();
    delay(500);
    myservo.write(80); // center again
    delay(500);
    CompareDistance1();
}

// choose path (used in ChangePath1)
void CompareDistance1()
{
    if (RightDistance < LeftDistance)
    {
        digitalWrite(12, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(9, HIGH);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(300);
    }
    else if (LeftDistance < RightDistance)
    {
        digitalWrite(12, LOW);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(300);
    }
    else
    {
        digitalWrite(12, LOW);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(700);
    }
}

// choose path (used in ChangePath)
void CompareDistance()
{
    if (RightDistance > LeftDistance)
    {
        digitalWrite(12, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(9, HIGH);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(300);
    }
    else if (LeftDistance > RightDistance)
    {
        digitalWrite(12, LOW);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(300);
    }
    else
    {
        digitalWrite(12, LOW);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        delay(700);
    }
}

void loop()
{
    if (Serial.available())
    {
        t = Serial.read();
        if (t != 'S')
        {
            Serial.println(t);
        }
    }

    if (t == 'F')
    { // forward
        digitalWrite(12, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
    }
    else if (t == 'B')
    { // backward
        digitalWrite(12, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, HIGH);
        digitalWrite(9, HIGH);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
        Serial.print("B");
    }
    else if (t == 'L')
    { // left
        digitalWrite(12, LOW);
        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(9, LOW);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
    }
    else if (t == 'R')
    { // right
        digitalWrite(12, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(9, HIGH);
        analogWrite(enA, 200);
        analogWrite(enB, 200);
    }
    else if (t == 'W')
    { // auto mode 1
        do
        {
            startMilis = millis();
            myservo.write(80); // forward
            delay(100);
            Distance = Search();
            if (Distance < 30)
            {
                startMilis = millis();
                while (millis() - startMilis < period1)
                {
                    analogWrite(enA, 200);
                    analogWrite(enB, 200);
                    ChangePath();
                }
            }
            else if ((Distance >= 30) && (Distance < 60))
            {
                startMilis = millis();
                while (millis() - startMilis < period1)
                {
                    digitalWrite(12, HIGH);
                    digitalWrite(10, HIGH);
                    digitalWrite(11, LOW);
                    digitalWrite(9, LOW);
                    analogWrite(enA, 200);
                    analogWrite(enB, 200);
                }
            }
            else if ((Distance >= 60) && (Distance < 90))
            {
                startMilis = millis();
                while (millis() - startMilis < period1)
                {
                    digitalWrite(12, HIGH);
                    digitalWrite(10, HIGH);
                    digitalWrite(11, LOW);
                    digitalWrite(9, LOW);
                    analogWrite(enA, 200);
                    analogWrite(enB, 200);
                }
            }
            else
            {
                startMilis = millis();
                while (millis() - startMilis < period1)
                {
                    digitalWrite(12, HIGH);
                    digitalWrite(10, HIGH);
                    digitalWrite(11, LOW);
                    digitalWrite(9, LOW);
                    analogWrite(enA, 250);
                    analogWrite(enB, 250);
                }
            }
        } while (millis() - startMilis < 3000);
    }
    else if (t == 'w')
    { // stop auto mode 1
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        analogWrite(enA, 0);
        analogWrite(enB, 0);
    }
    else if (t == 'U')
    { // auto mode 2
        do
        {
            startMilis = millis();
            myservo.write(80);
            delay(100);
            Distance = Search();
            if (Distance > 15)
            {
                startMilis = millis();
                while (millis() - startMilis < period)
                {
                    analogWrite(enA, 0);
                    analogWrite(enB, 0);
                    ChangePath1();
                }
            }
            else if ((Distance >= 0) && (Distance < 15))
            {
                startMilis = millis();
                while (millis() - startMilis < period)
                {
                    digitalWrite(12, HIGH);
                    digitalWrite(10, HIGH);
                    digitalWrite(11, LOW);
                    digitalWrite(9, LOW);
                    analogWrite(enA, 200);
                    analogWrite(enB, 200);
                }
            }
        } while (millis() - startMilis < 3000);
    }
    else if (t == 'u')
    { // stop auto mode 2
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        analogWrite(enA, 0);
        analogWrite(enB, 0);
    }
    else if (t == 'S')
    { // stop
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
        digitalWrite(10, LOW);
        digitalWrite(9, LOW);
        analogWrite(enA, 0);
        analogWrite(enB, 0);
    }
    delay(10);
}
