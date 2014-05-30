
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

class Or : public Tree::Primitives::Primitive
{
public:
	Or()
	{
		nArguments_ = 2;
		name_ = "OR";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool or = (bool)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		or = arg1 || arg2;
	}

	~Or()
	{	}
};


class Xor : public Tree::Primitives::Primitive
{
public:
	Xor()
	{
		nArguments_ = 2;
		name_ = "XOR";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool xor = (bool)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		xor = (arg1 && !arg2) || (!arg1 && arg2);
	}

	~Xor()
	{	}
};



class And : public Tree::Primitives::Primitive
{
public:
	And()
	{
		nArguments_ = 2;
		name_ = "AND";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool and = (bool)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		and = (arg1 && arg2);
	}

	~And()
	{	}
};


class Not : public Tree::Primitives::Primitive
{
public:
	Not()
	{
		nArguments_ = 1;
		name_ = "NOT";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool not = (bool)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);

		not = !arg1;
	}

	~Not()
	{	}
};


class If : public Tree::Primitives::Primitive
{
public:
	If()
	{
		nArguments_ = 3;
		name_ = "IF";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool res = (bool)result;

		bool arg, res1, res2;
		getNextArgument(&arg, tree);
		getNextArgument(&res1, tree);
		getNextArgument(&res2, tree);

		if (arg) {
			res = res1;
		}
		else {
			res = res2;
		}
	}

	~If()
	{	}
};


int main(int argc, char **argv)
{
	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new Mux11EvalOp);

	Tree::Tree*  tree = new Tree::Tree;
	state->addGenotype((GenotypeP)tree);

	// create new functions and add them to function set 
	Tree::PrimitiveP ifl(new If);
	tree->addFunction(ifl);
	Tree::PrimitiveP or(new Or);
	tree->addFunction(or);
	Tree::PrimitiveP and(new And);
	tree->addFunction(and);
	Tree::PrimitiveP not(new Not);
	tree->addFunction(not);
	Tree::PrimitiveP xor(new Xor);
	tree->addFunction(xor);

	state->initialize(argc, argv);
	state->run();

	Population p = *(state->getPopulation());


	return 0;
}
