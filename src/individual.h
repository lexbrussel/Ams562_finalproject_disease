#pragma once
#include <vector>
#include <random>
#include <set>
class Individual {
public:
    // 4 states for an individual
    enum State { Susceptible, Infected, Recovered, Vaccinated };

    //initilize the social level for an individual
    Individual(int index) : state(Susceptible), index(index)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 9);

        socialActivityLevel = dis(gen);
    }

    //sick person will infect others with probability lamada(i) at given time i
    //also the number of person it infect depend on its socialActivityLevel
    void infection(double infectionRate);

    //sick person will recover with probability gamma(i) at gievn time i
    //recovered people will be susceptible after infected once
    void recover(double recoveryRate);

    //Susceptible and Recovered person may vaccine
    void vaccinate();

    State getState() const;

    int getSocialActivityLevel() const;

    int getindex() const;

    //stimulation the person that contact with each other
    const std::set<int> & getContacts() const;

    void clearContacts();

    void insert_idx_contacts(int idx);
private:
    State state;
    int index;
    int socialActivityLevel;
    std::set<int> uniqueContacts;
};



void Individual::infection(double infectionRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (state == Susceptible && dis(gen) < infectionRate)
    {
        state = Infected;
    }
    else if (state == Recovered && dis(gen) < infectionRate)
    {
        state = Susceptible;
    }
}

void Individual::recover(double recoveryRate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    if (state == Infected && dis(gen) < recoveryRate)
    {
        state = Recovered;
    }
}

void Individual::vaccinate()
{
    if (state != Infected)
    {
        state = Vaccinated;
    }
}

int Individual::getSocialActivityLevel() const 
{
    return socialActivityLevel;
}

Individual::State Individual:: getState() const
{
    return Individual::state;
}

int Individual::getindex() const
{
    return index;
}

const std::set<int>& Individual::getContacts() const
{
    return Individual::uniqueContacts;
}

void Individual::clearContacts()
{
    Individual::uniqueContacts.clear();
}

void Individual:: insert_idx_contacts(int idx)
{
    Individual::uniqueContacts.insert(idx);
}