#include "Individual.h"

void Individual::randomInit(Info* infoInit) {
	info = infoInit;
	for (uint i = 0; i < info->fN; i++) {
		b.push_back(rand() % 2);
	}
}