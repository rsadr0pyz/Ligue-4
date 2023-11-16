from tkinter import *
from animator import *
from game import *

root = Tk()
bttFrame = Frame(root)
bttFrame.grid()


canvasFrame = Frame(root)
canvasFrame.grid(column=0, row=1)
canv = Canvas(canvasFrame, width=700, height=600, bg="white")
canv.grid(column=0, row=1)

game = Game(canv, root)

for l in range(7):
    f = Frame(bttFrame, height=32, width=100)
    f.grid(row=0, column=l)

    btt = Button(f, text="O", padx=0, width=1)
    btt.place(x=0, y=0, relheight=1, relwidth=1)

    def bttFunc(event, l=l):
        game.placeInSlot(l)

    btt.bind("<ButtonPress>", bttFunc)



root.mainloop()