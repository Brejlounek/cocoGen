#ifndef INDIVIDUAL_KJHT7R565EDCJHFZTZ
#define INDIVIDUAL_KJHT7R565EDCJHFZTZ

#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

#define uint unsigned int
#define ushort unsigned short

#define POPSIZE 100
#define TOURNSIZE 2
#define THREADS 4

#define pIndividual shared_ptr<Individual>

struct Info {
	int cN, fN;
	vector<int> cost, cap, demand;
	vector<double> fX, fY, cX, cY;
	vector<vector<ushort>> bestFacPerCus;
	vector<vector<double>> d;
	string name;
};

class Individual;
struct Pair {
	pIndividual ind1;
	pIndividual ind2;
};

class Individual {
public:
	virtual void randomInit() = 0;
	virtual double getFitness() = 0;
	virtual bool better(pIndividual ind, bool best = true) = 0;
	virtual pIndividual mutate() = 0;
	virtual Pair cross(pIndividual ind) = 0;
	virtual void print(ostream& out) = 0;
	virtual void read(istream& in) = 0;
protected:
};

#endif