#include "../ECF_base.h"
#include "FloatingPoint.h"
#include <math.h>

namespace FloatingPoint
{

void FloatingPointCrsSbx::registerParameters(StateP state)
{
	myGenotype_->registerParameter(state, "crx.sbx", (voidP) new double(0), ECF::DOUBLE);
	state->getRegistry()->registerEntry("ni", (voidP) new uint(1), ECF::UINT);
}


bool FloatingPointCrsSbx::initialize(StateP state)
{
	voidP sptr = myGenotype_->getParameterValue(state, "crx.sbx");
	probability_ = *((double*)sptr.get());

	voidP par = state->getRegistry()->getEntry("ni");
	ni = *((uint*) par.get());

	return true;
}


bool FloatingPointCrsSbx::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child)
{
	FloatingPoint* p1 = (FloatingPoint*) (gen1.get());
	FloatingPoint* p2 = (FloatingPoint*) (gen2.get());
	FloatingPoint* ch = (FloatingPoint*) (child.get());

	double u = state_->getRandomizer()->getRandomDouble();
	// scale down to avoid u == 1
	u *= 0.999;
	double beta = 0.0;

	if (u < 0.5) { // if u is smaller than 0.5 perform a contracting crossover
		beta = pow(2 * u, 1.0 / (ni + 1.0));
	}
	else { // otherwise perform an expanding crossover
		beta = pow(0.5 / (1.0 - u), 1.0 / (ni + 1.0));
	}

	switch (state_->getRandomizer()->getRandomInteger(0, 1)) {
		case 0: for (uint i = 0; i < p1->realValue.size(); i++) {
					ch->realValue[i] = ((p1->realValue[i] + p2->realValue[i])/2.0) - beta * 0.5 * fabs(p1->realValue[i] - p2->realValue[i]);
				}
				break;
		case 1: for (uint i = 0; i < p1->realValue.size(); i++) {
					ch->realValue[i] = ((p1->realValue[i] + p2->realValue[i])/2.0) + beta * 0.5 * fabs(p1->realValue[i] - p2->realValue[i]);
				}
	}

	return true;
}

}
