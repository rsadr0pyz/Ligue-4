from tkinter import *
from animator import *
from game import *
import paho.mqtt.client as mqtt

root = Tk()
bttFrame = Frame(root)
bttFrame.grid()


def on_message(client, userdata, msg):
  print(msg.topic + " " + str(msg.payload))


def on_connect(client, userdata, flags, rc):
  print("Connected with result code " + str(rc))


print("CONECTANDO")
client = mqtt.Client()
client.on_message = on_message
client.on_connect = on_connect
client.connect("broker.mqtt-dashboard.com", 1883, 60)
client.subscribe("PIC_LIGUE_4:JOGO")

client.loop_forever()

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
