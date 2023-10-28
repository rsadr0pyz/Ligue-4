from animator import *
class Game(object):
    def __init__(self, canvas, window):
        self.animator = Animator(canvas, window)
        self.slots = []
        self.actualPlayer = SlotStates.P1

        for i in range(6):
            self.slots.append([])
            for l in range(7):
                self.slots[i].append(SlotStates.EMPTY)
        
    def switchPlayer(self):
        if(self.actualPlayer == SlotStates.P1):
            self.actualPlayer = SlotStates.P2
        else:
            self.actualPlayer = SlotStates.P1
    
    def placeInSlot(self, col):
        if(self.animator.animating):
            return
        
        self.animator.drawCanvas(self.slots)
        endY = 0
        for i in range(5, -1, -1):
            if( self.slots[i][col] == SlotStates.EMPTY):
                self.slots[i][col] = self.actualPlayer
                endY = i
                break
        self.animator.animatePlacement(col, endY, 600, self.actualPlayer)
        self.switchPlayer()

        