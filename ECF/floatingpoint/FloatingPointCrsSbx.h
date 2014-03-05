#ifndef FLOATINGPOINTCRSSBX_H_
#define FLOATINGPOINTCRSSBX_H_

#include "FloatingPoint.h"


namespace FloatingPoint
{

/**
 * \brief FloatingPoint genotype: SBX, kao sto je implementirano u NSGA 
 */
class FloatingPointCrsSbx: public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);

	uint ni;
};
typedef boost::shared_ptr<FloatingPointCrsSbx> FloatingPointCrsSbxP;
}
#endif /* FLOATINGPOINTCRSSBX_H_ */

