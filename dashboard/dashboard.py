import panel as pn
import numpy as np
from bokeh.plotting import figure
from bokeh.models import ColumnDataSource
import param

# Configure Panel
pn.extension()

try:
    from model import Model
    print("Successfully imported Model")
except ImportError as e:
    print(f"Error importing Model: {e}")
    raise

def transform_coords(pos, bounds):
    """Transform simulation coordinates to display coordinates"""
    x = float(pos[0]) / bounds * 10
    y = float(pos[1]) / bounds * 10
    return x, y

class PredatorPreyDashboard(param.Parameterized):
    simulation_speed = param.Number(default=1.0/60.0, bounds=(0.001, 0.1))
    predator_speed = param.Number(default=1.0, bounds=(0.1, 10.0))
    prey_speed = param.Number(default=0.7, bounds=(0.1, 5.0))
    running = param.Boolean(default=True)
    
    def __init__(self, **params):
        super().__init__(**params)
        
        # Initialize model and get bounds
        self.model = Model("config.json")
        self.bounds = float(self.model.configuration.get("region_limit", 100.0))
        print(f"Model initialized with bounds: {self.bounds}")
        
        # Create plot first
        self.plot = figure(
            width=600,
            height=600,
            title="Predator vs Prey Simulation",
            match_aspect=True,
            tools="pan,wheel_zoom,box_zoom,reset,save"
        )
        
        # Set ranges explicitly
        self.plot.x_range.start = -10
        self.plot.x_range.end = 10
        self.plot.y_range.start = -10
        self.plot.y_range.end = 10
        
        # Add grid
        self.plot.grid.grid_line_color = 'gray'
        self.plot.grid.grid_line_alpha = 0.3
        
        # Get initial positions and transform them
        pred_pos = self.model.predator.position
        x, y = transform_coords(pred_pos, self.bounds)
        print(f"Initial predator position: raw=({pred_pos[0]}, {pred_pos[1]}), transformed=({x}, {y})")
        
        # Create data sources with transformed coordinates
        self.predator_source = ColumnDataSource({
            'x': [x],
            'y': [y]
        })
        
        prey_x = []
        prey_y = []
        for prey in self.model.preys:
            x, y = transform_coords(prey.position, self.bounds)
            prey_x.append(x)
            prey_y.append(y)
            print(f"Prey at: transformed=({x}, {y})")
        
        self.prey_source = ColumnDataSource({
            'x': prey_x,
            'y': prey_y
        })
        
        # Add glyphs
        self.predator_glyph = self.plot.circle(
            x='x', y='y',
            source=self.predator_source,
            size=15,
            color='red',
            alpha=0.8,
            line_width=2,
            legend_label='Predator'
        )
        
        self.prey_glyph = self.plot.circle(
            x='x', y='y',
            source=self.prey_source,
            size=10,
            color='green',
            alpha=0.8,
            line_width=2,
            legend_label='Prey'
        )
        
        # Configure legend
        self.plot.legend.click_policy = "hide"
        self.plot.legend.location = "top_right"
        
        print("Plot created")
        
        # Set initial speeds after plot creation
        self.predator_speed = self.model.predator.speed_p
        self.prey_speed = self.model.preys[0].speed if self.model.preys else 0.7
        
        # Create callback
        self._cb = pn.state.add_periodic_callback(
            self.update,
            period=50,
            start=True
        )
        print("Callback created")
        
        # Add breeding zone visualization
        breeding_radius = self.bounds * 0.2  # 20% of region size
        breeding_radius_transformed = breeding_radius / self.bounds * 10
        
        # Add filled circle for breeding zone
        self.plot.circle(
            x=0, y=0,
            radius=breeding_radius_transformed,
            fill_color='lightgreen',
            fill_alpha=0.2,
            line_color='green',
            line_alpha=0.5,
            legend_label='Breeding Zone'
        )
        
        # Add dashed border for breeding zone
        self.plot.circle(
            x=0, y=0,
            radius=breeding_radius_transformed,
            fill_alpha=0,
            line_color='green',
            line_alpha=0.8,
            line_dash='dashed',
            line_width=2
        )
        
        # Add text label
        self.plot.text(
            x=[0], y=[breeding_radius_transformed + 1],
            text=['Breeding Zone'],
            text_align='center',
            text_baseline='bottom'
        )

    def update(self):
        if not self.running:
            return
            
        try:
            # Update speeds from sliders
            self.model.predator.speed_p = self.predator_speed
            for prey in self.model.preys:
                prey.speed = self.prey_speed
            
            # Update model
            self.model.update(self.simulation_speed)
            print("Model updated")  # Debug print
            
            # Update predator with transformed coordinates
            pred_pos = self.model.predator.position
            x, y = transform_coords(pred_pos, self.bounds)
            print(f"Predator at: ({x}, {y})")  # Debug print
            
            self.predator_source.data.update({
                'x': [x],
                'y': [y]
            })
            
            # Update prey with transformed coordinates
            prey_x = []
            prey_y = []
            for prey in self.model.preys:
                x, y = transform_coords(prey.position, self.bounds)
                prey_x.append(x)
                prey_y.append(y)
            
            self.prey_source.data.update({
                'x': prey_x,
                'y': prey_y
            })
            print(f"Updated {len(prey_x)} prey positions")  # Debug print
            
        except Exception as e:
            print(f"Error in update: {e}")
            import traceback
            traceback.print_exc()

    def view(self):
        # Create widgets
        speed_slider = pn.widgets.FloatSlider(
            name='Simulation Speed',
            value=self.simulation_speed,
            start=0.001,
            end=0.1,
            step=0.001
        )
        
        predator_speed_slider = pn.widgets.FloatSlider(
            name='Predator Speed',
            value=self.predator_speed,
            start=0.1,
            end=10.0,
            step=0.1
        )
        
        prey_speed_slider = pn.widgets.FloatSlider(
            name='Prey Speed',
            value=self.prey_speed,
            start=0.1,
            end=5.0,
            step=0.1
        )
        
        play_button = pn.widgets.Toggle(
            name='Run/Pause',
            value=True,
            button_type='primary'
        )
        
        # Create bindings
        speed_slider.param.watch(lambda event: setattr(self, 'simulation_speed', event.new), 'value')
        predator_speed_slider.param.watch(lambda event: setattr(self, 'predator_speed', event.new), 'value')
        prey_speed_slider.param.watch(lambda event: setattr(self, 'prey_speed', event.new), 'value')
        play_button.param.watch(lambda event: setattr(self, 'running', event.new), 'value')
        
        # Create sidebar with controls
        controls = pn.Column(
            pn.pane.Markdown("## Controls"),
            play_button,
            pn.Spacer(height=20),
            speed_slider,
            pn.Spacer(height=10),
            predator_speed_slider,
            pn.Spacer(height=10),
            prey_speed_slider,
            background='white',
            width=300
        )
        
        # Create main view with plot
        main = pn.Column(
            pn.pane.Markdown("# Predator vs Prey Simulation"),
            pn.pane.Bokeh(self.plot),
            background='white'
        )
        
        # Create layout with sidebar and main view
        return pn.Row(
            controls,
            pn.Spacer(width=20),
            main,
            styles={'background': 'white'}
        )

# Create and serve the dashboard
dashboard = PredatorPreyDashboard()
template = pn.template.FastListTemplate(
    title="Predator vs Prey Simulation",
    sidebar=[dashboard.view().objects[0]],  # Put controls in sidebar
    main=[dashboard.view().objects[2]]      # Put main view in main area
)
template.servable()
