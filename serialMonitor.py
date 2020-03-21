import serial

#define the Arduino Serial Monitor
ser = serial.Serial(None,9600)

#STARTOF PRINTTOJS()
def printJavascript():
    if(ser is not None):
        arduinoData = ser.readline().decode('ascii') #Convert bits to ASCII
        arduinoDataClean = arduinoData.strip() #Remove whitespace
    return arduinoDataClean
#ENDOF PRINTTOJS()