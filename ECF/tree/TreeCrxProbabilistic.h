#ifndef TreeCrxProbabilistic_h
#define TreeCrxProbabilistic_h


namespace Tree
{

/**
 * \ingroup genotypes tree
 * \brief Tree genotype: probabilistic crossover operator
 * Reference: https://www.dropbox.com/s/oakt5emoy5sh14j/probabilistic%20functional%20crossover.pdf
 */
class TreeCrxProbabilistic : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	int calculateSize(int avg);
	void registerParameters(StateP);
protected:
	double funcChoiceProb_;
};
typedef boost::shared_ptr<TreeCrxProbabilistic> TreeCrxProbabilisticP;
}
#endif // TreeCrxProbabilistic_h
