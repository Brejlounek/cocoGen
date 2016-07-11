#include "CocoEvo.h"

void CocoEvo::init(string name) {

	ifstream file;
	file.open("data\\" + name);
	string strstr = "data\\" + name;
	info.name = name;

	file >> info.fN >> info.cN;
	uint cN = info.cN;
	uint fN = info.fN;
	for (int i = 0; i<info.fN; i++) {
		int cost_, cap_; double x, y;
		file >> cost_ >> cap_ >> x >> y;
		info.fX.push_back(x);
		info.fY.push_back(y);
		info.cost.push_back(cost_);
		info.cap.push_back(cap_);
	}
	for (int i = 0; i<info.cN; i++) {
		int demand_; double x, y;
		file >> demand_ >> x >> y;
		info.demand.push_back(demand_);
		info.cX.push_back(x);
		info.cY.push_back(y);
	}
	file.close();

	double cusPerFac = 1.0*cN/fN;
	for (uint ci = 0; ci<cN; ci++) {
		vector<Fac> facNow;
		for (uint fi = 0; fi<fN; fi++) {
			double dx = info.cX[ci] - info.fX[fi];
			double dy = info.cY[ci] - info.fY[fi];
			double d = sqrt(dx*dx + dy*dy);

			Fac fac;
			fac.fi = fi;
			fac.score = d + 1.0*info.cost[fi]/cusPerFac;
			facNow.push_back(fac);
		}
		sort(facNow.begin(), facNow.end(), facSort());
		vector<ushort> facNowId;
		for (uint fi = 0; fi<fN; fi++) {
			facNowId.push_back(facNow[fi].fi);
		}
		info.bestFacPerCus.push_back(facNowId);
	}

	/*for (uint ci = 0; ci<cN; ci++) {
		vector<double> cDist;
		for (uint fi = 0; fi<fN; fi++) {
			double dx = info.cX[ci] - info.fX[fi];
			double dy = info.cY[ci] - info.fY[fi];
			double d = sqrt(dx*dx + dy*dy);
			cDist.push_back(d);
		}
		info.d.push_back(cDist);
	}*/

	for (uint i = 0; i < POPSIZE; i++) {
		CocoInd ind;
		ind.getInfo(&info);
		ind.randomInit();

		pIndividual pInd = make_shared<CocoInd>(ind);
		Evolution::individuals.push_back(pInd);
	}
}