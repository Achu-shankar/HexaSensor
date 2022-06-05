from turtle import delay
import serial
import time
import unicodedata

# stringVal = u'Här är ett exempel på en svensk mening att ge dig.'

# print(unicodedata.normalize('NFKD', stringVal).encode('ascii', 'ignore'))


wind_speed_query = b"$//,WV?*//\r\n"
id_query         = b"$//,ID?*//\r\n"
# id_query  = unicodedata.normalize('NFKD', id_query).encode('ascii', 'ignore')
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'
ser.timeout = 1
ser.open()
while True:
    # print(wind_speed_query)

    # ser.write(wind_speed_query)
    ser.write(wind_speed_query)
    time.sleep(100 / 1000)
    # ser.readline()

    print(ser.readline())
    time.sleep(30/100)
