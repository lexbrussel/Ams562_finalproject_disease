#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include "individual.h"

class Population : state
{
public:

    //initiallize the virus for all individuals
    Population(const size_t size, const double infectionRate, const double recoveryRate, const double vaccinationRate, const double initialInfectionRate, const double dieRate)
        : recoveryRate(recoveryRate), infectionRate(infectionRate), vaccinationRate(vaccinationRate),dieRate(dieRate), size(size)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        state::Susceptible_count = size;
        state::Infected_count = 0;
        state::Recovered_count = 0;
        state::Vaccinated_count = 0;

        for (int i = 0; i < size; ++i) {
            Individual newIndividual(i);
            if (dis(gen) < initialInfectionRate) 
            {
                newIndividual.ini_infection(); // Initial infection

            }
            individuals.push_back(newIndividual);
        }
    }


    //update from today to next day
    void updatePopulationState();

    //print the result
    //currently too slow
    void OutPutPopulationStates();

    //check if the virus clear
    bool is_clear() const;
private:

    void die();

    //loop through all individuals to vaccinate
    void vaccination();


    //infect depend on social level for all individuals
    void contagion();
    void _contagion(Individual& individual, size_t idx);


    void ofstream_to_m(size_t susceptible, size_t infected, size_t recovered, size_t vaccinated, size_t died, size_t mutation) const;

    void clean_state();

    std::vector<Individual> individuals;

    const double infectionRate;

    const double recoveryRate;

    const double vaccinationRate;

    const double dieRate;

    bool disease_clear = false;

    int max_mutation = 0;

    const size_t size = 0;
};





void Population::updatePopulationState()
{
    die();
    contagion();
    vaccination();
    clean_state();
}

void Population::die()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (Individual& individual : individuals)
    {
        if (dis(gen) < dieRate && individual.getState() == state::Infected)
        {
            individual.die();
            individuals.back().changeIndex(individual.getIndex());
            individuals[individual.getIndex()] = individuals.back();
            individuals.pop_back();
        }
    }
}

void Population::vaccination()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (Individual& individual : individuals)
    {
        if (dis(gen) < vaccinationRate)
        {
            individual.vaccinate();
        }
    }

}

void  Population::clean_state()
{
    for (Individual& individual : individuals)
    {
        individual.clearContacts();
    }
}

void  Population::OutPutPopulationStates()
 {
    assert(state::Susceptible_count + state::Infected_count + state::Recovered_count + state::Vaccinated_count + state::Died_count == size);

    if (Infected_count == 0) { disease_clear = true; }
    std::cout << "Susceptible: " << state::Susceptible_count
        << ", Infected: " << state::Infected_count
        << ", Recovered: " << state::Recovered_count
        << ", Vaccinated: " << state::Vaccinated_count
        << ", Died: " << state::Died_count
        << ", Virus Mutate Time: " << max_mutation << std::endl;

    ofstream_to_m(state::Susceptible_count, state::Infected_count, state::Recovered_count, state::Vaccinated_count, state::Died_count, max_mutation);
}

void Population::contagion()
{
    std::vector<int> infect_individuals_idx;
    for (int i = 0; i < individuals.size(); i++)
    {
        if (individuals[i].getState() == state::Infected)
        {
            assert(individuals[i].getIndex() == i);
            infect_individuals_idx.push_back(i);
        }
        individuals[i].recover(recoveryRate);
    }
    for (int i = 0; i < infect_individuals_idx.size(); i++)
    {
        _contagion(individuals[infect_individuals_idx[i]], infect_individuals_idx[i]);
    }

}
void  Population::_contagion(Individual& individual, size_t idx)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> individualDist(0, individuals.size() - 1);


    //get unique set of person's index in vector that contact with infected person.
    while (individual.getContacts().size() < individual.getSocialActivityLevel())
    {
        size_t randomIndex = individualDist(gen);
        individual.insert_idx_contacts(randomIndex);
        individuals[randomIndex].insert_idx_contacts(idx);
    }

    for (const auto &idx : individual.getContacts())
    {
        individuals[idx].infection(infectionRate, individual);
        max_mutation = std::max(max_mutation, individuals[idx].getMutation_count() / individuals[idx].getMutation_time());
    }
}

bool Population::is_clear() const
{
    return disease_clear;
}


void Population::ofstream_to_m(size_t susceptible, size_t infected, size_t recovered, size_t vaccinated, size_t died, size_t mutation) const
{
    std::ofstream outputFile("result.m", std::ios::app);
    if (outputFile.is_open()) 
    {
        outputFile << susceptible << " " << infected << " " << recovered << " " << vaccinated << " " << died << " " << mutation << std::endl;
        outputFile.close();
    }
    else 
    {
        std::cerr << "Unable to open file for output!" << std::endl;
    }
}