#ifndef COCOEVO_KLHI6465DFUZ85DTZH
#define COCOEVO_KLHI6465DFUZ85DTZH

#include "Evolution.h"
#include "CocoInd.h"

#define POPSIZE 1000
#define TOURNSIZE 2

#define MUTATE_RATE 0.5
#define CROSS_RATE 0.7

#define pCocoInd shared_ptr<CocoInd>;

struct Fac {
	uint fi;
	double score;
};

struct facSort
{
	inline bool operator() (const Fac& t1, const Fac& t2)
	{
		return (t1.score < t2.score);
	}
};

class CocoEvo : public Evolution {
public:
	void init(string name);
//private:
	Info info;
};

#endif