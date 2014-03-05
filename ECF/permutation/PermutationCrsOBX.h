#ifndef PERMUTATIONCRSOBX_H
#define PERMUTATIONCRSOBX_H

#include "Permutation.h"


namespace Permutation
{

/**
 * \brief Permutation genotype: Order based crossover operator (adapted from HeuristicLab)
 */
class PermutationCrsOBX : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);	
	bool initialize(StateP);
	void registerParameters(StateP);
};
typedef boost::shared_ptr<PermutationCrsOBX> PermutationCrsOBXP;
}
#endif //PERMUTATIONCRSOBX_H
