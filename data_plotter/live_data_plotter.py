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


    html.Div([        
    html.Div([dcc.Graph(id='wind-speed', figure={})],style = {"width":"50%"}),
    html.Div([dcc.Graph(id='wind_dir', figure={})],style = {"width":"50%"})
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    dcc.Interval(id="wind-speed-update",
                interval=int(GRAPH_INTERVAL),
                n_intervals=0),
    html.Div([        
    html.Div([dcc.Graph(id='temperature', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='rel_hum_aht10', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='pressure_ms5611', figure={})],style = {"width":"33%"})
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

    html.Div([        
    html.Div([dcc.Graph(id='attitude', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='z', figure={})],style = {"width":"33%"}),
    html.Div([dcc.Graph(id='pix_xy', figure={})],style = {"width":"33%"}),
    ], style = {"display":"flex","justify-content": "center","flex-direction": "row"}),

])  


# ------------------------------------------------------------------------------
# # Connect the Plotly graphs with Dash Components
@app.callback(
    [
     Output("output_container", "children"), 
     Output("wind-speed", "figure"),
     Output("wind_dir", "figure"),
     Output("temperature", "figure"),
     Output("pressure_ms5611", "figure"),
     Output("rel_hum_aht10", "figure"),  
     Output("attitude", "figure"), 
     Output("z", "figure"), 
     Output("pix_xy", "figure")
     ], 
    [Input("wind-speed-update", "n_intervals")]
)

#  Output("temperature_ms5611", "figure"), 
    #  Output("abs_alt_ms5611", "figure"),
    #  Output("pressure_ms5611", "figure")

def update_graph(interval):
   
    container = "Time passed: {}s".format(interval*GRAPH_INTERVAL/1000)

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
                       8:'relhumaht10',
                       10:'roll',
                       11:'pitch',
                       12:'yaw',
                       17:'pix_x',
                       18:"pix_y",
                       19:'pix_z',
                       26:'pix_alt'
                    }, 
                    inplace = True)
    df["pix_z"] = -1*df["pix_z"]
    df["windspeed"] = 1.94384*df["windspeed"]
    # print(df.head(3))
    fig_wind           = px.line(df,x="x",y= "windspeed")
    fig_wind.update_layout(title_text='Wind speed ', title_x=0.5,title_y=1, xaxis_title="# data point",
        yaxis_title="Wind Speed(kn)",)

   
    fig_wind_dir    = px.scatter_polar(df, r="windspeed", theta="wind_dir" ,color="windspeed", size = "windspeed", render_mode="webgl")
    fig_wind_dir.update_layout(title_text='Wind direction ', title_x=0.5, title_y=1, )

    # fig_absalt5611  = px.line(df,x="x",y= "absaltms5611", title='Absolute altitude ms5611')
    # fig_absalt5611.update_layout(title_text='Absolute altitude ms5611 ', title_x=0.5)

    fig_pressms5611 = px.line(df,x="x",y= "pressms5611", title='Pressure ms5611')
    fig_pressms5611.update_layout(title_text='Pressure ms5611', title_x=0.5, xaxis_title="# data point",
        yaxis_title="Pressure (Pa)",)

    fig_relhumaht10 = px.line(df,x="x",y= "relhumaht10", title='Relative Humidity aht10')
    fig_relhumaht10.update_layout(title_text='Relative Humidity aht10', title_x=0.5, xaxis_title="# data point",
        yaxis_title="Relative humidity(%)",)


    fig_temp = go.Figure()
    fig_temp.add_trace(go.Scatter(x=df["x"], y=df["tempms5611"],
                        mode='lines',
                        name='MS5611'))
    fig_temp.add_trace(go.Scatter(x=df["x"], y=df["temp_aht10"],
                        mode='lines',
                        name='AHT10'))
    fig_temp.add_trace(go.Scatter(x=df["x"], y=df["wind_ac_temp"],
                        mode='lines',
                        name='Wind Sensor'))

    fig_temp.update_layout(
        title="Atmospheric Temperature ",
        title_x=0.5,
        xaxis_title="# data point",
        yaxis_title="Temperature  Reading (°C)",
        font=dict(
            size=12
        ),
        legend=dict(
        orientation="h",
        yanchor="bottom",
        y=1.02,
        xanchor="right",
        x=1
            )
    )


    fig_att = go.Figure()
    fig_att.add_trace(go.Scatter(x=df["x"], y=df["roll"],
                        mode='lines',
                        name='roll'))
    fig_att.add_trace(go.Scatter(x=df["x"], y=df["pitch"],
                        mode='lines',
                        name='pitch'))
    fig_att.add_trace(go.Scatter(x=df["x"], y=df["yaw"],
                        mode='lines',
                        name='yaw'))

    fig_att.update_layout(
        title="Hexa attitude",
        title_x=0.5,
        xaxis_title="# data point",
        yaxis_title="Rad",
        font=dict(
            size=12
        ),
        legend=dict(
        orientation="h",
        yanchor="bottom",
        y=1.02,
        xanchor="right",
        x=1
        )
    )

    fig_pixz = px.line(df,x="x",y= "pix_z", title='Local Altitude')
    fig_pixz.update_layout(title_text='Relative Altitude', title_x=0.5, xaxis_title="# data point",
        yaxis_title="Relative altitude(m)")

    # fig_pix_xy = px.line(df,x="pix_x",y= "pix_y", title='Hexa horizontal drift',render_mode='webgl',markers =True)
    # fig_pix_xy.update_layout(title_text='Hexa horizontal drift', title_x=0.5)
    
    fig_pix_xy = go.Figure()
    fig_pix_xy.add_trace(go.Scatter(x=df["pix_x"], y=df["pix_y"],
                        mode='lines + markers',marker=dict(size=6,
                              line=dict(width=1,color='DarkSlateGrey'))))

    fig_pix_xy.update_layout(title_text='Hexa horizontal drift', title_x=0.5, xaxis_title="local x pos (m)",
        yaxis_title="local y pos(m)")


    # fig_pix_xy = {}
    fig_wind_dir.add_annotation(dict(font=dict(color='black',size=18 ),
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

    

    return  container, fig_wind,fig_wind_dir, fig_temp, fig_pressms5611,fig_relhumaht10,fig_att, fig_pixz, fig_pix_xy
    # return  container, fig_wind,fig_tempms5611, \
    #         fig_absalt5611, fig_pressms5611, \
    #         fig_tempaht10, fig_relhumaht10,  fig_wind_dir, fig_wind_acou_temp #,\
    #         # fig_roll,fig_pitch,fig_yaw,fig_pixalt


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