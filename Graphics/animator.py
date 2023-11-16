from enum import Enum
from tkinter import *
import time

class SlotStates(Enum):
    EMPTY = 0,
    P1 = -1,
    P2 = 1


class Animator(object):
    def __init__(self, canvas, window):
        self.animating = False
        self.canvas:Canvas = canvas
        self.window = window

    def animatePlacement(self, col, endY, velocity, actualPlayer:SlotStates):
        self.animating = True
        canvas = self.canvas
        circleBorder = 4
        circleDiam = 100
        circleStartY = 0
        circleFinalY = endY*circleDiam

        posY = circleStartY*circleDiam + circleBorder/2
        posX = col*circleDiam + circleBorder/2
        posFY = posY+circleDiam - circleBorder/2
        posFX = posX + circleDiam - circleBorder/2

        fill = ""
        if actualPlayer == SlotStates.P1:
            fill = "red"
        else:
            fill = "blue"

        circle = canvas.create_oval(posX, posY, posFX, posFY, width=circleBorder, fill=fill)
        actualY = circleStartY

        refreshTime = 1/60 #60 fps
        yStep = velocity*refreshTime

        while(actualY < circleFinalY):
            canvas.move(circle, 0, yStep)
            self.window.update()
            time.sleep(refreshTime)
            actualY += yStep

        canvas.moveto(circle, y=circleFinalY)
        self.animating = False
    
    def drawCanvas(self, slots):
        canvas = self.canvas
        canvas.delete('all')

        circleBorder = 4
        circleDiam = 100

        for i in range(6):
            for l in range(7):
                posY = i*circleDiam + circleBorder/2
                posX = l*circleDiam + circleBorder/2
                posFY = posY+circleDiam - circleBorder/2
                posFX = posX + circleDiam - circleBorder/2
                fill = ""
                slot = slots[i][l]

                if slot == SlotStates.EMPTY:
                    continue
                elif slot == SlotStates.P1:
                    fill = "red"
                else:
                    fill = "blue"
                
                canvas.create_oval(posX, posY, posFX, posFY, width=circleBorder, fill=fill)
    
