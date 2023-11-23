#include "population.h"
#include "individual.h"


int main() {
    size_t populationSize = 100000;
    const double infectionRate = 0.35;
    const double initialInfectionRate = 0.001;
    const double recoveryRate = 0.5;
    const double vaccinationRate = 0.005;
    const double dieRate = 0.0001;
    Population myPopulation(populationSize, infectionRate, recoveryRate, vaccinationRate, initialInfectionRate, dieRate);


    int timeSteps = 1000; // Number of time steps for the simulation
    for (int step = 0; step < timeSteps; ++step) {
        std::cout << "Time Step " << step << std::endl;
        myPopulation.OutPutPopulationStates();
        if (myPopulation.is_clear() == true) { break; }
        myPopulation.updatePopulationState();
        std::cout << std::endl;
    }

    return 0;
}
