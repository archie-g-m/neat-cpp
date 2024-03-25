#include "neat.h"

#include "mountaincar.cpp"

int argmax(const std::vector<float> &v)
{
    return static_cast<int>(std::distance(v.begin(), max_element(v.begin(), v.end())));
}

float eval_genomes(Genome_ptr g)
{
    int max_steps = 1000;
    MountainCar car = MountainCar(); // Reset car position and velocity for each episode
    float fitness = 0;
    std::vector<double> positions;

    for (int step = 0; step < max_steps; ++step)
    {
        // Execute the Genome
        std::vector<float> input;
        input.push_back(car.getPosition());
        input.push_back(car.getVelocity());
        std::vector<float> output = g->forward(input);

        // Choose an action: 0 (left), 1 (idle), or 2 (right)
        int action = argmax(output);
        car.applyForce(action);

        if (car.reachedGoal())
        {
            break;
        }
        
        fitness -= 1;
    }
    return fitness;
}

void run(const std::string &config_file)
{
    std::cout << "Running Experiment with config at: " << config_file << std::endl;

    // Load configuration.
    ConfigParser_ptr config = std::make_shared<ConfigParser>(config_file);

    std::cout << config->to_string() << std::endl;

    // Create the population, which is the top-level object for a NEAT run.
    Population p(config);

    // Run for up to 300 generations.
    Genome_ptr best_genome = p.run(eval_genomes, 300, 3);

    // Display the winning genome.
    std::cout << "\nBest genome:\n"
              << best_genome->to_string() << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 1)
    {
        throw std::runtime_error("You must provide a file name");
        return 1;
    }
    srand(time(NULL));
    std::string fname = argv[1];

    run(fname);
    return 0;
}