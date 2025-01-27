// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include "model.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;
constexpr float ENTITY_RADIUS = 5.0f;

// Helper function to convert simulation coordinates to window coordinates
sf::Vector2f toWindowCoords(const std::array<double, 2>& pos, double bounds) {
    float x = (pos[0] + bounds) * (WINDOW_WIDTH / (2 * bounds));
    float y = (pos[1] + bounds) * (WINDOW_HEIGHT / (2 * bounds));
    return {x, y};
}

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(WINDOW_WIDTH), 
                                         static_cast<unsigned int>(WINDOW_HEIGHT)}), 
                           "Predator vs Prey Simulation");
    window.setFramerateLimit(60);

    // Load configuration
    std::string config_file = "config.json";
    Model model(config_file);
    double bounds = model.configuration["region_limit"].get<double>();

    // Create shapes for visualization
    sf::CircleShape predatorShape(ENTITY_RADIUS);
    predatorShape.setFillColor(sf::Color::Red);
    predatorShape.setOrigin({ENTITY_RADIUS, ENTITY_RADIUS});

    sf::CircleShape preyShape(ENTITY_RADIUS);
    preyShape.setFillColor(sf::Color::Green);
    preyShape.setOrigin({ENTITY_RADIUS, ENTITY_RADIUS});

    // Simulation parameters
    double time_delta = 1.0 / 60.0;  // 60 FPS

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        if (auto event = window.pollEvent()) {
            // Check if the window was closed
            if (event->type == sf::Event::Type::Closed) {
                window.close();
                break;
            }
        }

        // Update simulation
        model.update(time_delta);

        // Clear window
        window.clear(sf::Color::Black);

        // Draw predator
        auto predatorPos = toWindowCoords(model.predator.position, bounds);
        predatorShape.setPosition(predatorPos);
        window.draw(predatorShape);

        // Draw prey
        for (const auto& prey : model.preys) {
            auto preyPos = toWindowCoords(prey.position, bounds);
            preyShape.setPosition(preyPos);
            window.draw(preyShape);
        }

        // Display frame
        window.display();
    }

    return 0;
}