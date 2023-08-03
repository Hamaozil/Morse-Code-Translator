import time
import serial # to get data from arduino 
import pyttsx3 # to say whatever passed to it 

# initialize engine
engine = pyttsx3.init()
engine.setProperty('rate', 125) # speed of talking 
engine.setProperty('volume', 2.0) # sound 
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id) # female sound 

# creating object from serial lib
arduinoPort = "com5" # the port u have connected usb cable to 
baudRate = 9600 # baudrate u have initialiazed in your arduino code  
arduinoData = serial.Serial(arduinoPort, baudRate)
message=""
time.sleep(1) # optinal
# infint loop to always get data from arduino seial
while True:
    while (arduinoData.in_waiting == 0): # if there is no data then pass
        pass
    dataPacket = arduinoData.readline() # get data
    dataPacket = str(dataPacket, 'utf-8').strip('\r\n') #convert it to string then remove newline 
    message+=dataPacket
    if dataPacket==' ': # if arduino send space that means word have been done then print the whole word and say it 
        print(message)
        engine.say(message)
    else:
        print(dataPacket)
        engine.say(dataPacket)
    engine.runAndWait()
