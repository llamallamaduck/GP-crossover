#include <cmath>
#include <ecf/ECF.h>
#include "Mux11EvalOp.h"



// called only once, before the evolution – generates training data usually,
// does really nothing in this case
bool Mux11EvalOp::initialize(StateP state)
{
	return true;
}


FitnessP Mux11EvalOp::evaluate(IndividualP individual)
{
	// we try to minimize the function value, so we use FitnessMin fitness (for minimization problems)
	FitnessP fitness(new FitnessMin);

	// get the genotype we defined in the configuration file
	Tree::Tree* tree = (Tree::Tree*) individual->getGenotype().get();
	// (you can also use boost smart pointers:)
	//TreeP tree = boost::static_pointer_cast<Tree::Tree> (individual->getGenotype());

	double value = 0;

	for (uint a = 0; a < 8; a++) {
		int a0 = a & 1;
		int a1 = a & 2;
		int a2 = a & 4;

		tree->setTerminalValue("a0", &a0);
		tree->setTerminalValue("a1", &a1);
		tree->setTerminalValue("a2", &a2);

		for (uint i = 0; i < 256; i++) {
			int d0 = i & 1;
			int d1 = i & 2;
			int d2 = i & 4;
			int d3 = i & 8;
			int d4 = i & 16;
			int d5 = i & 32;
			int d6 = i & 64;
			int d7 = i & 128;

			tree->setTerminalValue("d0", &d0);
			tree->setTerminalValue("d1", &d1);
			tree->setTerminalValue("d2", &d2);
			tree->setTerminalValue("d3", &d3);
			tree->setTerminalValue("d4", &d4);
			tree->setTerminalValue("d5", &d5);
			tree->setTerminalValue("d6", &d6);
			tree->setTerminalValue("d7", &d7);

			int functionValue = 0;

			switch (a) {
			case 0: functionValue = d0;
				break;
			case 1: functionValue = d1;
				break;
			case 2: functionValue = d2;
				break;
			case 3: functionValue = d3;
				break;
			case 4: functionValue = d4;
				break;
			case 5: functionValue = d5;
				break;
			case 6: functionValue = d6;
				break;
			case 7: functionValue = d7;
				break;
			}

			double result;
			tree->execute(&result);
			value += abs(result - functionValue);
		}
	}

	fitness->setValue(value);
	return fitness;
}
