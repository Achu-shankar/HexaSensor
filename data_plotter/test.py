import pandas as pd
import yaml
import os

config_file = open("config.yaml", 'r')
config_content = yaml.load(config_file, Loader=yaml.FullLoader)
folder_name = config_content['processed_data_path']
data_file_name = "2_6_22_13_57_15.csv"
data_file_path = os.path.join(folder_name,data_file_name)

df = pd.read_csv(data_file_path,error_bad_lines=False)
df["time"] = df['Timestamp'].str.split(':').str[3] + "-" + df['Timestamp'].str.split(':').str[4] + "-" \
    + df['Timestamp'].str.split(':').str[5] + "-" + df['Timestamp'].str.split(':').str[6]

# print(ndf.head())
print(df.head())
# print(df.head(10))

