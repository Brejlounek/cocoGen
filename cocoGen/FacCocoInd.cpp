#include "FacCocoInd.h"

void FacCocoInd::getInfo(vector<bool> &builtInit, Info* infoInit) {
	info = infoInit;
	built = builtInit;
	for (uint i = 0; i < built.size(); i++) {
		if (built[i])
			builtPool.push_back(i);
	}
	fN = info->fN;
	cN = info->cN;
	bpS = builtPool.size();
}

void FacCocoInd::randomInit() {
	minstd_rand0 gen(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<uint> dis(0, UINT_MAX);

	if (!info)
		return;
	fN = info->fN;
	cN = info->cN;
	bpS = builtPool.size();
	for (uint i = 0; i < cN; i++) {
		//cus2fac.push_back(dis(gen)%fN);
		cus2fac.push_back(info->bestFacPerCus[i][0]);
	}
	fitnessOk = false;
}

pIndividual FacCocoInd::mutate() {
	minstd_rand0 gen(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<uint> dis(0, UINT_MAX);

	FacCocoInd ind = *this;

	double r = 1.0 * dis(gen) / UINT_MAX;
	if (r<0.2)  {
		uint fi = dis(gen)%fN; //delete one factory
		for (uint ci = 0; ci<cN; ci++) {
			if (cus2fac[fi]==fi) {
				r = 1.0 * dis(gen) / UINT_MAX; r = min(1.0, max(0.0, 1 - pow(r, 0.1)));
				uint i = 1.0*r*(fN-1);
				ind.cus2fac[ci] = info->bestFacPerCus[ci][i];
			}
		}
	}
	else {
		uint mutateNum = /*dis(gen)%3 + */1;
		for (uint i = 0; i<mutateNum; i++) {
			r = 1.0 * dis(gen) / UINT_MAX;

			if (r<0.05 || 1) {
				r = 1.0 * dis(gen) / UINT_MAX;
				if (r<0.5) {
					ind.cus2fac[dis(gen)%cN] = dis(gen)%fN;
				}
				else {

					if (over.size())
						ind.cus2fac[over[dis(gen)%over.size()]] = dis(gen)%fN;
					else
						ind.cus2fac[dis(gen)%cN] = builtPool[dis(gen) % bpS];
				}
			}
			else {
				r = 1.0 * dis(gen) / UINT_MAX;
				uint ci = dis(gen)%cN;
				if (r<0.5) {
					r = 1.0 * dis(gen) / UINT_MAX; r = min(1.0, max(0.0, 1 - pow(r, 0.1)));
					uint i = 1.0*r*(fN-1);
					ind.cus2fac[ci] = info->bestFacPerCus[ci][i];
				}
				else {

					if (over.size())
						ind.cus2fac[over[dis(gen)%over.size()]] = dis(gen)%fN;
					else {
						r = 1.0 * dis(gen) / UINT_MAX; r = min(1.0, max(0.0, 1 - pow(r, 0.1)));
						uint i = 1.0*r*(fN-1);
						ind.cus2fac[ci] = info->bestFacPerCus[ci][i];
						while (!built[ind.cus2fac[ci]]) {
							r = 1.0 * dis(gen) / UINT_MAX; r = min(1.0, max(0.0, 1 - pow(r, 0.1)));
							i = 1.0*r*(fN-1);
							ind.cus2fac[ci] = info->bestFacPerCus[ci][i];
						}

						ind.cus2fac[ci] = builtPool[dis(gen) % bpS];
					}
				}
			}
		}
	}

	/*double mR = 1.0 * dis(gen)/UINT_MAX*MUTATE_RATIO;
	double r = 1.0 * dis(gen) / UINT_MAX;
	if (r<0.99)
		mR = 0.5 * dis(gen) / UINT_MAX*MUTATE_RATIO;
	
	if (r<0.5) {
		bool go = false;
		for (uint i = 0; i < cN; i++) {
			double r = 1.0 * dis(gen) / UINT_MAX;
			if (r < mR*mR) {
				ind.cus2fac[i] = dis(gen)%fN;//builtPool[rand() % bpS];
				go = true;
			}
		}
		if (!go)
			ind.cus2fac[dis(gen)%cN] = dis(gen)%fN;//builtPool[rand() % bpS];
	}
	else {
		bool go = false;
		for (uint i = 0; i < cN; i++) {
			double r = 1.0 * dis(gen) / UINT_MAX;
			if (r < mR*mR) {
				ind.cus2fac[i] = builtPool[dis(gen) % bpS];
				go = true;
			}
		}
		if (!go)
			ind.cus2fac[dis(gen)%cN] = builtPool[dis(gen) % bpS];
	}*/

	ind.fitnessOk = false;
	return make_shared<FacCocoInd>(ind);
}

Pair FacCocoInd::cross(pIndividual ind) {
	minstd_rand0 gen(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<uint> dis(0, UINT_MAX);

	FacCocoInd ind1 = *this;
	FacCocoInd ind2 = *(FacCocoInd*)ind.get();

	double cR = 1.0 * dis(gen) / UINT_MAX*CROSS_RATIO;
	for (uint i = 0; i < cN; i++) {
		double r = 1.0 * dis(gen) / UINT_MAX;
		if (r < cR*cR) {
			swap(ind1.cus2fac[i], ind2.cus2fac[i]);
		}
	}
	ind1.fitnessOk = false;
	ind2.fitnessOk = false;

	Pair pair;
	pair.ind1 = make_shared<FacCocoInd>(ind1);
	pair.ind2 = make_shared<FacCocoInd>(ind2);
	return pair;
}

bool FacCocoInd::better(pIndividual ind, bool best) {
	double f1 = getFitness();
	double f2 = ind->getFitness();

	/*if (f1 > f2)
		return best;
	else
		return !best;*/

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

double FacCocoInd::getFitness() {
	/*if (fitnessOk)
		return fitness;

	bool last = false;
	fitness = 0;
	for (uint i = 0; i < built.size(); i++) {
		if (built[i] != last)
			fitness++;
		last = built[i];
	}
	fitnessOk = true;
	return fitness;*/
	if (fitnessOk)
		return fitness;

	bool feasible = true;
	vector<uint> demandPerFacility;
	vector<bool> realBuilt; builtPool.clear();
	realBuilt.insert(realBuilt.begin(), fN, false);
	demandPerFacility.insert(demandPerFacility.begin(), fN, 0);
	for (uint i = 0; i < cN; i++) {
		uint fi = cus2fac[i];
		if (!realBuilt[fi])
			builtPool.push_back(fi);
		realBuilt[fi] = true;

		demandPerFacility[fi] += info->demand[i];
		if (demandPerFacility[fi]>info->cap[fi]) {
			feasible = false;
		}
	}
	bpS = builtPool.size();
	built = realBuilt;

	fitness = 0;
	over.clear();
	if (!feasible) {
		double diff = 0;
		vector<uint> facOver;
		for (uint fi = 0; fi < fN; fi++) {
			if (demandPerFacility[fi] > info->cap[fi]) {
				diff += demandPerFacility[fi] - info->cap[fi];
				facOver.push_back(fi);
			}
		}

		if (facOver.size()) {
			for (uint ci = 0; ci < cN; ci++) {
				uint fi = cus2fac[ci];
				for (uint i = 0; i<facOver.size(); i++) {
					if (facOver[i] == fi) {
						over.push_back(ci);
						break;
					}
				}
			}
		}

		fitness = -diff;
	}
	else {
		for (uint fi = 0; fi < fN; fi++) {
			if (realBuilt[fi])
				fitness += info->cost[fi];
		}
		for (uint ci = 0; ci < cN; ci++) {
			uint fi = cus2fac[ci];
			double dx = info->cX[ci] - info->fX[fi];
			double dy = info->cY[ci] - info->fY[fi];
			double d = sqrt(dx*dx + dy*dy);
			fitness += d;
			//fitness += info->d[ci][fi];
		}
	}
	fitnessOk = true;
	return fitness;
}

void FacCocoInd::print(ostream& out) {
	/*for (uint i = 0; i < cus2fac.size(); i++) {
		out << cus2fac[i] << " ";
	}
	out << endl;*/
	vector<uint> cus2facMy = cus2fac;
	uint sz = cus2facMy.size();
	out.write((const char*) &sz, sizeof(cus2facMy[0]));
	out.write((const char*) &cus2facMy[0], sz*sizeof(cus2facMy[0]));
}

void FacCocoInd::read(istream& in) {
	cus2fac.clear();

	uint sz;
	in.read((char *) &sz, sizeof(uint)); //sz = cN;
	cus2fac.resize(sz);
	in.read((char*) &cus2fac[0], sizeof(uint)*sz);
}

void FacCocoInd::print(string name) {
	vector<uint> empty;
	vector< vector<uint> > fac2cus;
	fac2cus.insert(fac2cus.begin(), fN, empty);

	for (uint i = 0; i < info->cN; i++) {
		uint fi = cus2fac[i];
		fac2cus[fi].push_back(i);
	}

	ofstream file;
	file.precision(17);
	file.open(name);
	file << getFitness() << endl;
	for (uint fi = 0; fi < fN; fi++) {
		if (!fac2cus[fi].empty()) {
			file << fi << " ";
			for (uint i = 0; i < fac2cus[fi].size(); i++) {
				file << fac2cus[fi][i] << " ";
			}
			file << endl;
		}
	}
	file.close();
}