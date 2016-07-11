#ifndef FACCOCOIND_KJTZR7RUIHJRE5Z4UKFDUTE87I
#define FACCOCOIND_KJTZR7RUIHJRE5Z4UKFDUTE87IT

#include "Individual.h"

#define MUTATE_RATIO 4.0/46216
#define CROSS_RATIO 1

class FacCocoInd : public Individual {
public:
	void getInfo(vector<bool> &builtInit, Info* infoInit);
	void randomInit();
	double getFitness();
	bool better(pIndividual ind, bool best = true);
	pIndividual mutate();
	Pair cross(pIndividual ind);
	void print(ostream& out);
	void print(string file);
	void read(istream& in);
private:
	double fitness;
	bool fitnessOk;

	Info* info;
	uint fN, cN, bpS;
	vector<uint> cus2fac, builtPool, over;
	vector<bool> built;
};

#endif