#ifndef PERMUTATIONCRSULX_H
#define PERMUTATIONCRSULX_H

#include "Permutation.h"


namespace Permutation
{

/**
 * \brief Permutation genotype: Uniform like crossover operator (adapted from HeuristicLab)
 */
class PermutationCrsULX : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);
};
typedef boost::shared_ptr<PermutationCrsULX> PermutationCrsULXP;
}
#endif //PERMUTATIONCRSULX_H
