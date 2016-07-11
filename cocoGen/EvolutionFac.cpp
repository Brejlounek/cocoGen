#include "Evolution.h"

void Evolution::init(string name) {

	ifstream file;
	file.open("data\\" + name);
	string strstr = "data\\" + name;

	file >> info.fN >> info.cN;
	for (int i = 0; i<info.fN; i++) {
		int cost_, cap_; double x, y;
		file >> cost_ >> cap_ >> x >> y;
		info.fX.push_back(x);
		info.fY.push_back(y);
		info.cost.push_back(cost_);
		info.cap.push_back(cap_);
	}
	for (int i = 0; i<info.cN; i++) {
		int demand_; double x, y;
		file >> demand_ >> x >> y;
		info.demand.push_back(demand_);
		info.cX.push_back(x);
		info.cY.push_back(y);
	}
	file.close();

	for (uint i = 0; i < POPSIZE; i++) {
		Individual ind;
		ind.randomInit(&info);
		individuals.push_back(ind);
	}
}

uint Evolution::select(bool best) {
	uint i1 = rand() % POPSIZE;
	uint i2 = rand() % POPSIZE;
	uint i3 = rand() % POPSIZE;
	while (i2 == i1)
		i2 = rand() % POPSIZE;
	while (i3 == i2 || i3 == i1)
		i3 = rand() % POPSIZE;

	if (individuals[i1].better(individuals[i2],best) 
		&& individuals[i1].better(individuals[i3],best))
		return i1;
	else if (individuals[i2].better(individuals[i3],best) 
		&& individuals[i2].better(individuals[i1],best))
		return i2;
	else
		return i3;
}

void Evolution::evolveSteady(uint evals) {
	uint evN = 0;
	while (evN < evals) {
		uint i1 = select();
		uint i2 = select();
		while (i2 == i1) {
			i2 = select();
		}

		Individual ind1 = individuals[i1];
		Individual ind2 = individuals[i2];
		double r = rand() / RAND_MAX;
		if (r < CROSS_RATE) {
			ind1.cross(ind2);
			r = rand() / RAND_MAX;
			if (r < MUTATE_RATE) {
				ind1.mutate();
				ind2.mutate();
			}
		}
		else {
			ind1.mutate();
			ind2.mutate();
		}

		uint worst;
		worst = getWorst();
		individuals[worst] = ind1;
		worst = getWorst();
		individuals[worst] = ind2;
		evN += 2;

		if (!(evN % 10000)) {
			cout << "ev " << evN << ": best " << getBestFit() << ", worst " << getWorstFit() << "\n";
		}
	}
}

void Evolution::evolveGen(uint evals) {
	uint evN = 0, gen = 0;
	while (evN < evals) {
		vector<Individual> newIndividuals;
		newIndividuals.push_back(individuals[getBest()]);
		while (newIndividuals.size() < POPSIZE) {
			uint i1 = select();
			uint i2 = select();
			while (i2 == i1) {
				i2 = select();
			}

			Individual ind1 = individuals[i1];
			Individual ind2 = individuals[i2];
			double r = rand() / RAND_MAX;
			if (r < CROSS_RATE) {
				ind1.cross(ind2);
				r = rand() / RAND_MAX;
				if (r < MUTATE_RATE) {
					ind1.mutate();
					ind2.mutate();
				}
			}
			else {
				ind1.mutate();
				ind2.mutate();
			}

			newIndividuals.push_back(ind1);
			newIndividuals.push_back(ind2);
			evN += 2;
		}
		individuals = newIndividuals;
		cout << "gen " << gen << ": best " << getBestFit() << ", worst " << getWorstFit() << "\n";
		gen++;
	}
}

uint Evolution::getWorst() {
	uint worst = 0;
	for (uint i = 0; i < POPSIZE; i++) {
		if (!individuals[i].better(individuals[worst])) {
			worst = i;
		}
	}
	return worst;
}
uint Evolution::getBest() {
	uint best = 0;
	for (uint i = 0; i < POPSIZE; i++) {
		if (individuals[i].better(individuals[best])) {
			best = i;
		}
	}
	return best;
}

double Evolution::getBestFit() {
	uint best = getBest();
	return individuals[best].getFitness();
}
double Evolution::getWorstFit() {
	uint worst = getWorst();
	return individuals[worst].getFitness();
}