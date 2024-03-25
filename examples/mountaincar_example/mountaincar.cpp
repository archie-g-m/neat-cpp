#include <iostream>
#include <cmath>

// Constants
const double GRAVITY = 0.0025;    // 9.81; Acceleration due to gravity (m/s^2)
const double MIN_POSITION = -1.2; // Minimum position of the car
const double MAX_POSITION = 0.6;  // Maximum position of the car
const double MAX_SPEED = 0.07;    // Maximum speed of the car
const double CAR_MASS = 0.1;      // Mass of the car
const double FORCE = 0.001;       // Force applied to the car

class MountainCar
{
private:
    double position; // Position of the car
    double velocity; // Velocity of the car

public:
    MountainCar()
    {
        // Initialize the car at a random position and zero velocity
        position = -0.5; // Initial position
        velocity = 0.0;  // Initial velocity
    }

    void applyForce(int action)
    {
        // Apply force to the car based on the action
        double force = (action == 0) ? -FORCE : (action == 1) ? 0.0
                                                              : FORCE;
        velocity += force + (std::cos(3 * position) * (-GRAVITY));
        velocity = std::min(std::max(velocity, -MAX_SPEED), MAX_SPEED); // Clamp velocity
        position += velocity;
        position = std::min(std::max(position, MIN_POSITION), MAX_POSITION); // Clamp position
        if (position == MIN_POSITION && velocity < 0)
        {
            velocity = 0; // If at the left end, stop the car.
        }
    }

    double getPosition() const
    {
        return position;
    }

    double getVelocity() const
    {
        return velocity;
    }

    bool reachedGoal() const
    {
        // Check if the car reached the goal position
        return position >= 0.5;
    }

    void printState() const
    {
        // Print the current state of the car
        std::cout << "Position: " << position << ", Velocity: " << velocity << std::endl;
    }
};
