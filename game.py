from enum import Enum
from tkinter import *
import time

class SlotStates(Enum):
    EMPTY = 0,
    P1 = -1,
    P2 = 1


class Game(object):
    def __init__(self):
        self.slots = []
        self.actualPlayer = SlotStates.P1
        self.animating = False

    def initSlot(self):
        for i in range(6):
            self.slots.append([])
            for l in range(7):
                self.slots[i].append(SlotStates.EMPTY)

    def placeInSlot(self, col, canvas:Canvas, window, velocity):
        self.animating = True
        circleRadius = 100
        circleStartY = 0
        circleFinalY = 0

        for i in range(5, -1, -1):
            if( self.slots[i][col] == SlotStates.EMPTY):
                circleFinalY = i*circleRadius
                self.slots[i][col] = self.actualPlayer
                break

        posY = circleStartY*circleRadius
        posX = col*circleRadius
        posFY = posY+circleRadius
        posFX = posX + circleRadius

        fill = ""
        if self.actualPlayer == SlotStates.P1:
            fill = "red"
        else:
            fill = "blue"

        circle = canvas.create_oval(posX, posY, posFX, posFY, fill=fill)
        actualY = circleStartY

        refreshTime = 1/60 #60 fps
        yStep = velocity*refreshTime
        while(actualY < circleFinalY):
            canvas.move(circle, 0, yStep)
            window.update()
            time.sleep(refreshTime)
            actualY += yStep 
        canvas.moveto(circle, y=circleFinalY)
        self.animating = False
        self.switchPlayer()
    
    def drawCanvas(self, canvas:Canvas):
        canvas.delete('all')
        circleBorder = 3
        circleH = (canvas.winfo_height() - 2*circleBorder)/6
        circleW = (canvas.winfo_width() - 2*circleBorder)/7
        for i in range(6):
            for l in range(7):
                posY = i*circleH+circleBorder; posX = l*circleW+circleBorder
                posFY = posY+circleH; posFX = posX + circleW
                fill = ""
                slot = self.slots[i][l]

                if slot == SlotStates.EMPTY:
                    continue
                elif slot == SlotStates.P1:
                    fill = "red"
                else:
                    fill = "blue"
                
                canvas.create_oval(posX, posY, posFX, posFY, width=circleBorder, fill=fill)
    
    def switchPlayer(self):
        if(self.actualPlayer == SlotStates.P1):
            self.actualPlayer = SlotStates.P2
        else:
            self.actualPlayer = SlotStates.P1