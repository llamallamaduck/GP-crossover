
#include <iostream>
#include <sstream>
#include <cmath>
#include <sstream>
#include <time.h>

#include <ecf/ECF.h>
#include "ecf/tree/Tree.h"
#include <cstdio>
#include "Mux11EvalOp.h"
#include "ecf/tree/Add.h"


int main(int argc, char **argv)
{
	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new Mux11EvalOp);

	Tree::Tree*  tree = new Tree::Tree;
	state->addGenotype((GenotypeP)tree);


	state->initialize(argc, argv);
	state->run();

	Population p = *(state->getPopulation());


	return 0;
}
