import pandas as pd


filename = 'Radiosonde_Data/20211027120018000120_UPP_RAW_%%%%%_2021102712.txt'
# # with open('Radiosonde_Data/20211027120018000120_UPP_RAW_%%%%%_2021102712.txt','r') as f:
# #     # lines  = f.readlines()
# #     # for i in range(3684):
# #     #     lines = f.readline()
# #     #     print(lines)
# #     for count, line in enumerate(f):
# #         pass
# #     f.close()
# # # print(len(lines))
# # print('Total Lines', count + 1)

# # no_of_lines = count+1
# # with open('Radiosonde_Data/20211027120018000120_UPP_RAW_%%%%%_2021102712.txt','r') as f:
# #     for i in range(no_of_lines-10):
# #         lines = f.readline()
# #         print(lines)

rows_to_skip = [i for i in range(0,18)] + [19]
print(rows_to_skip)
df = pd.read_csv(filename,skiprows = rows_to_skip, skipfooter=10,sep='\t', encoding = "ISO-8859-1")


# import chardet    

# filename = 'Radiosonde_Data/20211027120018000120_UPP_RAW_%%%%%_2021102712.txt'
# rawdata = open(filename, "rb").read()
# result = chardet.detect(rawdata)
# charenc = result['encoding']

# print(charenc)