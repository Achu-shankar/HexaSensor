from sys import builtin_module_names
from tokenize import String
import serial
import time
import csv
import os
import yaml

config_file = open("config.yaml", 'r')
config_content = yaml.load(config_file, Loader=yaml.FullLoader)
folder_name = config_content['data_folder_path']


ser = serial.Serial()
ser.port = config_content['serial_port'] #Arduino serial port
ser.baudrate = 57600
ser.timeout = 10 #specify timeout when using readline()
ser.open()
if ser.is_open==True:
	print("\nAll right, serial port now open. Configuration:\n")
	print(ser, "\n") #print serial parameters

# folder_name= "New_folder"
base_file_name = "_base.txt"
base_file_path = os.path.join(folder_name,base_file_name)
data_file_name_indx = 0
data_file_path = os.path.join(folder_name,str(data_file_name_indx)+'.csv')
# print(data_file_path)

DATA_FILE_LINES = 1000

if not os.path.isdir(folder_name):
    os.mkdir(folder_name)
    f = open(base_file_path,"w")
    f.write(str(data_file_name_indx))
    f.close()
else :
    f =  open(base_file_path,"r")
    # print(int(f.readline()))
    data_file_name_indx =  int(f.readline())
    data_file_path = os.path.join(folder_name,str(data_file_name_indx)+'.csv')
    f.close()
    # Also need to update data_index?

counter = 0
data_index = 1
start_t = time.time()
while True:
    
    curr_t = time.time()
    if(curr_t-start_t>1.0):
        print(counter)
        counter = 0
        start_t = curr_t
    counter +=1
    try:
        line = ser.readline()
        data_arr = line.split(b',')
        # print(data_arr)
        data_arr_float =list(map(float,data_arr[:-1]))
        data_index +=1
        data_arr_float2 =  [data_index] + data_arr_float + [data_arr[-1]]
        print(data_arr_float2)

        if not data_index%DATA_FILE_LINES:
            data_file_name_indx +=1
            data_file_path = os.path.join(folder_name,str(data_file_name_indx)+'.csv')
            f = open(base_file_path,"w")
            f.write(str(data_file_name_indx))
            f.close()

        with open(data_file_path,"a",newline='') as f:
            # print(data_file_path)
            writer = csv.writer(f,delimiter=",")
            writer.writerow(data_arr_float2)
    except KeyboardInterrupt:
        print("Keyboard Interrupt")
        break
    except:
        print("other error")
        # None
