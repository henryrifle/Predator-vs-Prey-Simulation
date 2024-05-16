"""Predator-Prey
"""
import json
import pathlib

import holoviews as hv
import pandas as pd
import panel as pn

from .model import Model


configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
config = json.load(open(configuration_file))
region_limit = config['region_limit']


def plot_model(data):
    return hv.Scatter(data, kdims=['x'], vdims=['y', 'c']).opts(color='c')


pipe = hv.streams.Pipe(data=pd.DataFrame({'x':[], 'y':[], 'c': []}))
dmap = hv.DynamicMap(plot_model, streams=[pipe]).opts(
    xlim=(-region_limit, region_limit),
    ylim=(-region_limit, region_limit),
    height=600,
    width=600
)


model_state = Model(str(configuration_file))

global_time = 0.0
global_time_delta = 0.25
periodic_callback = None
def run_model():
    if model_state.preys:
        global global_time
        global_time += global_time_delta
        model_state.update(global_time_delta)
        predator_state = pd.DataFrame([model_state.predator.position], columns=['x', 'y'])
        predator_state['c'] = 'red'
        prey_state = pd.DataFrame([prey.position for prey in model_state.preys], columns=['x', 'y'])
        prey_state['c'] = 'white'
        time_box.value = f'{global_time}'
        predator_box.value = str(1)
        prey_box.value = str(len(model_state.preys))
        pipe.send(pd.concat([predator_state, prey_state]))
    else:
        periodic_callback.stop()
        play_button.name = 'Play'

def play(event):
    global periodic_callback, state
    if periodic_callback is None or not periodic_callback.running:
        play_button.name = 'Stop'
        periodic_callback = pn.state.add_periodic_callback(run_model, period=1000//int(fps_input.value))
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()

def reset(event):
    global model_state, periodic_callback
    config["seed"] = seed_input.value
    config["region_limit"] = region_limits_input.value
    config["number_of_preys"] = number_of_preys_input.value
    config["predator_speed"] = predator_speed_slider.value
    config["prey_speed"] = prey_speed_slider.value
    with open(configuration_file, 'w') as f:
        json.dump(config, f, indent=4)

    if periodic_callback:
        periodic_callback.stop()
        periodic_callback = None
    model_state = Model(str(configuration_file))
    predator_state = pd.DataFrame([model_state.predator.position], columns=['x', 'y'])
    predator_state['c'] = 'red'
    prey_state = pd.DataFrame([prey.position for prey in model_state.preys], columns=['x', 'y'])
    prey_state['c'] = 'white'
    pipe.send(pd.concat([predator_state, prey_state]))

time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
predator_box = pn.widgets.TextInput(name='predators', disabled=True)
prey_box = pn.widgets.TextInput(name='preys', disabled=True)

seed_input = pn.widgets.IntInput(name='Random Seed', value=config['seed'])
region_limits_input = pn.widgets.FloatInput(name='Region Limit', value=config['region_limit'])
number_of_preys_input = pn.widgets.IntInput(name='Number of Prey', value=config['number_of_preys'])
predator_speed_slider = pn.widgets.FloatSlider(name='Predator Speed', start=1.0, end=20.0, value=config['predator_speed'])
prey_speed_slider = pn.widgets.FloatSlider(name='Prey Speed', start=1.0, end=20.0, value=config['prey_speed'])

reset(None)

fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

play_button = pn.widgets.Button(name='Play')
play_button.on_click(play)

reset_button = pn.widgets.Button(name='Reset')
reset_button.on_click(reset)

sim_data_card = pn.Card(
    time_box,
    predator_box,
    prey_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    seed_input,
    region_limits_input,
    predator_speed_slider,
    prey_speed_slider,
    fps_input,
    reset_button,
    play_button,
    collapsible=False,
    title='Simulation Controls'
)

description_pane = pn.pane.Markdown('''

### Inputs & Controls

* Random Seed - seed for random number generation
* Region Limit - the min/max X/Y for spawning/waypoint locations
* Number of Prey - number of Prey entities to spawn
* Predator Speed - how fast the predator moves
* Prey Speed - how fast the prey move
* FPS - frames per second, or how quickly we playback the simulation (use 4 FPS for realtime)

''')

pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="Predator-Prey",
    title="Predator-Prey",
    main=[
        'A predator-prey model',
        pn.Row(dmap, description_pane)
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
