from OpenGL.GL import *
from OpenGL.GLU import *
import pygame
from pygame.locals import *
import pygatt
import logging
import time

ax = ay = az = 0.0
yaw_mode = True

def resize(width, height):
    if height==0:
        height=1
    glViewport(0, 0, width, height)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, 1.0*width/height, 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

def init():
    glShadeModel(GL_SMOOTH)
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glClearDepth(1.0)
    glEnable(GL_DEPTH_TEST)
    glDepthFunc(GL_LEQUAL)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)

def drawText(position, textString):
    font = pygame.font.SysFont ("Courier", 18, True)
    textSurface = font.render(textString, True, (255,255,255,255), (0,0,0,255))
    textData = pygame.image.tostring(textSurface, "RGBA", True)
    glRasterPos3d(*position)
    glDrawPixels(textSurface.get_width(), textSurface.get_height(), GL_RGBA, GL_UNSIGNED_BYTE, textData)

def draw():
    print("AX = {0} - AY = {1} - AZ = {2}".format(ax,ay,az))
    global rquad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity()
    glTranslatef(0,0.0,-7.0)
    osd_text = "Y: " + str("{0:.2f}".format(ay)) + ", X: " + str("{0:.2f}".format(ax))
    if yaw_mode:
        osd_line = osd_text + ", Z: " + str("{0:.2f}".format(az))
    else:
        osd_line = osd_text
    drawText((-2,-2, 2), osd_line)

    glRotatef(az, 0.0, 1.0, 0.0)
    glRotatef(ay ,1.0,0.0,0.0)
    glRotatef(-1*ax ,0.0,0.0,1.0)

    glBegin(GL_QUADS)
    glColor3f(0.0,1.0,0.0)
    glVertex3f( 1.0, 0.2,-1.0)
    glVertex3f(-1.0, 0.2,-1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f( 1.0, 0.2, 1.0)

    glColor3f(1.0,0.5,0.0)
    glVertex3f( 1.0,-0.2, 1.0)
    glVertex3f(-1.0,-0.2, 1.0)
    glVertex3f(-1.0,-0.2,-1.0)
    glVertex3f( 1.0,-0.2,-1.0)

    glColor3f(1.0,0.0,0.0)
    glVertex3f( 1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(-1.0,-0.2, 1.0)
    glVertex3f( 1.0,-0.2, 1.0)

    glColor3f(1.0,1.0,0.0)
    glVertex3f( 1.0,-0.2,-1.0)
    glVertex3f(-1.0,-0.2,-1.0)
    glVertex3f(-1.0, 0.2,-1.0)
    glVertex3f( 1.0, 0.2,-1.0)

    glColor3f(0.0,0.0,1.0)
    glVertex3f(-1.0, 0.2, 1.0)
    glVertex3f(-1.0, 0.2,-1.0)
    glVertex3f(-1.0,-0.2,-1.0)
    glVertex3f(-1.0,-0.2, 1.0)

    glColor3f(1.0,0.0,1.0)
    glVertex3f( 1.0, 0.2,-1.0)
    glVertex3f( 1.0, 0.2, 1.0)
    glVertex3f( 1.0,-0.2, 1.0)
    glVertex3f( 1.0,-0.2,-1.0)
    glEnd()

def read_data():
    def reading(handle, value):
        global ax, ay, az
        ax = ay = az = 0.0
        line_done = 0
        data = value.decode('utf-8',errors='ignore')
        angles = data.split("|")
        if len(angles) == 3:
            ax = float(angles[0])
            ay = float(angles[1])
            az = float(angles[2])
            line_done = 1
    device.subscribe("0000ffe1-0000-1000-8000-00805f9b34fb",callback=reading)


def main():
    global yaw_mode
    video_flags = OPENGL|DOUBLEBUF
    pygame.init()
    screen = pygame.display.set_mode((640,480), video_flags)
    pygame.display.set_caption("Modelleme")
    resize(640,480)
    init()
    frames = 0
    ticks = pygame.time.get_ticks()
    while 1:
        event = pygame.event.poll()
        if event.type == QUIT or (event.type == KEYDOWN and event.key == K_ESCAPE):
            break
        read_data()
        draw()
        pygame.display.flip()
        frames = frames+1

    print ("Fps :  {0}".format((frames*1000)/(pygame.time.get_ticks()-ticks)))

if __name__ == '__main__':
    adapter = pygatt.GATTToolBackend()
    device_Adress = "98:7b:f3:c2:f4:42"
    adapter.start()
    device = adapter.connect(device_Adress)
    if device:
        print("Bağlandı")
    chars = device.discover_characteristics()
    print(chars)
    main()
