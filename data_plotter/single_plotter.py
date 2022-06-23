# from tkinter.ttk import Style
from ast import parse
from tkinter.ttk import Style
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


app = Dash(__name__)
base_file_name = "_base.txt"
base_file_path = os.path.join(folder_name,base_file_name)

list_of_files = os.listdir(folder_name)
list_of_files_radiosonde = os.listdir("Radiosonde_Data")

# print(list_of_files_radiosonde)

x_axis_vals = ["Wind speed","Wind angle","Acoustic temperature windsensor",
                "Temperature_ms5611","Absolute_altitude_ms5611","Pressure_ms5611",
                "Temperature_aht10", "Relative humidity_aht10","Roll","Pitch","Yaw","x","y","z",
                "pix_lat","pix_lon","pix_alt"]
y_axis_vals =  ["Wind speed","Wind angle","Acoustic temperature windsensor",
                "Temperature_ms5611","Absolute_altitude_ms5611","Pressure_ms5611",
                "Temperature_aht10", "Relative humidity_aht10","Roll","Pitch","Yaw","x","y","z",
                "pix_lat","pix_lon","pix_alt"]

radiosonde_parameter_value_list = ["Time","UTC Time","P","T","Hu","Ws","Wd","Long.",
                                    "Lat.","Geopot","MRI","RI","Dewp.","Rs","Elevation","Azimuth","Range","D"]
radiosonde_parameter_value_dict = {}


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



app.layout = html.Div([

    # Side input menu
    html.Div([

        # Hexa Sensor menu section
        html.Div([
            # Section heading
             html.Div([
                html.H2("Hexa Sensor Input"),
            ] ,className = "section-heading"),

            # file input section for hexa sensor
            html.Div([
                html.Div([ html.P(['Hexa sensor filename'],style = {"margin":"0px"})
                ], className = "inputfield-header"),
                html.Div([
                dcc.Dropdown(options = list_of_files_dict,
                            value = list_of_files[0], 
                            id='file-dropdown',multi = True),
                ]),
            
            ]),

            # Graph axis value selection section for hexa sensor
            html.Div([
                # X axis value selector
                html.Div([
                    html.Div([ html.P(['x- axis : Hexa sensor'],style = {"margin":"0px"})
                    ], className = "inputfield-header"),

                    html.Div([
                        dcc.Dropdown(
                            x_axis_vals,
                            'Wind angle',
                            id='xaxis-column-hexa-sensor'
                        )
                    ])

                ]),
                # Y axis value selector
                html.Div([
                    html.Div([ html.P(['y- axis : Hexa sensor'],style = {"margin":"0px"})
                     ], className = "inputfield-header"),
                    html.Div([
                        dcc.Dropdown(
                            y_axis_vals,
                            'Wind speed',
                            id='yaxis-column-hexa-sensor'
                        )
                    ])
            
                ]),
                
            ]),

            # Parameter range selection section for hexa sensor
            html.Div([
                html.Div([ html.P(['x filter value'],style = {"margin":"0px"})
                ], className = "inputfield-header"),
                html.Div([
                    html.Div([
                        dcc.Dropdown(
                            x_axis_vals,
                            'Wind angle',
                            id='x-filter-val-hexa',
                        ),
                        ],className = 'filter-param-name'),

                    html.Div([
                        dcc.Input(id='x-filter-input-hexa', 
                                type='number',
                                debounce=True),
                    ],className = 'filter-param-val'),
                ], style = {'display':'flex'})
                
            ])

        ]),
        
        # Radiosonde sensor menu section
        html.Div([
            # Section heading
            html.Div([
                html.H2("Radiosonde Sensor Input"),
            ] ,className = "section-heading"),
            

            # File selection section for radiosonde
            html.Div([
                html.Div([ html.P(['Radiosonde filename'],style = {"margin":"0px"})
                ], className = "inputfield-header"),

                html.Div([
                dcc.Dropdown(options = list_of_files_radiosonde, 
                            value  = list_of_files_radiosonde[0],
                            id='file-dropdown-radiosonde',multi = True),
                ]),

            ]),

            # Graph axis value selection section for radiosonde
            html.Div([
                # X axis value selection for radiosonde
                html.Div([

                    html.Div([ html.P(['x- axis : Radiosonde sensor'],style = {"margin":"0px"})
                    ], className = "inputfield-header"),

                    html.Div([
                        dcc.Dropdown(
                            radiosonde_parameter_value_list,
                            'Wd',
                            id='xaxis-column-radiosonde'
                        ),
                    ]),

                ]),

                # Y axis value selection for radiosonde
                html.Div([

                    html.Div([ html.P(['y- axis : Radiosonde sensor'],style = {"margin":"0px"})
                    ], className = "inputfield-header"),

                    html.Div([
                        dcc.Dropdown(
                            radiosonde_parameter_value_list,
                            'Ws',
                            id='yaxis-column-radiosonde'
                        )
                    ])
           
                ])
            ]),

            # Parameter range selection section for hexa sensor
            html.Div([

                html.Div([ html.P(['x filter value'],style = {"margin":"0px"})
                ], className = "inputfield-header"),

                html.Div([
                    html.Div([
                        dcc.Dropdown(
                            radiosonde_parameter_value_list,
                            'Wd',
                            id='x-filter-val-radiosonde',
                        ),
                    ],className = 'filter-param-name'),

                    html.Div([
                        dcc.Input(id='x-filter-input-radiosonde', 
                                type='number',
                                debounce=True),
                    ],className = 'filter-param-val'),
                ], style = {'display':'flex'})
                
            ]),
        ]),
        
        # Graph type selection section
        html.Div([
            html.Div([ html.H2(['Plot type'],style = {"margin":"0px"})
            ], className = "section-heading"),

            html.Div([
                    dcc.Dropdown(type_of_plots,
                                "Scatter", 
                                id='plot-type-dropdown'),
            ],style={"margin": "5px",'width': '48%', 'float': 'center', 'display': 'block'}),
    
        ])
    
    ], className = "side-menu"),

    # Graph section
    html.Div([
        dcc.Graph(id='indicator-graphic',className = "graph",),
    ], className = "graph-container")
    

], className = 'main-container')


@app.callback(
    Output('indicator-graphic', 'figure'),
    Input('file-dropdown', 'value'), 
    Input('file-dropdown-radiosonde', 'value'),   
    Input('xaxis-column-hexa-sensor', 'value'),
    Input('yaxis-column-hexa-sensor', 'value'),
    Input('xaxis-column-radiosonde', 'value'),
    Input('yaxis-column-radiosonde', 'value'),
    Input('x-filter-val-hexa', 'value'),
    Input('x-filter-input-hexa', 'value'),
    Input('x-filter-val-radiosonde', 'value'),
    Input('x-filter-input-radiosonde', 'value'),
    Input('plot-type-dropdown', 'value'))

def plot_update(filenames,filenames_radiosonde, 
                xaxis_column,yaxis_column, xaxis_column_radiosonde, yaxis_column_radiosonde,
                x_filter_val_hexa, x_filter_input_hexa, x_filter_val_radiosonde, x_filter_input_radiosonde, 
                plot_type):

    if type(filenames) is not list:
        filenames = [filenames]

    if type(filenames_radiosonde) is not list:
        filenames_radiosonde = [filenames_radiosonde]
        
    frames    = dict()
    frames_radiosonde = dict()

    #loading hexa sensor values from file
    for ind,file in enumerate(filenames):
        data_file_path = os.path.join(folder_name,file)
        try:
            df = pd.read_csv(data_file_path,error_bad_lines=False) 

            if (x_filter_input_hexa!=None):
                df = df[(df[x_filter_val_hexa]<x_filter_input_hexa) ]
            frames[file] = df[[xaxis_column,yaxis_column]].copy()
        except Exception as e: 
            print(e)

    # loading radiosonde sensor values from file
    for ind,file in enumerate(filenames_radiosonde):
        data_file_path = os.path.join("Radiosonde_Data",file)
        try:
            rows_to_skip = [i for i in range(0,18)] + [19]
            df = pd.read_csv(
                            data_file_path,error_bad_lines=False,\
                            skiprows = rows_to_skip, skipfooter=10,sep='\t', \
                            encoding = "ISO-8859-1", engine='python'
                            ) 

            # rename headers removing extra spaces
            df.columns = [x.strip() for x in df.columns]

            # filter out non numeric values
            df.loc[:, df.columns != 'UTC Time'] = df.loc[:, df.columns != 'UTC Time'].apply(pd.to_numeric, errors='coerce')
            df.dropna(inplace=True) 

            # remove values of quantity above given filter value
            if (x_filter_input_radiosonde!=None):
                df = df[(df[x_filter_val_radiosonde]<x_filter_input_radiosonde) ]

            frames_radiosonde[file] = df[[xaxis_column_radiosonde, yaxis_column_radiosonde]].copy()
        except Exception as e: 
            print(e)
        

    if plot_type == "Scatter":
        fig = go.Figure()
        for filename, df in frames.items():            
            fig.add_trace(go.Scattergl(x=df[xaxis_column], y=df[yaxis_column],
                            mode='markers',
                            marker=dict(
                            size=8,
                            opacity = 0.75,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                            name=parse_file_name([filename])[0]))

        for filename, df in frames_radiosonde.items():
            fig.add_trace(go.Scattergl(x=df[ xaxis_column_radiosonde], y=df[ yaxis_column_radiosonde],
                        mode='markers',
                        marker=dict(
                            size=8,
                            opacity = 0.75,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                        name=filename))

        fig.update_layout(
            title= yaxis_column + " Vs " + xaxis_column,
            title_x=0.5,
            xaxis_title= xaxis_column,
            yaxis_title= yaxis_column,
            font=dict(
                size=14
            ),
            legend=dict(
            orientation="h",
            yanchor="bottom",
            y=-0.5,
            xanchor="right",
            x=1
                )
        )
    elif plot_type == "Line":
        fig = go.Figure()
        for filename, df in frames.items():
            fig.add_trace(go.Scattergl(x=df[xaxis_column], y=df[yaxis_column],
                            mode='lines + markers',
                            marker=dict(
                            size=8,
                            opacity = 0.75,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                            name=parse_file_name([filename])[0]))

        for filename, df in frames_radiosonde.items():
            fig.add_trace(go.Scattergl(x=df[ xaxis_column_radiosonde], y=df[ yaxis_column_radiosonde],
                        mode='lines + markers',
                        marker=dict(
                            size=8,
                            opacity = 0.75,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                        name=filename))
       
        
        fig.update_layout(
            # height = 400,
            title= yaxis_column + " Vs  " + xaxis_column,
            title_x=0.5,
            xaxis_title= xaxis_column,
            yaxis_title= yaxis_column,
            font=dict(
                size=16
            ),
            legend=dict(
            orientation="h",
            yanchor="bottom",
            y=-0.5,
            xanchor="right",
            x=1
                )
        )
    elif plot_type == "Polar":
        fig = go.Figure()
        for filename, df in frames.items():
            fig.add_trace(go.Scatterpolargl(theta=df[xaxis_column], r=df[yaxis_column],
                            mode='markers',
                            marker=dict(
                            size=8,
                            opacity = 0.5,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                            name=parse_file_name([filename])[0]))

        for filename, df in frames_radiosonde.items():
            fig.add_trace(go.Scatterpolargl(theta=df[ xaxis_column_radiosonde], r=df[ yaxis_column_radiosonde],
                        mode='markers',
                        marker=dict(
                            size=8,
                            opacity = 0.5,
                            line=dict(
                                color='black',
                                width=0.5)
                            ),
                        name=filename))

        fig.update_layout(
            title= yaxis_column + " Vs  " + xaxis_column,
            title_x=0.5,
            font=dict(
                size=16
            ),
            legend=dict(
            orientation="h",
            yanchor="bottom",
            y=-0.2,
            xanchor="right",
            x=1
                )
        )
    
    else :
        fig ={}

    return fig

if __name__ == '__main__':
    app.run_server(debug=True)