#include "ECF.h"

// Za svaki primjer je potrebno:
// a) definirati odgovarajuci EvaluateOp objekt koji se primjenjuje za evaluaciju jedinke
// b) definirati genotip (po zelji i druge parametre) u konf fajlu
//
// Svaki primjer ima posebnu funkciju main() - otkomentirati


// 0. primjer: kako ispitati fitnes jedinke (spremljene u ind.txt)





// 1. primjer: GA OneMax problem
/*
#include "examples/GAonemax/OneMaxEvalOp.h"
int main(int argc, char **argv)
{
	argc = 2;	// hard coded za lakse isprobavanje :)
	argv[1] = "./examples/GAOneMax/parametri.txt";

	StateP state (new State);

	//state->setEvalOp(static_cast<EvaluateOpP> (new OneMaxEvalOp));
	state->setEvalOp(new OneMaxEvalOp);

	state->initialize(argc, argv);
	state->run();

	return 0;
}
*/





// 2. primjer: GA minimizacija funkcije

#include "examples/GAFunctionMin/FunctionMinEvalOp.h"
#include "AlgGenHookeJeeves.h"
#include "AlgDifferentialEvolution.h"
int main(int argc, char **argv)
{
//	argc = 2;
//	argv[1] = "./examples/GAFunctionMin/parametri.txt";

	// PSO algoritam:
	//argv[1] = "./examples/GAFunctionMin/parameters_DE.txt";

	GenHookeJeevesP alg (new GenHookeJeeves);

	StateP state (new State);
	state->addAlgorithm(alg);

	state->setEvalOp(static_cast<EvaluateOpP> (new FunctionMinEvalOp));

	state->initialize(argc, argv);
	state->run();

	return 0;
}



namespace Tree {

// primjer tipa podataka
struct my_type
{
	double v;
	bool b;
};

// terminal za doticni tip
class MyTerminal : public Primitives::Primitive
{
public:
	my_type value_;

	MyTerminal()
	{
		nArguments_ = 0;
	}
	void execute(void* result, Tree& tree)
	{
		my_type& res = *(my_type*)result;
		res = value_;
	}
	void setValue(void* value)
	{
		value_ = *(my_type*)value;
	}
	~MyTerminal()
	{	}
};

// primjer funkcije za korisnicki tip podataka
class MyFunc : public Primitives::Primitive
{
public:
	MyFunc()
	{
		nArguments_ = 2;
		name_ = "func";
	}
	void execute(void* result, Tree& tree)
	{
		my_type first, second;
		my_type& func = *(my_type*)result;

		getNextArgument(&first, tree);
		getNextArgument(&second, tree);
		
		func.b = first.b && second.b;
		func.v = first.v + second.v;
	}
	~MyFunc()
	{	}
};

}



// 3. primjer: GP simbolicka regresija
/*
#include "examples/GPSymbReg/SymbRegEvalOp.h"
#include "examples/GPSymbReg/zbr.h"
int main(int argc, char **argv)
{
	argc = 2;
	argv[1] = "./examples/GPSymbReg/parametri.txt";

	StateP state = static_cast<StateP> (new State);

	state->setEvalOp(static_cast<EvaluateOpP> (new SymbRegEvalOp));

	// primjer: dodavanje korisnickog operatora
//	MyOpP myOp = (MyOpP) (new MyOp);
//	state->addOperator(myOp);

	// primjer: dodavanje korisnickog algoritma
//	MyAlgP myAlg = (MyAlgP) (new MyAlg);
//	state->addAlgorithm(myAlg);

	// primjer: dodavanje korisnickog genotipa
//	MyGenotypeP myGenotype = (MyGenotypeP) (new MyGenotype);
//	state->addGenotype(myGenotype);

	// primjer: dodavanje korisnicke funkcije za stablo
	TreeP tree = (TreeP) new Tree::Tree;
	Tree::PrimitiveP zbr = (Tree::PrimitiveP) new Tree::Ad;
	tree->addFunction(zbr);

Tree::PrimitiveP myFunc = (Tree::PrimitiveP) new Tree::MyFunc;
tree->addFunction(myFunc);

Tree::PrimitiveP myTerm = (Tree::PrimitiveP) new Tree::MyTerminal;
myTerm->setName("term");
tree->addTerminal(myTerm);


	state->addGenotype(tree);

	//for(int i = 0; i < 100; i++) {
		state->initialize(argc, argv);
		state->run();
	//}

	//std::vector<IndividualP> vec = state->getPopulation()->hof_->getBest();
	//IndividualP ind = vec[0];
	//state->getAlgorithm()->evaluate(ind);
	//XMLNode out;
	//ind->write(out);
	//std::cout << out.createXMLString() << std::endl;

	return 0;
}
*/


//4. primjer: GA problem trgovackog putnika, 29 gradova
/*
#include "examples/GATSP/TSPEvalOp.h"
int main(int argc, char **argv)
{
//	argc = 2;
//	argv[1] = "./examples/GATSP/parameters.txt";

	StateP state = static_cast<StateP> (new State);

	state->setEvalOp(static_cast<EvaluateOpP> (new TSPEvalOp));

	state->initialize(argc, argv);
	//state->getFitnessObject();

	state->run();

	return 0;
}
*/



//5. primjer: GA problem aproksimacije funkcije
/*
#include "examples/GAApprox/ApproxEvalOp.h"
int main(int argc, char **argv)
{
//	argc = 2;
//	argv[1] = "./examples/GAApprox/parameters.txt";

	StateP state(new State);

	state->setEvalOp(EvaluateOpP (new AproxEvalOp));

	state->initialize(argc, argv);
	state->run();

	return 0;
}
*/



//6. primjer: GP evolucija pravila rasporedjivanja
/*
#include "examples/GPScheduling/SchedulingEvalOp.h"
int main(int argc, char **argv)
{
	argc = 2;
	//argv[1] = "./examples/GPScheduling/parameters.txt";
	argv[1] = "./parameters.txt";

	StateP state = static_cast<StateP> (new State);

	state->setEvalOp(static_cast<EvaluateOpP> (new SchedulingEvalOp));

	state->initialize(argc, argv);
	state->run();

//	XMLNode xInd = XMLNode::parseFile("./ind.txt", "Individual");
//	IndividualP ind = (IndividualP) new Individual(state);
//	ind->read(xInd);
//	state->getAlgorithm()->evaluate(ind);
//	std::cout << ind->toString();

	return 0;
}
*/


//7. primjer: XCS
/*
#include "examples/XCSmux/MuxEvalOp.h"
#include "examples/XCSmaze/SingleObjMazeEnv.h"
#include "examples/XCSmaze/SeqObjMazeEnv.h"
#include "examples/XCSmaze/TwoObjMazeEnv.h"
#include "examples/XCSmaze/ThreeObjMazeEnv.h"

int main(int argc, char **argv)
{
	argc = 2;
	StateP state = static_cast<StateP> (new State);
	MazeEnvP maze;

	//Multistep:

	//	- sigle-objective maze:
		//argv[1] = "examples/XCSmaze/single-obj params.txt";
		//maze = static_cast<MazeEnvP> (new SingleObjMazeEnv(state));
		//maze->setMazeFile("examples/XCSmaze/Environments/single-objective/Maze1.txt");

	//	- multi-objective maze:
		//argv[1] = "examples/XCSmaze/seq-obj params.txt";
		//maze = static_cast<MazeEnvP> (new SeqObjMazeEnv(state));
		//maze->setMazeFile("examples/XCSmaze/Environments/multi-objective/Maze1k.txt");
	
		argv[1] = "examples/XCSmaze/three-obj params.txt";
		maze = static_cast<MazeEnvP> (new ThreeObjMazeEnv(state,0));
		maze->setMazeFile("examples/XCSmaze/Environments/multi-objective/Maze1em.txt");
	
	maze->setResultsFile("examples/XCSmaze/Maze1k results.txt");
	state->setEvalOp(maze);
	
	//Singlestep:

	//	- 6-multiplexor problem:
	//	argv[1] = "./examples/XCSmux/parametri.txt";
	//	state->setEvalOp(static_cast<EvaluateOpP> (new MuxEvalOp(state)));

	state->initialize(argc, argv);

	state->run();

	int a;
	cin >> a;
	return 0;
}
*/



//8. primjer: Kartezijski GP - feedforward
/*
#include "examples/CGPFeedForward/FeedForwardEvalOp.h"
using namespace cart;

int main(int argc, char **argv)
{
	argc = 2;
	argv[1] = "./examples/CGPFeedForward/parametri.txt";

	StateP state (new State);

	//izabrati koji tip
	//state->setEvalOp(new cart::FeedForwardEvalOpInt);
	state->setEvalOp(static_cast<EvaluateOpP> (new cart::FeedForwardEvalOpDouble));
	//state->setEvalOp(static_cast<EvaluateOpP> (new cart::CircuitEvalOpUint));

	state->initialize(argc, argv);
	state->run();

	return 0;
}
*/
