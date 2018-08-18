#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <math.h>

#define MPU6050_I2C_ADDRESS 0x68
#define FREQ  30.0 

double gSensitivity = 65.5;
double gx = 0, gy = 0, gz = 0;
double gyrX = 0, gyrY = 0, gyrZ = 0;
int16_t accX = 0, accY = 0, accZ = 0;
double gyrXoffs = -281.00, gyrYoffs = 18.00, gyrZoffs = -83.00;

MPU6050 acc_gyro_sens;

SoftwareSerial btSerial(2,3);
 
void setup() 
{
  int error;
  uint8_t c;
  uint8_t sample_div;

  btSerial.begin(9600);
  Wire.begin();
  Serial.begin(9600);
  while(!Serial);
  Serial.println("OK");
  i2c_write_reg (MPU6050_I2C_ADDRESS, 0x6b, 0x00);
  i2c_write_reg (MPU6050_I2C_ADDRESS, 0x1a, 0x01);
  i2c_write_reg(MPU6050_I2C_ADDRESS, 0x1b, 0x08);
  sample_div = 1000 / FREQ - 1;
  i2c_write_reg (MPU6050_I2C_ADDRESS, 0x19, sample_div);

  acc_gyro_sens.initialize();
  Serial.println(acc_gyro_sens.testConnection() ? "MPU6050 Bağlantı Başarılı": "MPU6050 Bağlantı Başarısız.");
  Serial.write("Calibrating...");
  calibrate();
  Serial.write("done.");

}

void loop() 
{
  int error;
  double dT;
  double ax, ay, az;
  unsigned long start_time, end_time;
  start_time = millis();
  read_sensor_data();

  ay = atan2(accX, sqrt( pow(accY, 2) + pow(accZ, 2))) * 180 / M_PI;
  ax = atan2(accY, sqrt( pow(accX, 2) + pow(accZ, 2))) * 180 / M_PI;
  
  gx = gx + gyrX / FREQ;
  gy = gy - gyrY / FREQ;
  gz = gz + gyrZ / FREQ;
  
  gx = gx * 0.96 + ax * 0.04;
  gy = gy * 0.96 + ay * 0.04;

  if(gx != NULL && gy != NULL && gz != NULL)
   {
     btSerial.print(gx, 2);
     btSerial.print("|");
     btSerial.print(gy, 2);
     btSerial.print("|");
     btSerial.println(gz, 2);
   }

  end_time = millis();
  delay(((1/FREQ) * 1000) - (end_time - start_time));  
  
}


void calibrate(){

  int x;
  long xSum = 0, ySum = 0, zSum = 0;
  uint8_t i2cData[6]; 
  int num = 500;
  uint8_t error;

  for (x = 0; x < num; x++){

    error = i2c_read(MPU6050_I2C_ADDRESS, 0x43, i2cData, 6);
    if(error!=0)
    return;

    xSum += ((i2cData[0] << 8) | i2cData[1]);
    ySum += ((i2cData[2] << 8) | i2cData[3]);
    zSum += ((i2cData[4] << 8) | i2cData[5]);
  }
  gyrXoffs = xSum / num;
  gyrYoffs = ySum / num;
  gyrZoffs = zSum / num;

//  Serial.println("Calibration result:");
//  Serial.print(gyrXoffs);
//  Serial.print(", ");
//  Serial.print(gyrYoffs);
//  Serial.print(", ");
//  Serial.println(gyrZoffs);
  
} 
void read_sensor_data(){
 uint8_t i2cData[14];
 uint8_t error;
 // read imu data
 error = i2c_read(MPU6050_I2C_ADDRESS, 0x3b, i2cData, 14);
 if(error!=0)
 return;

 // assemble 16 bit sensor data
 accX = ((i2cData[0] << 8) | i2cData[1]);
 accY = ((i2cData[2] << 8) | i2cData[3]);
 accZ = ((i2cData[4] << 8) | i2cData[5]);

 gyrX = (((i2cData[8] << 8) | i2cData[9]) - gyrXoffs) / gSensitivity;
 gyrY = (((i2cData[10] << 8) | i2cData[11]) - gyrYoffs) / gSensitivity;
 gyrZ = (((i2cData[12] << 8) | i2cData[13]) - gyrZoffs) / gSensitivity;
 
}

// ---- I2C routines

int i2c_read(int addr, int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(addr);
  n = Wire.write(start);
  if (n != 1)
  return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
  return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(addr, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
  return (-11);

  return (0);  // return : no error
}


int i2c_write(int addr, int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(addr);
  n = Wire.write(start);        // write the start address
  if (n != 1)
  return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
  return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
  return (error);

  return (0);         // return : no error
}


int i2c_write_reg(int addr, int reg, uint8_t data)
{
  int error;
  
  error = i2c_write(addr, reg, &data, 1);
  return (error);
}

