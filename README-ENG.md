# MPU6050-BLE-Python
MPU6050 Imu communication CC2541 BLE module with Python

Hardware : 
Arduino UNO-NANO-PROMINI
MPU6050 IMU ACC-GYRO Sensor
CC2541 BLE MODULE


Communication Part

I used 'pygatt' for communication on this project. Pygatt can be installed in Python3.x like ;

pip3 install pygatt

GATT Profile support can be installed like (Only UNIX-Bases System);

pip3 install "pygatt[GATTTOOL]"


Modelling Part

I used 'OpenGL' and 'pygame' for modelling IMU values. These requirements will install like ;

pip3 install PyOpenGL 
pip3 install pygame

