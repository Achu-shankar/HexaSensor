# from tkinter.ttk import Style
from ast import parse
import pandas as pd
import plotly.express as px  # (version 4.7.0 or higher)
import plotly.graph_objects as go
from dash import Dash, dcc, html, Input, Output  # pip install dash (version 2.0.0 or higher)
import dash_bootstrap_components as dbc
import numpy as np
import io
import tailer
import os
import yaml
import datetime

config_file = open("config.yaml", 'r')
config_content = yaml.load(config_file, Loader=yaml.FullLoader)
folder_name = config_content['processed_data_path']

# GRAPH_INTERVAL = config_content['graph_update_interval']
# NO_DATAPOINTS = 100
app = Dash(__name__)
# folder_name= "New_folder"
base_file_name = "_base.txt"
base_file_path = os.path.join(folder_name,base_file_name)

list_of_files = os.listdir(folder_name)
plot_active_file_list = []

x_axis_vals = ["Wind speed","Wind angle","Acoustic temperature windsensor",
                "Temperature_ms5611","Absolute_altitude_ms5611","Pressure_ms5611",
                "Temperature_aht10", "Relative humidity_aht10","Roll","Pitch","Yaw","x","y","z",
                "pix_lat","pix_lon","pix_alt"]
y_axis_vals =  ["Wind speed","Wind angle","Acoustic temperature windsensor",
                "Temperature_ms5611","Absolute_altitude_ms5611","Pressure_ms5611",
                "Temperature_aht10", "Relative humidity_aht10","Roll","Pitch","Yaw","x","y","z",
                "pix_lat","pix_lon","pix_alt"]

def parse_file_name(list_of_files):
    parsed_filename = []
    for filename in list_of_files:
        filename_parsed = filename.split(".")[0]
        time_data = filename_parsed.split("_")
        time_data = list(map(int, time_data))
        

        x = datetime.datetime(time_data[2]+2000, time_data[1], time_data[0], time_data[3], time_data[4], time_data[5])
        parsed_filename.append(x.strftime("%b %d %Y %I:%M:%S %p"))

    return parsed_filename

list_of_files_parsed = parse_file_name(list_of_files)

list_of_files_dict = dict(zip(list_of_files, list_of_files_parsed))

type_of_plots = ["Line", "Scatter","Polar"]


# app.layout = html.Div([

#     html.H1("Data plotter", style={'text-align': 'center'}),
#     html.Br(),
    
    
#     html.Div([
#         dcc.Dropdown(options = list_of_files_dict,
#                     value = list_of_files[0], id='demo-dropdown'),
#         html.Div(id='dd-output-container'),
#     ],style = {"position":"absolute","width":"70%","right":"15%","top":"10%","border-style": "solid","padding":"20px",
#                 "border-width": "thin"}),
    
    
#     html.Div([
#         html.Div([dcc.Graph(id='wind-speed', figure={} )]) 
#         ],
#         style = {"position":"absolute","width":"70%","right":"15%","top":"30%","border-style": "solid","padding":"20px",
#                 "border-width": "thin"}),           
   
# ])  


app.layout = html.Div([

    html.Div([
        html.Div([
            dcc.Dropdown(options = list_of_files_dict,
                         value = list_of_files[0], 
                         id='file-dropdown'),
        ],style={"margin": "5px",'width': '48%', 'display': 'inline-block'}),
        
        html.Div([
            dcc.Dropdown(type_of_plots,
                         "Scatter", 
                         id='plot-type-dropdown'),
        ],style={"margin": "5px",'width': '48%', 'float': 'right', 'display': 'inline-block'}),
    ]),

    html.Div([

        html.Div([
            dcc.Dropdown(
                x_axis_vals,
                'Wind angle',
                id='xaxis-column'
            ),
        ], style={"margin": "5px",'width': '48%', 'display': 'inline-block'}),

        html.Div([
            dcc.Dropdown(
                y_axis_vals,
                'Wind speed',
                id='yaxis-column'
            )
        ], style={"margin": "5px",'width': '48%', 'float': 'right', 'display': 'inline-block'})
    ]),

   

    dcc.Graph(id='indicator-graphic'),

])


@app.callback(
    Output('indicator-graphic', 'figure'),
    Input('file-dropdown', 'value'),
    Input('xaxis-column', 'value'),
    Input('yaxis-column', 'value'),
    Input('plot-type-dropdown', 'value'))

def plot_update(filename, xaxis_column,yaxis_column, plot_type):
    print(xaxis_column,yaxis_column,filename)

    data_file_path = os.path.join(folder_name,filename)
    try:
        df = pd.read_csv(data_file_path,error_bad_lines=False) 
    except Exception as e: 
        print(e)
    if plot_type == "Scatter":
        fig = px.scatter(df,x = xaxis_column,y = yaxis_column)
    elif plot_type == "Line":
        fig = px.line(df,x = xaxis_column,y = yaxis_column)
    elif plot_type == "Polar":
        fig = px.scatter_polar(df, r=yaxis_column, theta=xaxis_column ,color=yaxis_column)
    else :
        fig ={}
    # fig.update_layout(title_text='Wind speed ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)


    return fig

if __name__ == '__main__':
    app.run_server(debug=True)