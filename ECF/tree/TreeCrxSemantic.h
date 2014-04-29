#ifndef TreeCrxSemantic_h
#define TreeCrxSemantic_h


namespace Tree
{

/**
 * \ingroup genotypes tree
 * \brief Tree genotype: Semantic crossover operator
 * Reference: https://www.dropbox.com/s/ceiysbz9tpn0r9d/semantic-gp.pdf
 */
class TreeCrxSemantic : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	int calculateSize(int avg);
	void registerParameters(StateP);
protected:
	double funcChoiceProb_;
};
typedef boost::shared_ptr<TreeCrxSemantic> TreeCrxSemanticP;
}
#endif // TreeCrxSemantic_h
