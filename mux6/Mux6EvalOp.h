#ifndef Mux6EvalOp_h
#define Mux6EvalOp_h


/**
* \defgroup 6 - Multiplexer (GP)
* \ingroup examples
* \brief 6 - Multiplexer
*
* \par Description
* The objective is to find a function representing multiplexer with 3 adress and 8 data bits.
* The fitness is defined with total error for the given set of output values.
*
* \par Tree genotype parameters (in the config file)
* - set of functions (inner tree nodes)
* - set of terminals (variables and constants, tree leaves)
* - min and max tree depth
*/


/**
* \brief 6 - Multiplexer evaluation operator.
*/
class Mux6EvalOp : public EvaluateOp
{
public:
	FitnessP evaluate(IndividualP individual);
	bool initialize(StateP);
};
typedef boost::shared_ptr<Mux6EvalOp> Mux6EvalOpP;

#endif // Mux6EvalOp_h
