#ifndef FLOATINGPOINTCRSBLXALPHABETA_H_
#define FLOATINGPOINTCRSBLXALPHABETA_H_

#include "FloatingPoint.h"


namespace FloatingPoint
{

/**
 * \ingroup genotypes flpoint
 * \brief FloatingPoint genotype: value on allele i is random value taken from min-max interval from parents plus/minus difference times rand
 */
class FloatingPointCrsBlxAlphaBeta: public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);

	double alpha;
	double beta;
};
typedef boost::shared_ptr<FloatingPointCrsBlxAlphaBeta> FloatingPointCrsBlxAlphaBetaP;
}
#endif /* FLOATINGPOINTCRSBLXALPHABETA_H_ */

