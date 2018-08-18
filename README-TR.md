# MPU6050-BLE-Python
MPU6050 IMU sensörünün Python ile BLE modülü iletişimi.

Donanım : 
Arduino UNO-NANO-PROMINI
MPU6050 IMU ACC-GYRO Sensor
CC2541 BLE MODULE


İletişim Kısmı

Bu projenin iletişim kısmında 'pygatt' isimli küütphaneyi kullandım. KÜtüphane Python3.x üzerinden şu şekilde kurulabilir ;

pip3 install pygatt

Low Energy modülleri için gerekli olan GATT profile desteği ise sadece UNIX tabanlı sistemler için geçerlidir. Kurulumu ; 

pip3 install "pygatt[GATTTOOL]"


Modelleme Kısmı

Bu projenin modellenmesinde 'OpenGL' ve 'pygame' kütüphanelerini kullandım. Kütüphaneler Python3.x üzerinden şu şekilde kurulabilir ;

pip3 install PyOpenGL 
pip3 install pygame
