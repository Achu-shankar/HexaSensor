import serial

ser = serial.Serial()
ser.port = 'COM3' #Arduino serial port
ser.baudrate = 57600
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters



while True:
    try:
        line = ser.readline()
        print(line)
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        break
    except:
        None
