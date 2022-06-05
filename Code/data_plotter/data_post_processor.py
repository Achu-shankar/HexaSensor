from tkinter.ttk import Style
import pandas as pd
import plotly.express as px  # (version 4.7.0 or higher)
import plotly.graph_objects as go
from dash import Dash, dcc, html, Input, Output  # pip install dash (version 2.0.0 or higher)
import numpy as np
import io
import tailer
import os
import yaml

config_file = open("config.yaml", 'r')
config_content = yaml.load(config_file, Loader=yaml.FullLoader)
folder_name = config_content['processed_data_path']
data_file_name = config_content['processed_filename']

# GRAPH_INTERVAL = config_content['graph_update_interval']
# NO_DATAPOINTS = 100
app = Dash(__name__)
# folder_name= "New_folder"
base_file_name = "_base.txt"
base_file_path = os.path.join(folder_name,base_file_name)



#---------------------------------------------------------------------------


data_file_path = os.path.join(folder_name,data_file_name)

try:
    df = pd.read_csv(data_file_path,error_bad_lines=False)
    df["time"] = df['Timestamp'].str.split(':').str[3] + "-" + df['Timestamp'].str.split(':').str[4] + "-" \
    + df['Timestamp'].str.split(':').str[5] + "-" + df['Timestamp'].str.split(':').str[6]

    print(df.head(10))
except Exception as e: 
    print(e)

fig_wind           = px.line(df,x="time",y= "Wind speed")
fig_wind.update_layout(title_text='Wind speed ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)

fig_wind_dir = px.line(df,x="time",y= "Wind angle")
fig_wind_dir.update_layout(title_text='Wind angle ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)

fig_wind_acou_temp = px.line(df,x="time",y= "Acoustic temperature windsensor")
fig_wind_acou_temp.update_layout(title_text='Acoustic temperature', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)



fig_tempms5611  = px.line(df,x="time",y= "Temperature_ms5611", title='Temperature ms5611')
fig_tempms5611.update_layout(title_text='Temperature ms5611', title_x=0.5,title_font_color="RebeccaPurple",title_font_size = 20)

fig_absalt5611  = px.line(df,x="time",y= "Absolute_altitude_ms5611", title='Absolute altitude ms5611')
fig_absalt5611.update_layout(title_text='Absolute altitude ms5611 ', title_x=0.5,title_font_color="RebeccaPurple",title_font_size = 20)

fig_pressms5611 = px.line(df,x="time",y= "Pressure_ms5611", title='Pressure ms5611')
fig_pressms5611.update_layout(title_text='Pressure ms5611', title_x=0.5,title_font_color="RebeccaPurple",title_font_size = 20)


fig_tempaht10   = px.line(df,x="time",y= "Temperature_aht10", title='Temperature aht10')
fig_tempaht10.update_layout(title_text='Temperature aht10', title_x=0.5,title_font_color="RebeccaPurple",title_font_size = 20)

fig_relhumaht10 = px.line(df,x="time",y= "Relative humidity_aht10", title='Relative Humidity aht10')
fig_relhumaht10.update_layout(title_text='Relative Humidity aht10', title_x=0.5,title_font_color="RebeccaPurple",title_font_size = 20)




# ------------------------------------------------------------------------------
# App layout
app.layout = html.Div([

    html.H1("Data plotter", style={'text-align': 'center'}),
    html.Br(),
    
    html.Div([
        html.Div([dcc.Graph(id='wind-speed', figure=fig_wind )]),
        html.Div([dcc.Graph(id='wind_dir', figure=fig_wind_dir)]),
        html.Div([dcc.Graph(id='wind_acoustic_temp', figure=fig_wind_acou_temp)]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"10%","border-style": "solid","padding":"20px",
                "border-width": "thin"}),        
    

    html.Div([
        html.Div([dcc.Graph(id='temperature_ms5611', figure=fig_tempms5611 )]),
        html.Div([dcc.Graph(id='abs_alt_ms5611', figure=fig_absalt5611)]),
        html.Div([dcc.Graph(id='pressure_ms5611', figure=fig_pressms5611)]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"200%","border-style": "solid","padding":"20px",
                "border-width": "thin"}),   
   
    # html.Hr(),

     html.Div([
        html.Div([dcc.Graph(id='temperature_aht10', figure=fig_tempaht10)]),
        html.Div([dcc.Graph(id='rel_hum_aht10', figure=fig_relhumaht10)]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"400%","border-style": "solid","padding":"20px",
                "border-width": "thin"})

    
   
])  



if __name__ == '__main__':
    app.run_server(debug=True)
    


  
