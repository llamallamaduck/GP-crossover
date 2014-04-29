#ifndef TreeCrxDeterministic_h
#define TreeCrxDeterministic_h


namespace Tree
{

/**
 * \ingroup genotypes tree
 * \brief Tree genotype: deterministic crossover operator
 * Reference: https://www.dropbox.com/s/oakt5emoy5sh14j/probabilistic%20functional%20crossover.pdf
 */
class TreeCrxDeterministic : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	int calculateSize(int avg);
	void registerParameters(StateP);
protected:
	double funcChoiceProb_;
};
typedef boost::shared_ptr<TreeCrxDeterministic> TreeCrxDeterministicP;
}
#endif // TreeCrxDeterministic_h
