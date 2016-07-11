#ifndef FACCOCOEVO_KLHI6465DFUZ85DTZH
#define FACCOCOEVO_KLHI6465DFUZ85DTZH

#include "Evolution.h"
#include "FacCocoInd.h"

/*#define POPSIZE 1000
#define TOURNSIZE 2

#define MUTATE_RATE 0.5
#define CROSS_RATE 0.7*/

#define pFacCocoInd shared_ptr<FacCocoInd>

class FacCocoEvo : public Evolution {
public:
	void init(vector<bool> &built, Info *info);
	void checkpoint(uint step);
	void read(string name);
private:
	Info *info;
	vector<bool> built;
	uint id;
};

#endif