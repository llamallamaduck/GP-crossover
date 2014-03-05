#ifndef PERMUTATIONCRSCYCLIC_H
#define PERMUTATIONCRSCYCLIC_H

#include "Permutation.h"


namespace Permutation
{

/**
 * \brief Permutation genotype: Cyclic crossover operator (adapted from HeuristicLab)
 */
class PermutationCrsCyclic : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);	
	bool initialize(StateP);
	void registerParameters(StateP);
};
typedef boost::shared_ptr<PermutationCrsCyclic> PermutationCrsCyclicP;
}
#endif //PERMUTATIONCRSCYCLIC_H
