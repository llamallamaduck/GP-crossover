#include "../ECF_base.h"
#include "Binary.h"

namespace Binary
{

void BinaryCrsSegmented::registerParameters(StateP state)
{
	myGenotype_->registerParameter(state, "crx.segmented", (voidP) new double(0), ECF::DOUBLE);
}


bool BinaryCrsSegmented::initialize(StateP state)
{
	voidP sptr = myGenotype_->getParameterValue(state, "crx.segmented");
	probability_ = *((double*)sptr.get());
	return true;
}


bool BinaryCrsSegmented::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child)
{
    Binary* p1 = (Binary*) (gen1.get());
	Binary* p2 = (Binary*) (gen2.get());
	Binary* ch = (Binary*) (child.get());
	
	double s=0.2;

    for (uint dimension = 0; dimension < p1->variables.size(); dimension++) {
		uint parent = state_->getRandomizer()->getRandomInteger(0, 1);
		for (uint i = 0; i < p1->getNumBits(); i++) {
			if (parent==0)	{
				double q = state_->getRandomizer()->getRandomDouble();
				uint j = state_->getRandomizer()->getRandomInteger(p1->getNumBits());
				if (q > s) {
					for (uint k = 0; k < j; k++) {
						ch->variables[dimension][k] = p1->variables[dimension][k];
					}
					for (uint k = j; k < p2->getNumBits(); k++) {
						ch->variables[dimension][k] = p2->variables[dimension][k];
					}
				}
				else
					ch->variables[dimension][i] = p1->variables[dimension][i];
			}
			else	{
				double q = state_->getRandomizer()->getRandomDouble();
				uint j = state_->getRandomizer()->getRandomInteger(p1->getNumBits());
				if (q > s) {
					for (uint k = 0; k < j; k++) {
						ch->variables[dimension][k] = p2->variables[dimension][k];
					}
					for (uint k = j; k < p1->getNumBits(); k++) {
						ch->variables[dimension][k] = p1->variables[dimension][k];
					}
				}
				else
					ch->variables[dimension][i] = p2->variables[dimension][i];
			}
		}
	}

	ch->update();

	return true;
}

}