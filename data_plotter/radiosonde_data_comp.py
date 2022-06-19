import pandas as pd
import plotly.express as px  # (version 4.7.0 or higher)
import plotly.graph_objects as go
import matplotlib.pyplot as plt

radiosonde = pd.read_csv("radiosonde_18_06_22_5_33.csv")
hex_sense  = pd.read_csv("hex_data.csv",error_bad_lines=False,encoding= 'unicode_escape')
radiosonde.columns = ['Time', 'P', 'T', 'Hu', 'Ws','Wd','Geopot','Dewp']

wind_s_r = radiosonde[radiosonde['Geopot']<3800]['Ws']
h_r      = radiosonde[radiosonde['Geopot']<3800]['Geopot']



hex_filtered = hex_sense[(hex_sense['Wind speed']>0) & (hex_sense['Wind speed']<25) ]

wind_s_h_ascent = hex_filtered['Wind speed'][0:1900]/0.514444
h_h_ascent      = hex_filtered['pix_alt'][0:1900]*3.28084/1000

wind_s_h_descent = hex_filtered['Wind speed'][1900:]/0.514444
h_h_descent      = hex_filtered['pix_alt'][1900:]*3.28084/1000

# plt.plot(h_h_ascent,wind_s_h_ascent)
# plt.plot(h_h_descent,wind_s_h_descent)
# plt.plot(h_r,wind_s_r)
# plt.show()


fig = go.Figure()
fig.add_trace(go.Scatter(x=h_h_ascent, y=wind_s_h_ascent,
                    mode='lines',
                    name='Hexa ascent (raw)'))
fig.add_trace(go.Scatter(x=h_h_descent, y=wind_s_h_descent,
                    mode='lines',
                    name='Hexa descent (raw)'))
fig.add_trace(go.Scatter(x=h_r, y= wind_s_r,
                    mode='lines',
                    name='Radiosonde'))

fig.update_layout(
    title="Raw wind speed from Hexa Vs Radiosonde wind speed readings",
    xaxis_title="Altitude (ft)",
    yaxis_title="Wind speed (kn)",
    legend_title="Wind speed  ",
    font=dict(
        size=18
    )
)

fig.show()

fig.write_image("images/wind speed.png")

wind_a_r = radiosonde[radiosonde['Geopot']<3800]['Wd']
h_r      = radiosonde[radiosonde['Geopot']<3800]['Geopot']

hex_filtered = hex_sense[(hex_sense['Wind angle']>0) & (hex_sense['Wind angle']<360) ]

wind_a_h_descent = hex_filtered['Wind angle'][:1900]
h_h_descent      = hex_filtered['pix_alt'][:1900]*3.28084/1000

fig = go.Figure()
fig.add_trace(go.Scatter(x=h_h_descent, y=wind_a_h_descent,
                            mode='lines',
                            name='Hexa ascent'))

fig.add_trace(go.Scatter(x=h_r, y=wind_a_r ,
                            mode='lines',
                            name='Radiosonde'))
fig.update_layout(
    title="Wind direction w.r.t magnetic north",
    xaxis_title="Altitude (ft)",
    yaxis_title="Wind direction (deg)",
    legend_title="Wind direction",
    font=dict(
        size=18
    )
)

fig.show()
fig.write_image("images/wind direction.png")

temp_r = radiosonde[radiosonde['Geopot']<3800]['T']
h_r    = radiosonde[radiosonde['Geopot']<3800]['Geopot']

hex_filtered = hex_sense[(hex_sense['Acoustic temperature windsensor']>20) & (hex_sense['Acoustic temperature windsensor']<40) ]


tempms5611_descent = hex_filtered['Temperature_ms5611'][:1900]
tempaht10_descent = hex_filtered['Temperature_aht10'][:1900]
tempacoutic_descent = hex_filtered['Acoustic temperature windsensor'][:1900]
h_h_descent      = hex_filtered['pix_alt'][:1900]*3.28084/1000

fig = go.Figure()
fig.add_trace(go.Scatter(x=h_h_descent, y=tempms5611_descent,
                    mode='lines',
                    name='MS5611 (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_h_descent, y=tempaht10_descent,
                    mode='lines',
                    name='AHT10 (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_h_descent, y=tempacoutic_descent,
                    mode='lines',
                    name='Wind Sensor (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_r, y= temp_r,
                    mode='lines',
                    name='Radiosonde'))

fig.update_layout(
    title="Temperature readings (hexa ascent)",
    xaxis_title="Altitude (ft)",
    yaxis_title="Temperature (°C)",
    legend_title="Temperature ",
    font=dict(
        size=18
    )
)

fig.show()

fig.write_image("images/temp_ascent.png")


tempms5611_descent = hex_filtered['Temperature_ms5611'][1900:]
tempaht10_descent = hex_filtered['Temperature_aht10'][1900:]
tempacoutic_descent = hex_filtered['Acoustic temperature windsensor'][1900:]
h_h_descent      = hex_filtered['pix_alt'][1900:]*3.28084/1000

fig = go.Figure()
fig.add_trace(go.Scatter(x=h_h_descent, y=tempms5611_descent,
                    mode='lines',
                    name='MS5611 (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_h_descent, y=tempaht10_descent,
                    mode='lines',
                    name='AHT10 (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_h_descent, y=tempacoutic_descent,
                    mode='lines',
                    name='Wind Sensor (hexa sensor)'))
fig.add_trace(go.Scatter(x=h_r, y= temp_r,
                    mode='lines',
                    name='Radiosonde'))

fig.update_layout(
    title="Temperature readings (hexa descent)",
    xaxis_title="Altitude (ft)",
    yaxis_title="Temperature  Reading (°C)",
    legend_title="Temperature ",
    font=dict(
        size=18
    )
)

fig.show()

fig.write_image("images/temp_descent.png")

Hu_r = radiosonde[radiosonde['Geopot']<3800]['Hu']
h_r    = radiosonde[radiosonde['Geopot']<3800]['Geopot']

# hex_filtered = hex_sense[(hex_sense['Acoustic temperature windsensor']>20) & (hex_sense['Acoustic temperature windsensor']<40) ]

humidity_ascent = hex_filtered['Relative humidity_aht10'][:1900]
h_h_ascent      = hex_filtered['pix_alt'][:1900]*3.28084/1000

fig = go.Figure()
fig.add_trace(go.Scatter(x=h_h_ascent, y=humidity_ascent,
                    mode='lines',
                    name='Hexa ascent'))
fig.add_trace(go.Scatter(x=h_r, y= Hu_r,
                    mode='lines',
                    name='Radiosonde'))

fig.update_layout(
    title="Relative Humidity readings",
    xaxis_title="Altitude (ft)",
    yaxis_title="Relative Humidity (%)",
    legend_title="Rel Humidity ",
    font=dict(
        size=18
    )
)

fig.show()

fig.write_image("images/Hu.png")



