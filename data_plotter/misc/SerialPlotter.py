import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
from matplotlib.widgets import CheckButtons

#initialize serial port
ser = serial.Serial()
ser.port = 'COM3' #Arduino serial port
ser.baudrate = 57600
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

# Create figure for plotting
# fig = plt.figure()
# ax = fig.add_subplot(1, 1, 1)
# l0, = ax.plot([],[], lw=2, color='k', label='2 Hz')

fig = plt.figure()
ax= plt.axes()
plt.xlim(left=-2)
l0, = ax.plot([], [], lw = 2)

l0.set_data([1,2,3,4],[1,2,3,4])
plt.xlim(0,5)
plt.ylim(0,5)
plt.show()