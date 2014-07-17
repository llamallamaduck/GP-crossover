
#include <iostream>
#include <sstream>
#include <cmath>
#include <sstream>
#include <time.h>

#include <ecf/ECF.h>
#include "ecf/tree/Tree.h"
#include <cstdio>
#include "Mux6EvalOp.h"
#include "ecf/tree/Add.h"



class OrB : public Tree::Primitives::Primitive
{
public:
	OrB()
	{
		nArguments_ = 2;
		name_ = "ORB";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool* orB = (bool*)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		*orB = arg1 || arg2;
	}

	~OrB()
	{	}
};


class XorB : public Tree::Primitives::Primitive
{
public:
	XorB()
	{
		nArguments_ = 2;
		name_ = "XORB";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool* xorB = (bool*)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		*xorB = (arg1 && !arg2) || (!arg1 && arg2);
	}

	~XorB()
	{	}
};



class AndB : public Tree::Primitives::Primitive
{
public:
	AndB()
	{
		nArguments_ = 2;
		name_ = "ANDB";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool* and = (bool*)result;

		bool arg1, arg2;

		getNextArgument(&arg1, tree);
		getNextArgument(&arg2, tree);

		*and = (arg1 && arg2);
	}

	~AndB()
	{	}
};


class NotB : public Tree::Primitives::Primitive
{
public:
	NotB()
	{
		nArguments_ = 1;
		name_ = "NOTB";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool* notB = (bool*)result;

		bool arg1;

		getNextArgument(&arg1, tree);

		*notB = !arg1;
	}

	~NotB()
	{	}
};


class IfB : public Tree::Primitives::Primitive
{
public:
	IfB()
	{
		nArguments_ = 3;
		name_ = "IFB";
	}

	void execute(void* result, Tree::Tree& tree)
	{
		bool* res = (bool*)result;

		bool arg, res1, res2;
		getNextArgument(&arg, tree);
		getNextArgument(&res1, tree);
		getNextArgument(&res2, tree);

		if(arg) {
			*res = res1;
		}
		else {
			*res = res2;
		}
	}

	~IfB()
	{	}
};


namespace Tree{

	class TreeCrxSemanticBool : public CrossoverOp
	{
	public:
		void TreeCrxSemanticBool::registerParameters(StateP state)
		{
			myGenotype_->registerParameter(state, "crx.semanticbool", (voidP) new double(0), ECF::DOUBLE);
		}


		bool TreeCrxSemanticBool::initialize(StateP state)
		{
			voidP sptr = myGenotype_->getParameterValue(state, "crx.semanticbool");
			probability_ = *((double*)sptr.get());
			return true;
		}


		int TreeCrxSemanticBool::calculateSize(int avg)
		{
			int sigma = avg - 1;
			return avg + state_->getRandomizer()->getRandomInteger(-sigma, sigma);
		}

		bool TreeCrxSemanticBool::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP ch)
		{
			Tree* male = (Tree*)(gen1.get());
			Tree* female = (Tree*)(gen2.get());
			Tree* child = new Tree();

			child->clear();

			//create random tree
			Tree* randomTree = new Tree();
			randomTree->maxDepth_ = 1;
			randomTree->minDepth_ = 0;
			randomTree->initMaxDepth_ = 1;
			randomTree->initMinDepth_ = 0;

			randomTree->growBuild(male->primitiveSet_);

			//create negated random tree
			Tree* negatedRandomTree = new Tree();
			PrimitiveP subP(new NotB);
			Node* nodeSub = new Node();
			nodeSub->setPrimitive(subP);
			negatedRandomTree->addNode(nodeSub);
			for(uint i = 0; i < randomTree->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
				negatedRandomTree->addNode(node);
			}

			//set the root of the child to +
			PrimitiveP addP(new OrB);
			Node* nodeAdd = new Node();
			nodeAdd->setPrimitive(addP);
			child->addNode(nodeAdd);

			//generate left subtree
			Tree* leftSubtree = new Tree();
			PrimitiveP mulP(new AndB);
			Node* nodeMul = new Node();
			nodeMul->setPrimitive(mulP);
			leftSubtree->addNode(nodeMul);
			//leftSubtree->addFunction((PrimitiveP)mulP);
			for(uint i = 0; i < male->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				leftSubtree->addNode(node);
			}
			for(uint i = 0; i < randomTree->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
				leftSubtree->addNode(node);
			}

			//append left subtree
			for(uint i = 0; i < leftSubtree->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(leftSubtree->at(i)->primitive_));
				child->addNode(node);
			}

			//create right subtree
			Tree* rightSubtree = new Tree();
			rightSubtree->addNode(nodeMul);
			for(uint i = 0; i < negatedRandomTree->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(negatedRandomTree->at(i)->primitive_));
				rightSubtree->addNode(node);
			}
			for(uint i = 0; i < female->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(female->at(i)->primitive_));
				rightSubtree->addNode(node);
			}

			//append right subtree
			for(uint i = 0; i < rightSubtree->size(); i++)
			{
				NodeP node = static_cast<NodeP> (new Node(rightSubtree->at(i)->primitive_));
				child->addNode(node);
			}
			child->update();

			return true;
		}
	};
	typedef boost::shared_ptr<TreeCrxSemanticBool> TreeCrxSemanticBoolP;

	class MyTree : public Tree
	{
	public:

		MyTree(void) {
			name_ = "MyTree";
		}

		MyTree* copy()
		{
			MyTree *newObject = new MyTree(*this);
			return newObject;
		}

		vector<CrossoverOpP> getCrossoverOp()
		{
			vector<CrossoverOpP> crx;

			crx = Tree::getCrossoverOp();

			// dodati nas crx operatorB:
			crx.push_back((CrossoverOpP)(new TreeCrxSemanticBool));

			return crx;
		}


	};

}
typedef boost::shared_ptr<Tree::MyTree> MyTreeP;


int main(int argc, char **argv)
{
	StateP state(new State);

	// set the evaluation operatorB
	state->setEvalOp(new Mux6EvalOp);

	Tree::MyTree*  tree = new Tree::MyTree;


	// create new functions and add them to function set 
	Tree::PrimitiveP ifBl(new IfB);
	tree->addFunction(ifBl);
	Tree::PrimitiveP orB(new OrB);
	tree->addFunction(orB);
	Tree::PrimitiveP and(new AndB);
	tree->addFunction(and);
	Tree::PrimitiveP notB(new NotB);
	tree->addFunction(notB);
	Tree::PrimitiveP xorB(new XorB);
	tree->addFunction(xorB);

	state->addGenotype((GenotypeP)tree);
	state->initialize(argc, argv);
	state->run();

	Population p = *(state->getPopulation());


	return 0;
}
