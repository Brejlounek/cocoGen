#include <stdio.h>
#include <time.h>
#include "CocoEvo.h"
#include "FacCocoEvo.h"

int main(){
	cout << setprecision(0); cout.setf(ios::fixed);
	srand(time(0));
	CocoEvo evolution0;
	//evolution0.init("fl_7_35_0.txt");
	//evolution0.init("fl_12755_46216_0.txt");
	//evolution0.init("fl_13457_24773_0.txt");
	evolution0.init("fl_100_1.txt");
	//evolution0.evolveGen();

	FacCocoEvo evolution;
	vector<bool> built;
	built.insert(built.begin(), evolution0.info.fN, true);
	evolution.init(built, &evolution0.info);

	EvolveSettings setup;
	setup.writeOut = false;
	//evolution.read("pops/fl_981_3094_1.txt_1400_309051.pop");
	evolution.evolve(setup);

	return 0;
}