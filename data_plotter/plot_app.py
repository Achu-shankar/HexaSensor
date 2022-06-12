# from tkinter.ttk import Style
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
folder_name = config_content['data_folder_path']

GRAPH_INTERVAL = config_content['graph_update_interval']
NO_DATAPOINTS = 100
app = Dash(__name__)
# folder_name= "New_folder"
base_file_name = "_base.txt"
base_file_path = os.path.join(folder_name,base_file_name)

# ------------------------------------------------------------------------------
# App layout
app.layout = html.Div([

    html.H1("Data plotter", style={'text-align': 'center'}),
    
    html.Div(id='output_container', children=[]),
    html.Br(),
    html.Div([
        html.H3("Number of Data Points"),
        dcc.Dropdown(
            options=[
                {'label': '20', 'value': 40},
                {'label': '50', 'value': 100},
                {'label': '100', 'value': 200},
            ],
            value=100,
            id = "no_points_selector"),
            html.Div(id='dummy') ],
        style = {'width':"10%","position":"relative",'left':'5%'}),
    html.Br(),

    # html.Div([
    # dcc.Graph(id='wind-speed', figure={}),
    # dcc.Graph(id='wind_dir', figure={}),
    # ], style = {'width':"50%","position":"relative",'left':'25%'}),

    html.Div([        
    html.Div([dcc.Graph(id='wind-speed', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='wind_dir', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='wind_acoustic_temp', figure={})],style = {"width":"33%"})
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    # html.Div([        
    # html.Div([dcc.Graph(id='wind-speed', figure={})],style = {"width":"50%"}),
    # html.Div([dcc.Graph(id='wind_dir', figure={})],style = {"width":"50%"})
    # ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    dcc.Interval(id="wind-speed-update",
                interval=int(GRAPH_INTERVAL),
                n_intervals=0),
    html.Div([        
    html.Div([dcc.Graph(id='temperature_ms5611', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='abs_alt_ms5611', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='pressure_ms5611', figure={})],style = {"width":"33%"})
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    html.Div([        
    html.Div([dcc.Graph(id='temperature_aht10', figure={})],style = {"width":"50%"}),
    html.Div([dcc.Graph(id='rel_hum_aht10', figure={})],style = {"width":"50%"})
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    
   
])  


# ------------------------------------------------------------------------------
# # Connect the Plotly graphs with Dash Components
@app.callback(
    [Output("output_container", "children"), 
     Output("wind-speed", "figure"),
     Output("temperature_ms5611", "figure"),
     Output("abs_alt_ms5611", "figure"),
     Output("pressure_ms5611", "figure"),
     Output("temperature_aht10", "figure"),
     Output("rel_hum_aht10", "figure"), 
     Output("wind_dir", "figure"), 
     Output("wind_acoustic_temp", "figure")], 
    [Input("wind-speed-update", "n_intervals")]
)

#  Output("temperature_ms5611", "figure"), 
    #  Output("abs_alt_ms5611", "figure"),
    #  Output("pressure_ms5611", "figure")

def update_graph(interval):
   
    container = "Time passed: {}s".format(interval)

    f =  open(base_file_path,"r")
    data_file_name_indx =  int(f.readline())
    f.close()
    data_file_path = os.path.join(folder_name,str(data_file_name_indx)+'.csv')

    with open(data_file_path) as file:
        # print(NO_DATAPOINTS)
        last_lines = tailer.tail(file, NO_DATAPOINTS)
        # print(len(last_lines))
    try:
        df = pd.read_csv(io.StringIO('\n'.join(last_lines[1:])), header=None,error_bad_lines=False)
    except Exception as e: 
        print(e)
    df.rename(columns = {0:'x', 
                       1:'windspeed',
                       2:'wind_dir',
                       3:'wind_ac_temp',
                       4:'tempms5611',
                       5:'pressms5611',
                       6:'absaltms5611',
                       7:'temp_aht10',
                       8:'relhumaht10'}, 
                    inplace = True)
    # print(df.head(3))
    fig_wind           = px.line(df,x="x",y= "windspeed")
    fig_wind.update_layout(title_text='Wind speed ', title_x=0.5,title_y=1)

    fig_wind_acou_temp = px.line(df,x="x",y= "wind_ac_temp")
    fig_wind_acou_temp.update_layout(title_text='Wind Acoustic Temperature ', title_x=0.5,title_y=1)

    fig_wind_dir    = px.scatter_polar(df, r="windspeed", theta="wind_dir" ,color="windspeed")
    fig_wind_dir.update_layout(title_text='Wind direction ', title_x=0.5, title_y=1)

    fig_tempms5611  = px.line(df,x="x",y= "tempms5611", title='Temperature ms5611')
    fig_tempms5611.update_layout(title_text='Temperature ms5611', title_x=0.5)
    
    fig_absalt5611  = px.line(df,x="x",y= "absaltms5611", title='Absolute altitude ms5611')
    fig_absalt5611.update_layout(title_text='Absolute altitude ms5611 ', title_x=0.5)

    fig_pressms5611 = px.line(df,x="x",y= "pressms5611", title='Pressure ms5611')
    fig_pressms5611.update_layout(title_text='Pressure ms5611', title_x=0.5)


    fig_tempaht10   = px.line(df,x="x",y= "temp_aht10", title='Temperature aht10')
    fig_tempaht10.update_layout(title_text='Temperature aht10', title_x=0.5)

    fig_relhumaht10 = px.line(df,x="x",y= "relhumaht10", title='Relative Humidity aht10')
    fig_relhumaht10.update_layout(title_text='Relative Humidity aht10', title_x=0.5)

    fig_wind_dir.add_annotation(dict(font=dict(color='black',size=18    ),
                                        x=0.5,
                                        y=1.15,
                                        showarrow=False,
                                        text="N",
                                        textangle=0,
                                        xanchor='center',
                                        xref="paper",
                                        yref="paper"))

    fig_wind_dir.update_layout(coloraxis_colorbar=dict(
            title="Wind Speed",
            thicknessmode="pixels", thickness=20
        ))

    

    # return  container, fig_wind, fig_tempms5611, fig_absalt5611, fig_pressms5611
    return  container, fig_wind,fig_tempms5611, \
            fig_absalt5611, fig_pressms5611, \
            fig_tempaht10, fig_relhumaht10, fig_wind_dir, fig_wind_acou_temp


@app.callback(
    Output("dummy", "children"),
    [Input("no_points_selector", "value")]
)

def update_no_data_points(value):
    global NO_DATAPOINTS
    NO_DATAPOINTS = int(value)
    return None

if __name__ == '__main__':
    app.run_server(debug=True)
    


  
# @app.callback(
#     Output("temperature_ms5611", "figure"), [Input("wind-speed-update", "n_intervals")]
# )

# # # ------------------------------------------------------------------------------

# def update_tempah10_graph(interval):
   
#     with open('test.csv') as file:
#         last_lines = tailer.tail(file, 500)
#     # print(last_lines[1:])
#     df = pd.read_csv(io.StringIO('\n'.join(last_lines[1:])), header=None)
#     df.rename(columns = {0:'x', 
#                        3:'y'}, 
#             inplace = True)

#     fig = px.line(df,x="x",y="y", title='Temperature ms5611')

#     return  fig