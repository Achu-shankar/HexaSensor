from traceback import print_tb
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
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
# ax.clear()
# l0, = ax.plot([],[], lw=2, color='k', label='2 Hz')

fig = plt.figure()
ax = plt.axes()
l0, = ax.plot([], [], lw = 2)
l1, = ax.plot([], [], lw = 2)
l2, = ax.plot([], [], lw = 2)

# Make checkbuttons with all plotted lines with correct visibility

# rax = plt.axes([0.05, 0.4, 0.1, 0.15])
# labels = [str(line.get_label()) for line in lines]
# visibility = [line.get_visible() for line in lines]
# check = CheckButtons(rax, labels, visibility)
# def func(label):
#     index = labels.index(label)
#     lines[index].set_visible(not lines[index].get_visible())
    # plt.draw()

# check.on_clicked(func)

class SensorDataClass:
    def __init__(self):
        self.xs = []
        self.wind_speed = []
        self.wind_angle = []
        self.wind_temp  = []

        self.temp_ms5611 = []
        self.pressure_ms5611 = []
        self.abs_alt_ms5611 = []

        self.roll = []
        self.pitch = []
        self.yaw = []

        self.rollspeed = []
        self.pitchspeed = []
        self.yawspeed = []

        self.x = []
        self.y = []
        self.z = []

        self.vx = []
        self.vy = []
        self.vz = []

        self.pix_lat = []
        self.pix_lon = []
        self.pix_lat = []

    def clear_vars(self):
        self.xs = []
        self.wind_speed = []
        self.wind_angle = []
        self.wind_temp  = []

        self.temp_ms5611 = []
        self.pressure_ms5611 = []
        self.abs_alt_ms5611 = []

        self.roll = []
        self.pitch = []
        self.yaw = []

        self.rollspeed = []
        self.pitchspeed = []
        self.yawspeed = []

        self.x = []
        self.y = []
        self.z = []

        self.vx = []
        self.vy = []
        self.vz = []

        self.pix_lat = []
        self.pix_lon = []
        self.pix_lat = []
                     
data_indx = 0

def update_data(SDC):
    global data_indx
    try:
        line = ser.readline()
        data_arr = line.split(b',')
        print(data_arr)
        
        SDC.wind_speed.append(float(data_arr[0]))
        SDC.wind_angle.append(float(data_arr[1]))
        SDC.wind_temp.append(float(data_arr[2]))

        SDC.temp_ms5611.append(float(data_arr[3]))
        SDC.pressure_ms5611.append(float(data_arr[4]))
        SDC.abs_alt_ms5611.append(float(data_arr[5]))

        SDC.roll.append(float(data_arr[6]))
        SDC.pitch.append(float(data_arr[7]))
        SDC.yaw.append(float(data_arr[8]))

        SDC.rollspeed.append(float(data_arr[9]))
        SDC.pitchspeed.append(float(data_arr[10]))
        SDC.yawspeed.append(float(data_arr[11]))

        SDC.x.append(float(data_arr[12]))
        SDC.y.append(float(data_arr[13]))
        SDC.z.append(float(data_arr[14]))

        SDC.vx.append(float(data_arr[15]))
        SDC.vy.append(float(data_arr[16]))
        SDC.vz.append(float(data_arr[17]))

        SDC.pix_lat.append(float(data_arr[18]))
        SDC.pix_lon.append(float(data_arr[19]))
        SDC.pix_lat.append(float(data_arr[20]))

        data_indx = data_indx+1
        SDC.xs.append(data_indx)
                    
    except:
        print("serial data error")

SDC = SensorDataClass() 

def animate(i):
    global SDC
    update_data(SDC)
    y = SDC.wind_temp

    xs = [j for j in range(len(y))]
    l0.set_data(xs, y)
    l1.set_data(xs, SDC.temp_ms5611)
    plt.xlim(0,max(xs))
    plt.ylim(0,max(y)*1.5)
    plt.subplots_adjust(left=0.2)
    
    if(len(SDC.wind_speed)>500):
        SDC.clear_vars()



# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(), interval=100)
plt.show()






# print([i for i in range(len(SDC.wind_speed))])
    
    # lines = [l0]

    # # Make checkbuttons with all plotted lines with correct visibility
    # rax = plt.axes([0.05, 0.4, 0.1, 0.15])
    # labels = [str(line.get_label()) for line in lines]
    # visibility = [line.get_visible() for line in lines]
    # check = CheckButtons(rax, labels, visibility)
    # def func(label):
    #     index = labels.index(label)
    #     lines[index].set_visible(not lines[index].get_visible())
    #     plt.draw()

    # check.on_clicked(func)

    
    #     # Format plot
    # plt.xticks(rotation=45, ha='right')
    # plt.subplots_adjust(bottom=0.30)
    # plt.subplots_adjust(left=0.2)