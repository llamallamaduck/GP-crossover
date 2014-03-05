#include "../ECF_base.h"
#include "Binary.h"

namespace Binary
{

void BinaryCrsShuffle::registerParameters(StateP state)
{
	myGenotype_->registerParameter(state, "crx.shuffle", (voidP) new double(0), ECF::DOUBLE);
}


bool BinaryCrsShuffle::initialize(StateP state)
{
	voidP sptr = myGenotype_->getParameterValue(state, "crx.shuffle");
	probability_ = *((double*)sptr.get());
	return true;
}


bool BinaryCrsShuffle::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child)
{
	Binary* p1 = (Binary*) (gen1.get());
	Binary* p2 = (Binary*) (gen2.get());
	Binary* ch = (Binary*) (child.get());

	uint rand1, rand2;
	bool temp;
	typedef std::vector<uint> v_uint;

	v_uint pFrom;
	v_uint pTo;

	uint bitCrs = state_->getRandomizer()->getRandomInteger(p1->getNumBits());

	for (uint dimension = 0; dimension < p1->variables.size(); dimension++) {
		//shuffle parents
		for (uint i = 0; i < p1->getNumBits(); i++) {
			rand1 = state_->getRandomizer()->getRandomInteger(0, 1);
			rand2 = state_->getRandomizer()->getRandomInteger(0, 1);
			pFrom.push_back(rand1);
			pTo.push_back(rand2);

			temp = p1->variables[dimension][rand1];
			p1->variables[dimension][rand1] = p1->variables[dimension][rand2];
			p1->variables[dimension][rand2] = temp;

			temp = p2->variables[dimension][rand1];
			p2->variables[dimension][rand1] = p2->variables[dimension][rand2];
			p2->variables[dimension][rand2] = temp;
		}
		//shuffle done
		switch (state_->getRandomizer()->getRandomInteger(0, 1)) {
			case 0: for (uint i = 0; i < bitCrs; i++) {
						ch->variables[dimension][i] = p1->variables[dimension][i];
					}
					for (uint i = bitCrs; i < p2->getNumBits(); i++) {
						ch->variables[dimension][i] = p2->variables[dimension][i];
					}
					break;
			case 1: for (uint i = 0; i < bitCrs; i++) {
						ch->variables[dimension][i] = p2->variables[dimension][i];
					}
					for (uint i = bitCrs; i < p1->getNumBits(); i++) {
						ch->variables[dimension][i] = p1->variables[dimension][i];
					}
		}
		//unshuffle child
		for (uint i = 0; i < p1->getNumBits(); i++) {
			rand1 = pFrom[p1->getNumBits() - 1 - i];
			rand2 = pTo[p1->getNumBits() - 1 - i];
			temp = ch->variables[dimension][rand1];
			ch->variables[dimension][rand1] = ch->variables[dimension][rand2];
			ch->variables[dimension][rand2] = temp;
		}
	}

	// update integer and real domain representation
	ch->update();

	return true;
}

}