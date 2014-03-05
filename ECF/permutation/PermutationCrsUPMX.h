#ifndef PERMUTATIONCRSUPMX_H
#define PERMUTATIONCRSUPMX_H


namespace Permutation
{

/**
 * \ingroup genotypes permutation
 * \brief Permutation genotype: UMPX crossover operator (see e.g. ?)
 */
class PermutationCrsUPMX : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);	
	bool initialize(StateP);
	void registerParameters(StateP);
};
typedef boost::shared_ptr<PermutationCrsUPMX> PermutationCrsUPMXP;
}
#endif //PERMUTATIONCRSUPMX_H
