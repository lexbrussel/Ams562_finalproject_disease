#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include "individual.h"

class Population {
public:

    //initiallize the virus for all individuals
    Population(int size, double infectionRate, double recoveryRate, double vaccinationRate, double initialInfectionRate)
        : recoveryRate(recoveryRate), infectionRate(infectionRate), vaccinationRate(vaccinationRate) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int i = 0; i < size; ++i) {
            Individual newIndividual(i);
            if (dis(gen) < initialInfectionRate) {
                newIndividual.infection(1.0); // Initial infection
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

    //loop through all individuals to vaccinate
    void vaccination();


    //infect depend on social level for all individuals
    void contagion();
    void _contagion(Individual& individual, int idx);


    void ofstream_to_m(int susceptible, int infected, int recovered, int vaccinated) const;
    void clean_state();
    std::vector<Individual> individuals;
    double infectionRate;
    double recoveryRate;
    double vaccinationRate;
    bool disease_clear = false;
};





void Population::updatePopulationState()
{
    contagion();
    vaccination();
    clean_state();
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

void  Population::OutPutPopulationStates() {
    int susceptible = 0, infected = 0, recovered = 0, vaccinated = 0;
    for (const Individual& individual : individuals) {
        switch (individual.getState()) {
        case Individual::Susceptible: ++susceptible; break;
        case Individual::Infected: ++infected; break;
        case Individual::Recovered: ++recovered; break;
        case Individual::Vaccinated: ++vaccinated; break;
        }
    }
    if (infected == 0) { disease_clear = true; }
    std::cout << "Susceptible: " << susceptible
        << ", Infected: " << infected
        << ", Recovered: " << recovered
        << ", Vaccinated: " << vaccinated << std::endl;

    ofstream_to_m(susceptible, infected, recovered, vaccinated);
}

void Population::contagion()
{
    std::vector<int> infect_individuals_idx;
    for (int i = 0; i < individuals.size(); i++)
    {
        if (individuals[i].getState() == Individual::Infected)
        {
            assert(individuals[i].getindex() == i);
            infect_individuals_idx.push_back(i);
        }
        individuals[i].recover(recoveryRate);
    }
    for (int i = 0; i < infect_individuals_idx.size(); i++)
    {
        _contagion(individuals[infect_individuals_idx[i]], infect_individuals_idx[i]);
    }

}
void  Population::_contagion(Individual& individual, int idx)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> individualDist(0, individuals.size() - 1);


    //get unique set of person's index in vector that contact with infected person.
    while (individual.getContacts().size() < individual.getSocialActivityLevel())
    {
        int randomIndex = individualDist(gen);
        individual.insert_idx_contacts(randomIndex);
        individuals[randomIndex].insert_idx_contacts(idx);
    }

    for (const auto idx : individual.getContacts())
    {
        individuals[idx].infection(infectionRate);
    }
}

bool Population::is_clear() const
{
    return disease_clear;
}


void Population::ofstream_to_m(int susceptible, int infected, int recovered, int vaccinated) const
{
    std::ofstream outputFile("result.m", std::ios::app);
    if (outputFile.is_open()) 
    {
        outputFile << susceptible << " " << infected << " " << recovered << " " << vaccinated << std::endl;
        outputFile.close();
    }
    else 
    {
        std::cerr << "Unable to open file for output!" << std::endl;
    }
}