#ifndef EVOLUTION_LKZI54E376UOUZF7656986
#define EVOLUTION_LKZI54E376UOUZF7656986

#include "Individual.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
using namespace std;

#define MUTATE_RATE 0.5
#define CROSS_RATE 0.3
#define RANDOM_RATIO 0.3

struct EvolveSettings {
	bool evolveLimit, steady, debug, writeOut;
	uint evals, debugStep, checkpointStep, threadGens;
	EvolveSettings() {
		evolveLimit = false;
		steady = false;
		debug = true;
		writeOut = true;
		debugStep = 10000*1000;
		checkpointStep = 10 * debugStep;
		evals = UINT_MAX;
		threadGens = 10000*1000;
	}
};

class Evolution {
public:
	uint select(uint ti = 0, bool best = true);
	void evolve(EvolveSettings setup);
	void evolveThread(uint ti, uint evals);
	Pair modify(uint ti = 0);
	virtual void checkpoint(uint step) {};

	uint getWorst();
	uint getBest();
	uint getBest(uint ti);
	double getWorstFit();
	double getBestFit();
protected:
	vector< pIndividual > individuals;
	vector< pIndividual > threadIndividuals[THREADS];
	mutex mtx;
};

#endif