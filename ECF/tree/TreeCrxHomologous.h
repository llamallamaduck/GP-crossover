#ifndef TreeCrxHomologous_h
#define TreeCrxHomologous_h


namespace Tree
{

/**
 * \ingroup genotypes tree
 * \brief Tree genotype: homologous crossover operator
 * Reference: https://www.dropbox.com/s/mheiumjitqpcld1/size%20fair%20and%20homologous.pdf
 */
class TreeCrxHomologous : public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	int calculateSize(int avg);
	void registerParameters(StateP);
protected:
	double funcChoiceProb_;
};
typedef boost::shared_ptr<TreeCrxHomologous> TreeCrxHomologousP;
}
#endif // TreeCrxHomologous_h
