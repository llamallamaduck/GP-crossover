#ifndef FLOATINGPOINTCRSBGA_H_
#define FLOATINGPOINTCRSBGA_H_

#include "FloatingPoint.h"


namespace FloatingPoint
{

/**
 * \ingroup genotypes flpoint
 * \brief FloatingPoint genotype: BGA (TODO: description)
 */
class FloatingPointCrsBga: public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);

};
typedef boost::shared_ptr<FloatingPointCrsBga> FloatingPointCrsBgaP;
}
#endif /* FLOATINGPOINTCRSBGA_H_ */

