/*
   Various Sensors Interfacing V1

   This program will provide reference codes for the following sensors:
      - Temperature
      - Humidity
      - Light level
      - Sound level
      - Motion sensor
      - IR Receiver
   In addition, the data will be timestamped using the Real Time Clock (RTC) module

   Requires:
      - ELEGOO Car Kit
      - MEGA 2560 board
      - Breadbord
      - Wires
      - DHT11 Temperature and Humidy Module
          --> Installation of EduIntro by Arduino LLC
      - Photoresistor
      - Sound Sensor Module
      - HC-SR501 PIR Motion Sensor Module
      - IR Receiver Module (part of ELEGGO Car Kit)
          --> Installation of IRremote library by shirriff, z3t00
      - DS1307 RTC Module
          --> Installation of RTClib by Adafruit

   Pinout (you may also refer to picture for connections):
      - Signal of DHT11 --> Pin 30
      - Photoresitor --> Pin A15
      - A0 of Sound Sensor Module --> Pin A14
      - D0 of Sound Sensor Module --> Pin 48
      - Signal of Motion Sensor Module --> Pin 40
      - Y of IR Receiver module --> Pin 12 (Part of ELEGOO Kit)
      - SDA of RTC Module --> Pin 20
      - SCL of RTC Module --> Pin 21

*/
//For driving
#include <Servo.h>                           // Include servo library
#define ENA 5                                // Enable input for right motors
#define ENB 6                                // Enable input for left motors
#define IN1 7                                // Left forward motion
#define IN2 8                                // Left backward motion
#define IN3 9                                // Right backward motion
#define IN4 11   

#include <SD.h>
#include <SPI.h> //for SD reader

//---------------------------- DHT 11 Temperature and Humidty Module ----------------------------
#include <EduIntro.h>               // Include the EduIntro library
DHT11 dht11(30);                    // Creating DHT11 object sensor on pin 30
//-----------------------------------------------------------------------------------------------


//---------------------------------------- Photoresistor ----------------------------------------
#define PHOTORESISTOR_PIN A15        // Define digital pin location for phototresistor
//-----------------------------------------------------------------------------------------------


//------------------------------------- Sound Sensor Module -------------------------------------
#define SOUND_DIGITAL_PIN 48        // Define digital pin location for sound sensor module
#define SOUND_ANALOG_PIN A14        // Define analog pin location for sound sensor module
//-----------------------------------------------------------------------------------------------


//------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
#define PIR_PIN 40                  // Define pin location for Motion sensor module
//-----------------------------------------------------------------------------------------------


//------------------------------------- IR Receiver Module --------------------------------------
// IMPORTANT: Cannot use IRremote library with DHT (or Servo) due to timer conflict

//#include <IRremote.h>
//#define IR_REC_PIN 12               // Define pin location for IR receiver module
//IRrecv irrecv(IR_REC_PIN);          // Declare object instance of IR receiver
//decode_results results;             // Special type of variable for IR Receiver data
//-----------------------------------------------------------------------------------------------


//------------------------------------- DS1307 RTC Module ---------------------------------------
#include <Wire.h>                     // Include the Wire library for I2C communication
#include "RTClib.h"                   // Include the RTC library
RTC_DS1307 rtc;                       // Declare an RTC object instance
//-----------------------------------------------------------------------------------------------


int loopCounter = 0;                // Keeps track of current void loop() iteration

File myFile;
int pinCS = 53; ///sd reader


void setup() {
  Serial.begin(38400);               // Initialize Serial communications

  //---------------------------------------- Photoresistor ----------------------------------------
  pinMode(PHOTORESISTOR_PIN, INPUT); // Declare photoresistor as input to Arduino
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- Sound Sensor Module -------------------------------------
  pinMode(SOUND_DIGITAL_PIN, INPUT);// Declare sound sensor digital as input to Arduino
  pinMode(SOUND_ANALOG_PIN, INPUT); // Declare sound sensor analog as input to Arduino
  //-----------------------------------------------------------------------------------------------


  //------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
  pinMode(PIR_PIN, INPUT);          // Declare motion sensor as input to Arduino
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- IR Receiver Module --------------------------------------
  //  irrecv.enableIRIn();              // Start the IR receiver module
  //  irrecv.blink13(true);             // On board LED blinks every time the receiver gets a signal
  //-----------------------------------------------------------------------------------------------


  //------------------------------------- DS1307 RTC Module ---------------------------------------
  rtc.begin();                          // Initialize the RTC module
  // Following code should only be ever run once
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));   //sets the RTC to the date & time this sketch was compiled - THIS IS WHAT WE WANT
  // rtc.adjust(DateTime(2020, 12, 23, 3, 0, 0));
  // You may also explicitly set date and time using rtc.adjust(DateTime(2020, 12, 23, 3, 0, 0));
  // Example above evaluates to December 23, 2020 at 3am
  //-----------------------------------------------------------------------------------------------

  //for driving 
  pinMode(ENA, OUTPUT);                     // Enables left wheels to move
  pinMode(ENB, OUTPUT);                     // Enables right wheels to move
  // An analog input (instead of a digital) can also be used to set the speed (instead of using 0 and max)
  pinMode(IN1, OUTPUT);                     // Allows right wheels to turn backwards when HIGH
  pinMode(IN2, OUTPUT);                     // Allows right wheels to turn forwards when HIGH
  pinMode(IN3, OUTPUT);                     // Allows left wheels to turn forward when HIGH
  pinMode(IN4, OUTPUT);



  pinMode(pinCS, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }

   myFile = SD.open("Output.txt", FILE_WRITE);
   if (myFile) {
    Serial.println("Writing to file...");
    // Write to file
    getReading();
    //moveFunct();
    myFile.close(); // close the file
    Serial.println("Done.");
  }
  else {
    Serial.println("error opening test.txt");
  }
 
    
   
  delay(3000);                      // Wait 3 seconds to complete setup (not required)
}






void loop() {
//  if (loopCounter >= 100) {
//    while (1);                      // Only allows 500 iterations of void loop()
//  }
//  loopCounter++;                    // Increment loopCounter by 1
//
//  //------------------------------------- DS1307 RTC Module ---------------------------------------
//  /*
//   * Many functions available (however only some are used). RTClib also includes:
//   *    - TimeSpan(); --> Determine the exact time during the future
//   *    - now.unixTime(); --> Number of seconds that have elapsed since the Unix epoch (Midnight 1/1/1970)
//   *    - etc.
//   *    
//   * NOTE: Cannot implement MPU 6050 and RTS at the same time due to use of SDA/SCL
//   */
//  DateTime now = rtc.now();         // Update RTC every loop and store in DateTime special variable
//  
//  Serial.print(now.hour(), DEC); Serial.print(':');
//  
//  Serial.print(now.minute(), DEC); Serial.print(':');
//  
//  Serial.print(now.second(), DEC); Serial.println();
//  //-----------------------------------------------------------------------------------------------
//
//
//  //---------------------------- DHT 11 Temperature and Humidty Module ----------------------------
//  /*
//      Humidity Range: 20-90% ±5% Relative Humidity (RH)
//      Temperature Range: 0-50 °C ±2% °C
//  */
//  dht11.update();                   // Updates current data in DHT11 (updates at every loop)
//  int C = dht11.readCelsius();      // Reads the temperature in Celsius
//  float F = dht11.readFahrenheit(); // Reads the temperature in Fahrenheit
//  int H = dht11.readHumidity();     // Reads the humidity index
//
//  Serial.print("H: "); Serial.print(H);
//  Serial.print("\tC: "); Serial.print(C);
//  Serial.print("\tF: "); Serial.println(F);
//  //-----------------------------------------------------------------------------------------------
//
//
//  //---------------------------------------- Photoresistor ----------------------------------------
//  /*
//      Resistance at light: 18-50 kΩ
//      Resitance at dark: 2.0 MΩ
//
//      Connection:
//         - Photoresistor connected in series with 10kΩ resistor
//         - Signal pin (A15) connected to resistor and photoresitor node
//  */
//  float light = analogRead(PHOTORESISTOR_PIN);
//  Serial.print("Light level: ");
//  Serial.println(light);
//
//  //-----------------------------------------------------------------------------------------------
//
//
//  //------------------------------------- Sound Sensor Module -------------------------------------
//  /*
//      Sensitivity: 48 to 66 decibels
//      May either have an analog or digital output (different pin for each)
//
//      NOTE: Sounds sensor has a potentiometer, which can be used to set sensitivy
//  */
//  float sound = analogRead(SOUND_ANALOG_PIN);
//  Serial.print("Sound level: ");
//  Serial.println(sound);
//  if (sound > 100) {  //play with this # to set too loud
//    Serial.println("It is too loud");
//  }
//  //-----------------------------------------------------------------------------------------------
//
//
//  //------------------------------ HC-SR501 PIR Motion Sensor Module -------------------------------
//  /*
//      Detects at less than 100 degrees cone angle
//      Output from 0V (LOW) to 3.3V (HIGH) so can be used as a standalone unit
//      Need to wait about 30-60 secnods for te sensor to initialize
//      Several adjustments possible:
//        - Jumper:
//            -- H: Repeat trigger (every time motion is detected, the delay timer is restarted)
//            -- L: Single trigger (turn HIGH as soon as motion is detected and stays for time set below
//        - Potentiometer on the inside:
//            -- Sensitivity adjustment (CW to increase range from 3 to 7 meters)
//        - Potentiometer on the outside
//            -- Time-delay adjustment (CW to increase time-delay from 3 to 300 seconds)
//  */
//  if (digitalRead(PIR_PIN) == HIGH) {
//    Serial.println("Motion Detected!");
//  }
//  //-----------------------------------------------------------------------------------------------
//
//
//  //------------------------------------- IR Receiver Module --------------------------------------
//  /*
//     IR Receiver can detect and decode the various IR data
//
//     The following code does not decode any data as it is a simple outline of the functionality
//     Use the IR remote to test this section
//     Each time IR receiver detects data, it stops so it needs to be resumed at the end
//
//     NOTE: Also has a temperature sensor (like the MPU6050)
//  */
//  //  if (irrecv.decode(&results)) {
//  //    Serial.println("IR remote detected!");
//  //    irrecv.resume();              // Resume functionality of IR receiver module
//  //  }
//  //-----------------------------------------------------------------------------------------------
//
//  Serial.println();
}

void getReading(){
  
  while(loopCounter <=20){
    DateTime now = rtc.now();         // Update RTC every loop and store in DateTime special variable
    
    Serial.print(now.hour(), DEC); Serial.print(':');
    myFile.print(now.hour(), DEC); myFile.print(':');
    Serial.print(now.minute(), DEC); Serial.print(':');
    myFile.print(now.minute(), DEC); myFile.print(':');
    Serial.print(now.second(), DEC); Serial.print("\t\t");
    myFile.print(now.second(), DEC); myFile.print("\t\t");
    
    dht11.update();                   // Updates current data in DHT11 (updates at every loop)
    int C = dht11.readCelsius();      // Reads the temperature in Celsius
    float F = dht11.readFahrenheit(); // Reads the temperature in Fahrenheit
    int H = dht11.readHumidity();     // Reads the humidity index
  
    Serial.print(H);
    myFile.print(H);
    Serial.print("\t\t");
    myFile.print("\t\t");
    Serial.print(C);
    myFile.print(C);
    Serial.print("\t\t");
    myFile.print("\t\t");  
    Serial.println(F);
    myFile.println(F);
    
    float light = analogRead(PHOTORESISTOR_PIN);
    Serial.print("\t\t");
    myFile.print("\t\t"); 
    Serial.println(light);
    myFile.println(light);
    Serial.print("\t\t");
    myFile.print("\t\t"); 
  
    float sound = analogRead(SOUND_ANALOG_PIN);

    Serial.println(sound);
    myFile.println(sound);
    Serial.print("\t\t");
    myFile.print("\t\t"); 
    if (sound > 100) {  //play with this # to set too loud
      Serial.println("It is too loud");
    }
  
    if (digitalRead(PIR_PIN) == HIGH) {
      Serial.println("Motion Detected!");
      myFile.println("Motion Detected!");
    }
//    Serial.println();
//    myFile.println();
    loopCounter++;
  }
  myFile.close();
}

void moveFunct(){
  
}

//
void moveForward()                           // Forward function
{
  digitalWrite(IN1, HIGH);                   // Left forward motion activated
  digitalWrite(IN2, LOW);                    // Left backward motion not activated
  digitalWrite(IN3, LOW);                    // Right backward motion not activated
  digitalWrite(IN4, HIGH);                   // Right forward motion activated
}

void moveBackwards()                         // Backward function
{
  digitalWrite(IN1, LOW);                    // Left forward motion activated
  digitalWrite(IN2, HIGH);                   // Left backward motion not activated
  digitalWrite(IN3, HIGH);                   // Right backward motion not activated
  digitalWrite(IN4, LOW);                    // Right forward motion activated
}

void turnLeft()                              // Left turn function
{
  digitalWrite(IN1, LOW);                    // Left forward motion not activated
  digitalWrite(IN2, HIGH);                   // Left backward motion activated
  digitalWrite(IN3, LOW);                    // Right backward motion not activated
  digitalWrite(IN4, HIGH);                   // Right forward motion activated

}

void wideTurnLeft()                          // Wide left turn function
{
  digitalWrite(IN1, LOW);                    // Left forward motion not activated
  digitalWrite(IN2, LOW);                    // Left backward motion activated
  digitalWrite(IN3, LOW);                    // Right backward motion not activated
  digitalWrite(IN4, HIGH);                   // Right forward motion activated

}

void wideTurnRight()                          // Wide right turn function
{
  digitalWrite(IN1, HIGH);                   // Left forward motion not activated
  digitalWrite(IN2, LOW);                    // Left backward motion activated
  digitalWrite(IN3, LOW);                    // Right backward motion not activated
  digitalWrite(IN4, LOW);                    // Right forward motion activated

}

void turnRight()                             // Right turn function
{
  digitalWrite(IN1, HIGH);                   // Left forward motion activated
  digitalWrite(IN2, LOW);                    // Left backward motion not activated
  digitalWrite(IN3, HIGH);                   // Right backward motion activated
  digitalWrite(IN4, LOW);                    // Right forward motion not activated
}

void stopMoving()                            // Stopping function
{
  digitalWrite(IN1, LOW);                    // Left forward motion not activated
  digitalWrite(IN2, LOW);                    // Left backward motion not activated
  digitalWrite(IN3, LOW);                    // Right backward motion not activated
  digitalWrite(IN4, LOW);                    // Right forward motion not activated
}

void stopMovingForever()                     // Stopping function
{
  digitalWrite(ENA, LOW);                    // Disable right motor
  digitalWrite(ENB, LOW);                    // Disable left motor
}

void setMaxSpeed()                           // Set motors to max speed function
{
  digitalWrite(ENA, HIGH);                   // Disable right motor
  digitalWrite(ENB, HIGH);                   // Disable left motor
}

void setMotorSpeed(int newSpeed) {
  // Avoid setting the motors to 2 different speed as it tends to stop working
  analogWrite(ENA, newSpeed);               // Enable left motor at set speed
  analogWrite(ENB, newSpeed);               // Enable right motor at set speed
}
