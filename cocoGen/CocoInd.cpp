#include "CocoInd.h"

void CocoInd::getInfo(Info* infoInit) {
	info = infoInit;
}

void CocoInd::randomInit() {
	if (!info)
		return;
	fN = info->fN;
	cN = info->cN;
	for (uint i = 0; i < cN; i++) {
		built.push_back(rand() & 1);
	}
	fitnessOk = false;
}

pIndividual CocoInd::mutate() {
	CocoInd *ind = new CocoInd(*this);

	double mR = 1.0 * rand()/RAND_MAX*MUTATE_RATIO;
	for (uint i = 0; i < cN; i++) {
		double r = 1.0 * rand() / RAND_MAX;
		if (r*r < mR) {
			ind->built[i] = !ind->built[i];
		}
	}
	ind->fitnessOk = false;
	return make_shared<CocoInd>(*ind);
}

Pair CocoInd::cross(pIndividual ind) {
	CocoInd *ind1 = new CocoInd(*this);
	CocoInd *ind2 = new CocoInd(*(CocoInd*)ind.get());

	double cR = 1.0 * rand() / RAND_MAX*CROSS_RATIO;
	for (uint i = 0; i < cN; i++) {
		double r = 1.0 * rand() / RAND_MAX;
		if (r < cR) {
			swap(ind1->built[i], ind2->built[i]);
		}
	}
	ind1->fitnessOk = false;
	ind2->fitnessOk = false;

	Pair pair;
	pair.ind1 = make_shared<CocoInd>(*ind1);
	pair.ind2 = make_shared<CocoInd>(*ind2);
	return pair;
}

bool CocoInd::better(pIndividual ind, bool best) {
	double f1 = getFitness();
	double f2 = ind->getFitness();

	if (f1 > f2)
		return best;
	else
		return !best;

	/*if (f1 < 0) {
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
		return !best;*/
}

double CocoInd::getFitness() {
	if (fitnessOk)
		return fitness;

	bool last = false;
	fitness = 0;
	for (uint i = 0; i < built.size(); i++) {
		if (built[i] != last)
			fitness++;
		last = built[i];
	}
	fitnessOk = true;
	return fitness;
	/*if (fitnessOk)
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
	return fitness;*/
}

void CocoInd::print(ostream& out) {
	for (uint i = 0; i < built.size(); i++) {
		cout << built[i] ? "1" : "0";
	}
	cout << "\n";
}

void CocoInd::read(istream& in) {
}