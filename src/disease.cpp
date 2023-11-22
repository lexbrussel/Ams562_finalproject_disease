#include "population.h"
#include "individual.h"


int main() {
    int populationSize = 1000;
    double infectionRate = 0.3;
    double initialInfectionRate = 0.001;
    double recoveryRate = 0.5;
    double vaccinationRate = 0.01;

    Population myPopulation(populationSize, infectionRate, recoveryRate, vaccinationRate, initialInfectionRate);

    // Choose the vaccination strategy
    // Uncomment one of the following lines based on the strategy you want to test
    // myPopulation.vaccinateHighRisk(vaccinationRate);
    //myPopulation.vaccinateSociallyActive(vaccinationRate);

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
