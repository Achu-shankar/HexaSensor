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




# print(plot_active_file_list)
#---------------------------------------------------------------------------



# frames = dict()

# df2 = pd.DataFrame()

# for filename in list_of_files:
#     data_file_path = os.path.join(folder_name,filename)
#     # print(data_file_path)
#     try:
#         df = pd.read_csv(data_file_path,error_bad_lines=False) 
#         df["time"] = df['Timestamp'].str.split(':').str[3] + "-" + df['Timestamp'].str.split(':').str[4] + "-" \
#             + df['Timestamp'].str.split(':').str[5] + "-" + df['Timestamp'].str.split(':').str[6]
#     except Exception as e: 
#         print(e)
#     frames[filename] = df
#     df2["time"] = frames[filename]["time"]
#     df2[filename] = frames[filename]["Wind speed"]

# print(df2)


navbar = dbc.Navbar(
            [
                dbc.NavbarBrand("Test"),
                dbc.Nav([dbc.NavLink("Item 1"), dbc.NavLink("Item 2")]),
            ],
            sticky="top",
        )
# ------------------------------------------------------------------------------
# App layout
app.layout = html.Div([

    html.H1("Data plotter", style={'text-align': 'center'}),
    html.Br(),
    
    
    html.Div([
        dcc.Dropdown(options = list_of_files_dict,
                    value = list_of_files[0], id='file-dropdown'),
        html.Div(id='dd-output-container'),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"10%","border-style": "solid","padding":"20px",
                "border-width": "thin"}),
    
    
    html.Div([
        html.Div([dcc.Graph(id='wind-speed', figure={} )]),
        html.Div([dcc.Graph(id='wind_dir', figure={})]),
        # html.Div([dcc.Graph(id='wind_dir_polar', figure={})]),
        html.Div([dcc.Graph(id='wind_acoustic_temp', figure={})]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"30%","border-style": "solid","padding":"20px",
                "border-width": "thin"}),        
    

    html.Div([
        html.Div([dcc.Graph(id='temperature_ms5611', figure={} )]),
        html.Div([dcc.Graph(id='abs_alt_ms5611', figure={})]),
        html.Div([dcc.Graph(id='pressure_ms5611', figure={})]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"220%","border-style": "solid","padding":"20px",
                "border-width": "thin"}),   
   
    # html.Hr(),

     html.Div([
        html.Div([dcc.Graph(id='temperature_aht10', figure={})]),
        html.Div([dcc.Graph(id='rel_hum_aht10', figure={})]),
    ],style = {"position":"absolute","width":"70%","right":"15%","top":"420%","border-style": "solid","padding":"20px",
                "border-width": "thin"})
   
])  


@app.callback(
    [Output("wind-speed", "figure"),
     Output("wind_dir", "figure"),
    #  Output("wind_dir_polar", "figure"),
     Output("wind_acoustic_temp", "figure"),
     Output("temperature_ms5611", "figure"),
     Output("abs_alt_ms5611", "figure"),
     Output("pressure_ms5611", "figure"),
     Output("temperature_aht10", "figure"),
     Output("rel_hum_aht10", "figure")
     ], 
    [Input('file-dropdown', 'value')]
)

def update_output(filename):
    
    fig_wind = {}
    fig_wind_dir = {}
    fig_wind_acou_temp = {}
    fig_tempms5611 =  {}
    fig_absalt5611 = {}
    fig_pressms5611 = {}
    fig_tempaht10 = {}
    fig_relhumaht10 = {}

    if filename != None:
        data_file_path = os.path.join(folder_name,filename)
        # print(data_file_path)
        try:
            df = pd.read_csv(data_file_path,error_bad_lines=False) 
            df["time"] = df['Timestamp'].str.split(':').str[3] + "-" + df['Timestamp'].str.split(':').str[4] + "-" \
                + df['Timestamp'].str.split(':').str[5] + "-" + df['Timestamp'].str.split(':').str[6]
        except Exception as e: 
            print(e)

        fig_wind       = px.line(df,x="time",y= "Wind speed")
        fig_wind.update_layout(title_text='Wind speed ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)

        fig_wind_dir = px.line(df,x="time",y= "Wind angle")
        fig_wind_dir.update_layout(title_text='Wind angle ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)
        
        fig_wind_dir_polar    = px.scatter_polar(df, r="Wind speed", theta="Wind angle" ,color="Wind speed")
        fig_wind_dir_polar.update_layout(title_text='Wind speed Vs Wind direction ', title_x=0.5,title_y=1,title_font_color="RebeccaPurple",title_font_size = 20)
        
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
        
        fig_wind_dir.add_annotation(dict(font=dict(color='black',size=18    ),
                                        x=0.5,
                                        y=1.15,
                                        showarrow=False,
                                        text="N",
                                        textangle=0,
                                        xanchor='center',
                                        xref="paper",
                                        yref="paper"))
                                        
    return  fig_wind,fig_wind_dir, fig_wind_acou_temp,\
            fig_tempms5611, fig_absalt5611, fig_pressms5611, \
            fig_tempaht10, fig_relhumaht10
    


if __name__ == '__main__':
    app.run_server(debug=True)
    


  
