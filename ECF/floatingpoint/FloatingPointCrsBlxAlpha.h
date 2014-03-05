#ifndef FLOATINGPOINTCRSBLXALPHA_H_
#define FLOATINGPOINTCRSBLXALPHA_H_

#include "FloatingPoint.h"


namespace FloatingPoint
{

/**
 * \ingroup genotypes flpoint
 * \brief FloatingPoint genotype: value on allele i is random value taken from min-max interval from parents plus/minus difference times rand
 */
class FloatingPointCrsBlxAlpha: public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);

	double alpha;
};
typedef boost::shared_ptr<FloatingPointCrsBlxAlpha> FloatingPointCrsBlxAlphaP;
}
#endif /* FLOATINGPOINTCRSBLXALPHA_H_ */

