// walkerForwardComplete.pde - Two servo walker. 
// Complete code with obstacle avoidance
// (c) Kimmo Karvinen & Tero Karvinen http://BotBook.com
// Updated by Joe Saavedra, 2010


/*
    Fix Note:
    Original source is as follows
    https://github.com/r-Stones/arduino.insectwalker_soncis

    I fixed some source code for my frog robot
    My demo is as follows and this source is shared in the http://smartmecard.wordpress.com

    See my demo (https://youtu.be/IAa7NtJqB2Y)
*/

//Configuration
//#define CONFIG_ARDUINO_UNO //my board is using arduino nano board. so disable it.

#include <Servo.h> 
 
Servo frontServo;  
Servo rearServo;  

//////////////////////////////////////////////////////////
/* Setup pins for sensor */
//////////////////////////////////////////////////////////
#ifdef CONFIG_ARDUINO_UNO // arduino uno board
#define TRIG_PIN 12
#define ECHO_PIN 13

#define FRONT_SERVO_PIN 2
#define REAR_SERVO_PIN  3
#else // my arduino nano board

#define TRIG_PIN 12
#define ECHO_PIN 11

#define FRONT_SERVO_PIN 2  //INT0
#define REAR_SERVO_PIN  3  //INT1
#endif

//////////////////////////////////////////////////////////
/* Servo motors - global variables */
int centerPos = 90; 
int frontRightUp = 72;
int frontLeftUp = 108;
int backRightForward = 75 - 15;
int backLeftForward = 105 + 15;
int walkSpeed = 150; // How long to wait between steps in milliseconds
int centerTurnPos = 81; 
int frontTurnRightUp = 63;
int frontTurnLeftUp = 117;
int backTurnRightForward = 66;
int backTurnLeftForward = 96;

/* Additional variables for the HCSR04 modifications */

int distance;

/* Ping distance measurement - global variables */
long int duration, distanceInches;
long distanceFront=0; //cm
int startAvoidanceDistance=20; //cm

long microsecondsToInches(long microseconds)
{
    /*
        microseconds / 74 / 2 = microseconds / 148
    */
    return microseconds / 148;
}

long microsecondsToCentimeters(long microseconds)
{
    /*
        microseconds / 29 / 2 = microseconds / 58
    */    
    return microseconds / 58;
}

long distanceCm()
{
 
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    /*
        duration / 29 /2 = duration / 58
    */
    distance = duration / 58;

    //Serial.println(distance);
    if (distance >= 200 || distance <= 0)
    {
        Serial.println("Out of range");
    }
    else 
    {
        Serial.print(distance);
        Serial.println(" cm");
    } 
    return distance; 
}

void center()
{
    frontServo.write(centerPos);
    rearServo.write(centerPos);  
}

void moveForward()
{
    frontServo.write(frontRightUp);
    rearServo.write(backLeftForward);
    delay(125);
    
    frontServo.write(centerPos);
    rearServo.write(centerPos);
    delay(65);

    frontServo.write(frontLeftUp);
    rearServo.write(backRightForward);
    delay(125);

    frontServo.write(centerPos);
    rearServo.write(centerPos);
    delay(65);
}

void moveBackRight()
{
    frontServo.write(frontRightUp);
    rearServo.write(backRightForward-6);
    delay(125);

    frontServo.write(centerPos);
    rearServo.write(centerPos-6);
    delay(65);

    frontServo.write(frontLeftUp+9);
    rearServo.write(backLeftForward-6);
    delay(125);

    frontServo.write(centerPos);
    rearServo.write(centerPos);
    delay(65);
}

void moveTurnLeft()
{
    frontServo.write(frontTurnRightUp);
    rearServo.write(backTurnLeftForward);
    delay(125);
    frontServo.write(centerTurnPos);
    rearServo.write(centerTurnPos);
    delay(65);
    frontServo.write(frontTurnLeftUp);
    rearServo.write(backTurnRightForward);
    delay(125);

    frontServo.write(centerTurnPos);
    rearServo.write(centerTurnPos);
    delay(65);
}

void setup()
{
    Serial.begin(9600);
    
    frontServo.attach(FRONT_SERVO_PIN);
    rearServo.attach(REAR_SERVO_PIN);
    

    frontServo.write(centerPos);
    rearServo.write(centerPos); 
    
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}  

void loop()
{
    distanceFront=distanceCm();
    if (distanceFront > 1)
    { 
        // Filters out any stray 0.00 error readings
        if (distanceFront<startAvoidanceDistance) 
        {
            for(int i=0; i<=10; i++) 
            {
                moveBackRight();
                delay(walkSpeed);
            }

            for(int i=0; i<=10; i++) 
            {
                moveTurnLeft();
                delay(walkSpeed);
            }

        } 
        else 
        {
            moveForward();
            delay(walkSpeed);
        }
    }
}

