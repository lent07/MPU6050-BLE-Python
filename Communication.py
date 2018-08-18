import pygatt
import logging
import time
#logging.basicConfig()
#logging.getLogger('pygatt').setLevel(logging.DEBUG)

ax = ay = az = 0.0
gx = gy = gz = 0.0

adapter = pygatt.GATTToolBackend()
device_Adress = "98:7b:f3:c2:f4:42" #YOUR DEVICE ADDRESS


def reading(handle, value):
    data = value.decode('utf-8',errors='ignore')
    print(data)

adapter.start()
device = adapter.connect(device_Adress)
if device:
    print("Connected")
chars = device.discover_characteristics()
print(chars)

while 1:
    device.subscribe("0000ffe1-0000-1000-8000-00805f9b34fb",callback=reading) #DEVICE NOTIFY CHARACTERISTICS
