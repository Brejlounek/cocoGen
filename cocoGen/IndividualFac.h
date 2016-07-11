#include <vector>
using namespace std;

#define uint unsigned int

#define MUTATE_RATE 0.05
#define MUTATE_RATIO 0.1
#define CROSS_RATE 0.7
#define CROSS_RATIO 1

struct Info {
	int cN, fN;
	vector<int> cost, cap, demand;
	vector<double> fX, fY, cX, cY;
};

class Individual {
public:
	void randomInit(Info* infoInit);
	double getFitness();
	bool better(Individual &ind, bool best = true);
	void mutate();
	void cross(Individual &ind);
private:
	double fitness;
	bool fitnessOk;

	Info* info;
	uint fN, cN;
	vector<uint> facilityOfCustomer;
};