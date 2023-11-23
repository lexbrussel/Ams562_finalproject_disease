#pragma once

class state
{
protected:


	// 4 states for an individual
	enum State { Susceptible, Infected, Recovered, Vaccinated, Died };

	//count for output
	static size_t Susceptible_count;
	static size_t Infected_count;
	static size_t Recovered_count;
	static size_t Vaccinated_count;
	static size_t Died_count;
};

size_t state::Susceptible_count = 0;
size_t state::Infected_count = 0;
size_t state::Recovered_count = 0;
size_t state::Vaccinated_count = 0;
size_t state::Died_count = 0;