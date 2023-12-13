from tkinter import *
from animator import *
from math import *
import paho.mqtt.client as mqtt
import threading

root = Tk()
logFrame = Frame(root)
logFrame.grid(column=0, row=0)
indicFrame = Frame(root)
indicFrame.grid(column=0, row=1)

indicadores = []
for i in range(7):
  l = Frame(indicFrame, bg="gray", width=100, height=20)
  l.grid(column=i, row=0)
  indicadores.append(l)

indic_atual = -1

canvasFrame = Frame(root)
canvasFrame.grid(column=0, row=2)
canv = Canvas(canvasFrame, width=700, height=600, bg="white")
canv.grid(column=0, row=0)

animator = Animator(canv, root)

lab = Label(logFrame,text="Ligue-4", font =("Comic Sans", 14))
lab.grid(row=0, column=0)


def thread_function(name):
  client.loop_forever()


def on_message(client, userdata, msg):
  msg.payload = msg.payload.decode("utf-8")
  print(msg.topic + " " + str(msg.payload) + "\n")
  data = str(msg.payload)
  tipo = data[0]
  slots = []
  nmbCol = 7
  nbmLin = 6
  if (tipo == 'J'):

    i = 1
    while (data[i] != ';'):
      actCol = (i - 1) % nmbCol
      actLin = floor((i - 1) / nmbCol)
      if (actCol == 0):
        slots.append([])
      if (data[i] == 'X'):
        slots[actLin].insert(actCol, SlotStates.P1)
      elif data[i] == 'O':
        slots[actLin].insert(actCol,SlotStates.P2)
      else:
        slots[actLin].insert(actCol,SlotStates.EMPTY)
      i += 1
  

    i += 1
    col = int(data[i])
    actualPlayer = SlotStates.EMPTY
    actualPlayerData = data[i + 1]
    

    if (actualPlayerData == 'X'):
      actualPlayer = SlotStates.P1
    elif (actualPlayerData == 'O'):
      actualPlayer = SlotStates.P2

    animator.drawCanvas(slots)
    endY = 0
    for i in range(5, -1, -1):
      if (slots[i][col] == SlotStates.EMPTY):
        slots[i].insert(col, actualPlayer)
        endY = i
        break
    animator.animatePlacement(col, endY, 600, actualPlayer)

    fill = ""

    if actualPlayer == SlotStates.P1:
      fill = "blue"
    else:
      fill = "red"
    
    for i in range(7):
      indicadores[i].config(bg='gray')
    indicadores[col].config(bg= fill)

  elif(tipo == 'F'):
    jogador = data[1] 
    #T = Text(root=root, bg="grey", fg="black")
    if (jogador == '-'):
      
      lab.config(font =("Comic Sans", 14), text="EMPATE!! COMO?") # nao tem comic sans >:(
      lab.grid(column= 0, row=0)
    else:
      cor = "vermelho"
      if (jogador == 'O'):
        cor = "azul"
      
      #lab = Label(logFrame, text = "Jogador "+cor+" venceu!!")
      lab.config(font =("Comic Sans", 14), text = "Jogador "+cor+" venceu!!")
      lab.grid(column= 0, row=0)
  
  elif(tipo == 'C'):
    cob = int(data[1])
    c = data[2]


    fill = ""
    if c == 'O':
      fill = "blue"
    else:
      fill = "red"
    
    print(fill)
    for i in range(7):
      indicadores[i].config(bg='gray')
    indicadores[cob].config(bg=fill)
  
  elif(tipo == 'L'):
    #lab = Label(logFrame, text = "")
    lab.config(font =("Comic Sans", 14), text="Ligue-4")
    canv.delete('all')

    #lab.grid(column= 0, row=0)





def on_connect(client, userdata, flags, rc):
  print("CONECTADO (" + str(rc) + ")")




print("CONECTANDO")
client = mqtt.Client()
client.on_message = on_message
client.on_connect = on_connect
client.connect("broker.mqtt-dashboard.com", 1883, 60)
client.subscribe("PIC_LIGUE_4:JOGO")

x = threading.Thread(target=thread_function, args=(client,))
x.start()

root.mainloop()
