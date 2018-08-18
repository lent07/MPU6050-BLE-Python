import pygatt
import logging
import time
#logging.basicConfig()
#logging.getLogger('pygatt').setLevel(logging.DEBUG)

ax = ay = az = 0.0
gx = gy = gz = 0.0

adapter = pygatt.GATTToolBackend()
device_Adress = "98:7b:f3:c2:f4:42"


def reading(handle, value):
    global ax,ay,az,gx,gy,gz
    data = value.decode('utf-8',errors='ignore')
    fakeangles = data.split('~')
    angles = str(fakeangles).split('|')
    print(fakeangles)

    #if len(angles) == 3:
    #    ax = int(angles[0])
    #    ay = int(angles[1])
    #    az = int(angles[2])


adapter.start()
device = adapter.connect(device_Adress)
if device:
    print("Bağlandı")
chars = device.discover_characteristics()
print(chars)
while 1:

    device.subscribe("0000ffe1-0000-1000-8000-00805f9b34fb",callback=reading)
    #print("AX = {0} - AY = {1} - AZ = {2}".format(ax,ay,az))
    time.sleep(0.5)
    break
    #print(data)
    #value = device.char_read("0000ffe1-0000-1000-8000-00805f9b34fb")
    #print(value)
    #time.sleep(3)
#device = adapter.connect("98:7b:f3:c2:f4:42")
#chars = device.discover_characteristics()
    #value = device.char_read("0000ffe1-0000-1000-8000-00805f9b34fb")
    #print(value)
#device.subscribe('0000ffe1-0000-1000-8000-00805f9b34fb', print_this)
