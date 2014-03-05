#ifndef FLOATINGPOINTCRSLOCAL_H_
#define FLOATINGPOINTCRSLOCAL_H_


namespace FloatingPoint
{

/**
 * \ingroup genotypes flpoint
 * \brief FloatingPoint genotype: offspring is defined as a linear combination of two vectors.
 */
class FloatingPointCrsLocal: public CrossoverOp
{
public:
	bool mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child);
	bool initialize(StateP);
	void registerParameters(StateP);
};
typedef boost::shared_ptr<FloatingPointCrsLocal> FloatingPointCrsLocalP;
}
#endif /* FLOATINGPOINTCRSLOCAL_H_ */

