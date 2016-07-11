#include "Individual.h"

void Individual::randomInit(Info* infoInit) {
	info = infoInit;
	fN = info->fN;
	cN = info->cN;
	for (uint i = 0; i < cN; i++) {
		facilityOfCustomer.push_back(rand() % fN);
	}
	fitnessOk = false;
}

void Individual::mutate() {
	vector<uint> mutatePool;
	vector<bool> built;
	built.insert(built.begin(), fN, false);
	for (uint i = 0; i < cN; i++) {
		uint fi = facilityOfCustomer[i];
		if (!built[fi])
			mutatePool.push_back(fi);
		built[fi] = true;
	}

	double mR = rand()/RAND_MAX*MUTATE_RATIO;
	for (uint i = 0; i < cN; i++) {
		double r = rand() / RAND_MAX;
		if (r < mR) {
			facilityOfCustomer[i] = mutatePool[rand()%mutatePool.size()];//rand() % fN;
		}
	}
	fitnessOk = false;
}

void Individual::cross(Individual &ind) {
	double cR = rand() / RAND_MAX*CROSS_RATIO;
	for (uint i = 0; i < cN; i++) {
		double r = rand() / RAND_MAX;
		if (r < cR) {
			swap(facilityOfCustomer[i], ind.facilityOfCustomer[i]);
		}
	}
	fitnessOk = false;
}

bool Individual::better(Individual &ind, bool best) {
	double f1 = getFitness();
	double f2 = ind.getFitness();

	if (f1 < 0) {
		if (f2 > 0)
			return !best;
		if (f1 < f2)
			return !best;
		else
			return best;
	}
	if (f2 < 0)
		return best;
	if (f1 < f2)
		return best;
	else
		return !best;
}

double Individual::getFitness() {
	if (fitnessOk)
		return fitness;

	bool feasible = true;
	vector<bool> built;
	vector<uint> demandPerFacility;
	built.insert(built.begin(), fN, false);
	demandPerFacility.insert(demandPerFacility.begin(), fN, 0);
	for (uint i = 0; i < cN; i++) {
		uint fi = facilityOfCustomer[i];
		built[fi] = true;
		demandPerFacility[fi] += info->demand[i];
		if (demandPerFacility[fi]>info->cap[fi]) {
			feasible = false;
		}
	}

	fitness = 0;
	if (!feasible) {
		double diff = 0;
		for (uint fi = 0; fi < fN; fi++) {
			if (demandPerFacility[fi] > info->cap[fi])
				diff += demandPerFacility[fi] - info->cap[fi];
		}
		fitness = -diff;
	}
	else {
		for (uint fi = 0; fi < fN; fi++) {
			if (built[fi])
				fitness += info->cost[fi];
		}
		for (uint ci = 0; ci < cN; ci++) {
			uint fi = facilityOfCustomer[ci];
			double dx = info->cX[ci] - info->fX[fi];
			double dy = info->cY[ci] - info->fY[fi];
			double d = sqrt(dx*dx + dy*dy);
			fitness += d;
		}
	}
	fitnessOk = true;
	return fitness;
}