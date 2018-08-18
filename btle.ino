#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 mpuSens;
int ax,ay,az,gx,gy,gz;
String AX,AY,AZ,GX,GY,GZ;
double fax,fay,faz;

SoftwareSerial btSerial(2,3);


void setup() 
{
  btSerial.begin(115200);
  Serial.begin(115200);
  Wire.begin();
  while(!Serial);
  Serial.println("OK");
  mpuSens.initialize();
  Serial.println(mpuSens.testConnection() ? "MPU6050 Bağlantı Başarılı." : "MPU6050 Bağlantı Başarısız.");
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
  fax = double(ax) /16341.0;
  fay = double(ay) /16341.0;
  faz = double(az) /16341.0;

  AX = (fax);
  AY = (fay);
  AZ = (faz);
  GX = (gx);
  GY = (gy);
  GZ = (gz);
  if (AX != "" && AY != "" && AZ != "")
  {
    btSerial.print(AX);
    btSerial.write("|");
    btSerial.print(AY);
    btSerial.write("|");
    btSerial.println(AZ);
    btSerial.write("~");
    btSerial.print(GX);
    btSerial.write("|");
    btSerial.print(GY);
    btSerial.write("|");
    btSerial.println(GZ);
    delay(500);
  }
  /*
  if(GX != "" && GY != "" && GZ != "")
  {
    btSerial.print(GX);
    btSerial.write("|");
    btSerial.print(GY);
    btSerial.write("|");
    btSerial.println(GZ);
    delay(500);

  }
  */

}
