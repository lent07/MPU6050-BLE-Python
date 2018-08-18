#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 mpuSens;
int ax,ay,az,gx,gy,gz;
double fax,fay,faz;

SoftwareSerial btSerial(2,3); //RX | TX


void setup() 
{
  btSerial.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  while(!Serial);
  Serial.println("OK");
  mpuSens.initialize();
  Serial.println(mpuSens.testConnection() ? "MPU6050 Connected" : "MPU6050 Can't Connected");
  // MPU6050 Offset Values
  mpuSens.setXAccelOffset(744);
  mpuSens.setYAccelOffset(1423);
  mpuSens.setZAccelOffset(1300);
  mpuSens.setXGyroOffset(75);
  mpuSens.setYGyroOffset(-33);
  mpuSens.setZGyroOffset(-11); 
}

void loop() 
{
  mpuSens.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  /*
  This scope will change Accelerometers value to angles with simple way.
  fax = double(ax) /16341.0;
  fay = double(ay) /16341.0;
  faz = double(az) /16341.0;
  */
  if (AX != "" && AY != "" && AZ != "")
  {
    btSerial.print(ax);
    btSerial.write("|");
    btSerial.print(ay);
    btSerial.write("|");
    btSerial.println(ay);
    delay(500);
  }

}
