from enum import Enum
from tkinter import *

class SlotStates(Enum):
    EMPTY = 0,
    P1 = -1,
    P2 = 1


class Game(object):
    def __init__(self):
        self.actualPlayer = SlotStates.P1
        self.slots = []
        self.initSlot()

    def initSlot(self):
        for i in range(6):
            self.slots.append([])
            for l in range(7):
                self.slots[i].append(SlotStates.EMPTY)

    def placeInSlot(self, col):
        for i in range(5, -1, -1):
            if( self.slots[i][col] == SlotStates.EMPTY):
                self.slots[i][col] = self.actualPlayer
                break
        self.switchPlayer()
    
    def switchPlayer(self):
        if(self.actualPlayer == SlotStates.P1):
            self.actualPlayer = SlotStates.P2
        else:
            self.actualPlayer = SlotStates.P1
    
    def updateCanvas(self, canvas:Canvas):
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
