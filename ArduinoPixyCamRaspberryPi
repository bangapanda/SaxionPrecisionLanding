// Autonomous landing - Saxion v1.07 Final
//  
// Script for IR lock
//
// It uses the Arduino's
// ICSP port.  For more information go here:
//
// http://cmucam.org/projects/cmucam5/wiki/Hooking_up_Pixy_to_a_Microcontroller_(like_an_Arduino)
//
//               (Front of drone)
//    (0,0)             ^             (315,0)         
//     ___________________________________
//    | --> X                             |
//    | |                                 |
//    | ˇ Y                               |
//    |                                   |
//    |                 .                 |
//    |                                   |
//    |                                   |
//    |                                   |
//    |___________________________________|
//  
//    (0,207)                         (315,207)
//
//
//    Center = (158, 104)
//    if X  <=  158 then move left
//    if X  >   158 then move right
//    if Y  <=  104 then move up
//    if Y  >   104 then move down
//
//        
//
//
//


// Include
#include <SPI.h>
#include <Pixy.h>
#include <Wire.h>

// This is the main Pixy object
Pixy pixy;

// Ultrasonic vars
const int i2cAddress = 0x66;
long sensor1, ultrasonic;
float lidar;


void setup()
{
  // Create serial monitor
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Starting...\n");

  // Make sure I2C protocol is active
  Wire.requestFrom(i2cAddress, 0);

  // Initialize the pixy
  pixy.init();

  Serial.println("Pixy initialized");
  
  // Get product info.
  Wire.beginTransmission(i2cAddress);
  Wire.write('?');
  Wire.write('\r');
  Wire.endTransmission(true);

  delay(10);

  Wire.requestFrom(i2cAddress, 16);

  char strBuf[16];

  for (int i = 0; i < 16;++i) {
    strBuf[i] = Wire.read();
  }

  Serial.print('[');
  Serial.print(strBuf);
  Serial.print(']');

  Serial.println();

  Serial.println("Setup completed");
  pinMode(2, INPUT);
}







void loop()
{

  Wire.beginTransmission(i2cAddress);
  Wire.write('?');
  Wire.write('l');
  Wire.write('d');
  Wire.write('f');
  Wire.write(',');
  Wire.write('1');
  Wire.write('\r');
  Wire.endTransmission(true);


  Wire.requestFrom(i2cAddress, 16);

  char strBuf[16];

  // Read LiDAR signal
  for (int i = 0; i < 16;++i) {
    strBuf[i] = Wire.read();
  }

  // If measurement == 0, dont print
  if (int(atof(strBuf + 6)*100) != 0){
    Serial.print("LiDAR");
    Serial.print(" = ");
    lidar = atof(strBuf + 6);
    Serial.print(int(lidar*100));
    Serial.println("cm");
  }
  

  // PWM signal from ultrasonic
  sensor1 = pulseIn(2, HIGH);
  ultrasonic = sensor1/58;
  
  Serial.print("UltraSonic");
  Serial.print(" = ");
  Serial.print(ultrasonic);
  Serial.print("cm");
  Serial.println();
  
  // Local vars for PixyCam
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];

  // Grab blocks
  blocks = pixy.getBlocks();

  // If there are blocks, process them
  if (blocks)
  {
      for (j = 0; j < blocks; j++)
      {
        // Print Pixycam X, Y, Width, Height, Angle
        Serial.print(" X:");
        Serial.print(pixy.blocks[j].x);
        Serial.print("xcm Y:");
        Serial.print(pixy.blocks[j].y);
        Serial.print("ycm W:");
        Serial.print(pixy.blocks[j].width);
        Serial.print("wpx H:");
        Serial.print(pixy.blocks[j].height);
        Serial.print("hpx A:");
        Serial.print(pixy.blocks[j].angle);
        Serial.println("deg");

      }  
  }
}
