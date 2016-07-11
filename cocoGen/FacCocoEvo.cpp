#include "FacCocoEvo.h"

void FacCocoEvo::init(vector<bool> &built_, Info *info_) {
	minstd_rand0 gen(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<uint> dis(0, UINT_MAX);

	built = built_;
	info = info_;

	for (uint ti = 0; ti<THREADS; ti++) {
		for (uint i = 0; i < POPSIZE; i++) {
			FacCocoInd ind;
			ind.getInfo(built, info);
			ind.randomInit();

			pIndividual pInd = make_shared<FacCocoInd>(ind);
			Evolution::threadIndividuals[ti].push_back(pInd);
		}
	}

	id = dis(gen);
}

void FacCocoEvo::checkpoint(uint step) {
	FacCocoInd *best = (FacCocoInd*) individuals[getBest()].get();
	best->print("best_big.txt");

	uint sz = individuals.size();
	uint fN = info->fN;
	ostringstream vname;
	vname << "pops/" << info->name << "_" << id%10000 << "_" << best->getFitness() << ".pop";

	ofstream out;
	out.open(vname.str(),ios::binary);
	out.write((char*)&sz,sizeof(uint));
	for (uint i = 0; i < individuals.size(); i++) {
		individuals[i]->print(out);
	}
	out.close();
}

void FacCocoEvo::read(string name) {
	ifstream in;
	in.open(name, ios::binary);
	if (in.fail())
		cout << "error opening file " << name << endl;

	uint sz;
	in.read((char*) &sz, sizeof(uint));

	//cout << info->cN << endl;
	for (uint ti = 0; ti<THREADS; ti++) {
		threadIndividuals[ti].clear();
	}
	for (uint i = 0; i<sz; i++) {
		//cout << i << endl;
		FacCocoInd ind;
		ind.getInfo(built, info);
		ind.read(in);

		uint ti = i%THREADS;
		pIndividual pInd = make_shared<FacCocoInd>(ind);
		Evolution::threadIndividuals[ti].push_back(pInd);
	}
	for (uint ti = 0; ti<THREADS; ti++) {
		uint tsz = Evolution::threadIndividuals[ti].size();
		if (!(tsz<POPSIZE))
			continue;
		uint sz2 = POPSIZE - tsz;
		for (uint i = 0; i<sz2; i++) {
			FacCocoInd ind;
			Evolution::threadIndividuals[ti].push_back(Evolution::threadIndividuals[ti][i%tsz]);
		}
	}
	in.close();
}