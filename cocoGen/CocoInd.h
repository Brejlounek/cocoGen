#ifndef COCOIND_KJZIU6547FCVZT
#define COCOIND_KJZIU6547FCVZT

#include "Individual.h"

#define MUTATE_RATIO 0.2
#define CROSS_RATIO 1

class CocoInd : public Individual {
public:
	void getInfo(Info* infoInit);
	void randomInit();
	double getFitness();
	bool better(pIndividual ind, bool best = true);
	pIndividual mutate();
	Pair cross(pIndividual ind);
	void print(ostream& out);
	void read(istream& in);
private:
	double fitness;
	bool fitnessOk;

	Info* info;
	uint fN, cN;
	vector<bool> built;
};

#endif