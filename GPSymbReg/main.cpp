
#include <iostream>
#include <sstream>
#include <cmath>
#include <sstream>
#include <time.h>

#include <ecf/ECF.h>
#include "ecf/tree/Tree.h"
#include <cstdio>
#include "SymbRegEvalOp.h"
#include "ecf/tree/Add.h"

namespace Tree{

	class TreeCrxHomologous : public CrossoverOp
	{
	public:
		void TreeCrxHomologous::registerParameters(StateP state)
		{
			myGenotype_->registerParameter(state, "crx.homologous", (voidP) new double(0), ECF::DOUBLE);
		}


		bool TreeCrxHomologous::initialize(StateP state)
		{
			voidP sptr = myGenotype_->getParameterValue(state, "crx.homologous");
			probability_ = *((double*)sptr.get());
			return true;
		}


		int TreeCrxHomologous::calculateSize(int avg)
		{
			int sigma = avg - 1;
			return avg + state_->getRandomizer()->getRandomInteger(-sigma, sigma);
		}


		bool TreeCrxHomologous::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP ch)
		{
			Tree* male = (Tree*)(gen1.get());
			Tree* female = (Tree*)(gen2.get());
			Tree* child = (Tree*)(ch.get());

			uint mIndex, fIndex;
			uint mRange, fRange;
			uint mNodeDepth, fNodeDepth, fNodeDepthSize;

			mRange = (uint)male->size();
			fRange = (uint)female->size();

			// LD: femaleSizeIndexes[i] je vektor indeksa cvorova cija je velicina podstabla = i
			std::vector < std::vector<uint> > femaleSizeIndexes;

			femaleSizeIndexes.resize(fRange + 1);

			for (uint i = 0; i < fRange; i++)
				femaleSizeIndexes[female->at(i)->size_].push_back(i);

			uint nTries = 0;
			while (1) {
				// choose random crx point in male parent
				mIndex = state_->getRandomizer()->getRandomInteger(0, mRange - 1);

				// chose female crx point with expected size less or equal to the male's subtree
				uint subtreeSize = calculateSize(male->at(mIndex)->size_);

				//choose the closest subtree to males breaking point, having size < 1 + 2 * subtreeSize
				int distance = (mIndex - femaleSizeIndexes.at(1)[0]);
				fIndex = femaleSizeIndexes.at(1)[0];

				//j iterates over sizes
				for (uint j = 1; j < (1 + 2 * subtreeSize) && j < femaleSizeIndexes.size(); j++){
					if (distance == 0)
						break;
					//index iterates over subtrees of j size
					for (uint index = 0; index < femaleSizeIndexes[j].size(); index++){
						int tmpDistance = (mIndex - femaleSizeIndexes.at(j)[index]);
						if (abs(tmpDistance) < abs(distance)){
							distance = tmpDistance;
							fIndex = index;
						}
					}
				}

				mNodeDepth = male->at(mIndex)->depth_;
				fNodeDepth = female->at(fIndex)->depth_;

				// find max depth
				int maxDepth = fNodeDepth, depth;
				for (uint i = 0; i < female->at(fIndex)->size_; i++) {
					depth = female->at(fIndex + i)->depth_;
					maxDepth = depth > maxDepth ? depth : maxDepth;
				}

				fNodeDepthSize = maxDepth - fNodeDepth;
				nTries++;

				if (nTries > 4 || mNodeDepth + fNodeDepthSize <= male->maxDepth_) break;
			}

			if (nTries > 4 && mNodeDepth + fNodeDepthSize > male->maxDepth_) {
				ECF_LOG(state_, 5, "TreeCrxSizeFair not successful.");
				return false;
			}

			child->clear();
			child->maxDepth_ = male->maxDepth_;
			child->minDepth_ = male->minDepth_;
			child->startDepth_ = male->startDepth_;

			// copy from male parent
			for (uint i = 0; i < mIndex; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
				child->at(i)->depth_ = male->at(i)->depth_;

			}
			// copy from female parent
			for (uint i = 0; i < female->at(fIndex)->size_; i++) {
				NodeP node = static_cast<NodeP> (new Node(female->at(fIndex + i)->primitive_));
				child->push_back(node);
			}
			// copy rest from male parent
			for (uint i = mIndex + male->at(mIndex)->size_; i < mRange; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
			}

			// update node depths and subtree sizes
			child->update();

			return true;
		}

	};
	typedef boost::shared_ptr<TreeCrxHomologous> TreeCrxHomologousP;

	class TreeCrxDeterministic : public CrossoverOp
	{
	public:
		void TreeCrxDeterministic::registerParameters(StateP state)
		{
			myGenotype_->registerParameter(state, "crx.deterministic", (voidP) new double(0), ECF::DOUBLE);
		}


		bool TreeCrxDeterministic::initialize(StateP state)
		{
			voidP sptr = myGenotype_->getParameterValue(state, "crx.deterministic");
			probability_ = *((double*)sptr.get());
			return true;
		}


		int TreeCrxDeterministic::calculateSize(int avg)
		{
			int sigma = avg - 1;
			return avg + state_->getRandomizer()->getRandomInteger(-sigma, sigma);
		}


		bool TreeCrxDeterministic::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP ch)
		{
			Tree* male = (Tree*)(gen1.get());
			Tree* female = (Tree*)(gen2.get());
			Tree* child = (Tree*)(ch.get());

			uint mIndex, fIndex;
			uint mRange, fRange;
			uint mNodeDepth, fNodeDepth, fNodeDepthSize;

			mRange = (uint)male->size();
			fRange = (uint)female->size();

			// LD: femaleSizeIndexes[i] je vektor indeksa cvorova cija je velicina podstabla = i
			std::vector < std::vector<uint> > femaleSizeIndexes;

			femaleSizeIndexes.resize(fRange + 1);

			for (uint i = 0; i < fRange; i++)
				femaleSizeIndexes[female->at(i)->size_].push_back(i);

			uint nTries = 0;
			while (1) {
				// choose random crx point in male parent
				mIndex = state_->getRandomizer()->getRandomInteger(0, mRange - 1);

				// chose female crx point with expected size less or equal to the male's subtree
				uint subtreeSize = calculateSize(male->at(mIndex)->size_);

				//choose the closest subtree to males breaking point
				double maleMax = male->at(mIndex)->primitive_->maxComputedValue;
				double femaleMax = female->at(femaleSizeIndexes.at(1)[0])->primitive_->maxComputedValue;
				double maleMin = male->at(mIndex)->primitive_->minComputedValue;
				double femaleMin = female->at(femaleSizeIndexes.at(1)[0])->primitive_->minComputedValue;

				double distance = 0.5 * (abs(maleMax - femaleMax) + abs(maleMin - femaleMin));
				fIndex = femaleSizeIndexes.at(1)[0];

				//j iterates over sizes
				for (uint j = 1; j < (1 + 2 * subtreeSize) && j < (uint)femaleSizeIndexes.size(); j++){
					//index iterates over subtrees of j size
					for (uint index = 0; index < (uint)femaleSizeIndexes[j].size(); index++){
						femaleMax = female->at(femaleSizeIndexes[j].at(index))->primitive_->maxComputedValue;
						femaleMin = female->at(femaleSizeIndexes[j].at(index))->primitive_->minComputedValue;
						double tmpDistance = 0.5 * (abs(maleMax - femaleMax) + abs(maleMin - femaleMin));
						if (abs(tmpDistance) < abs(distance)){
							distance = tmpDistance;
							fIndex = index;
						}
					}
				}

				mNodeDepth = male->at(mIndex)->depth_;
				fNodeDepth = female->at(fIndex)->depth_;

				// find max depth
				int maxDepth = fNodeDepth, depth;
				for (uint i = 0; i < female->at(fIndex)->size_; i++) {
					depth = female->at(fIndex + i)->depth_;
					maxDepth = depth > maxDepth ? depth : maxDepth;
				}

				fNodeDepthSize = maxDepth - fNodeDepth;
				nTries++;

				if (nTries > 4 || mNodeDepth + fNodeDepthSize <= male->maxDepth_) break;
			}

			if (nTries > 4 && mNodeDepth + fNodeDepthSize > male->maxDepth_) {
				ECF_LOG(state_, 5, "TreeCrxDeterministic not successful.");
				return false;
			}

			child->clear();
			child->maxDepth_ = male->maxDepth_;
			child->minDepth_ = male->minDepth_;
			child->startDepth_ = male->startDepth_;

			// copy from male parent
			for (uint i = 0; i < mIndex; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
				child->at(i)->depth_ = male->at(i)->depth_;

			}
			// copy from female parent
			for (uint i = 0; i < female->at(fIndex)->size_; i++) {
				NodeP node = static_cast<NodeP> (new Node(female->at(fIndex + i)->primitive_));
				child->push_back(node);
			}
			// copy rest from male parent
			for (uint i = mIndex + male->at(mIndex)->size_; i < mRange; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
			}

			// update node depths and subtree sizes
			child->update();

			return true;
		}

	};
	typedef boost::shared_ptr<TreeCrxDeterministic> TreeCrxDeterministicP;

	class TreeCrxProbabilistic : public CrossoverOp
	{
	public:
		void TreeCrxProbabilistic::registerParameters(StateP state)
		{
			myGenotype_->registerParameter(state, "crx.probabilistic", (voidP) new double(0), ECF::DOUBLE);
		}


		bool TreeCrxProbabilistic::initialize(StateP state)
		{
			voidP sptr = myGenotype_->getParameterValue(state, "crx.probabilistic");
			probability_ = *((double*)sptr.get());
			return true;
		}


		int TreeCrxProbabilistic::calculateSize(int avg)
		{
			int sigma = avg - 1;
			return avg + state_->getRandomizer()->getRandomInteger(-sigma, sigma);
		}


		bool TreeCrxProbabilistic::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP ch)
		{
			Tree* male = (Tree*)(gen1.get());
			Tree* female = (Tree*)(gen2.get());
			Tree* child = (Tree*)(ch.get());

			uint mIndex, fIndex;
			uint mRange, fRange;
			uint mNodeDepth, fNodeDepth, fNodeDepthSize;

			mRange = (uint)male->size();
			fRange = (uint)female->size();

			// LD: femaleSizeIndexes[i] je vektor indeksa cvorova cija je velicina podstabla = i
			std::vector < std::vector<uint> > femaleSizeIndexes;

			femaleSizeIndexes.resize(fRange + 1);

			for (uint i = 0; i < fRange; i++)
				femaleSizeIndexes[female->at(i)->size_].push_back(i);

			uint nTries = 0;
			while (1) {
				//probabilisticDistancesIndices[i] contains a female index whose distance 
				//is stored in probabilisticDistancesValues[i]
				vector<int> probabilisticDistancesIndices;
				//probabilisticDistancesValues[i] contains probabilistic distance of 
				//probabilisticDistancesValues[i]-th node from chosen male node
				vector<double> probabilisticDistancesValues;

				// choose random crx point in male parent
				mIndex = state_->getRandomizer()->getRandomInteger(0, mRange - 1);

				// chose female crx point with expected size less or equal to the male's subtree
				uint subtreeSize = calculateSize(male->at(mIndex)->size_);

				double maleMax = male->at(mIndex)->primitive_->maxComputedValue;
				double maleMin = male->at(mIndex)->primitive_->minComputedValue;
				double sumOfAllDistances = 0;

				//j iterates over sizes
				for (uint j = 1; j < (1 + 2 * subtreeSize) && j < femaleSizeIndexes.size(); j++){
					//index iterates over subtrees of j size
					for (uint index = 0; index < femaleSizeIndexes[j].size(); index++){

						double femaleMax = female->at(femaleSizeIndexes[j].at(index))->primitive_->maxComputedValue;
						double femaleMin = female->at(femaleSizeIndexes[j].at(index))->primitive_->minComputedValue;
						double distance = 0.5 * (abs(maleMax - femaleMax) + abs(maleMin - femaleMin));
						probabilisticDistancesIndices.push_back(femaleSizeIndexes[j].at(index));
						probabilisticDistancesValues.push_back(distance);
						sumOfAllDistances += distance;
					}
				}

				if (sumOfAllDistances == 0)
				{
					nTries++;
					if (nTries > 4) {
						ECF_LOG(state_, 5, "TreeCrxProbabilistic not successful.");
						return false;
					}
					continue;
				}

				//calculate d'
				for (int i = 0; i < probabilisticDistancesIndices.size(); i++){
					double value = probabilisticDistancesValues[i] / sumOfAllDistances;
					if (value < 0.000000000001)
						value = 0;
					probabilisticDistancesValues[i] = value;
				}

				//calculate p
				double sumOfAllInvertedDistances = 0;
				for (int i = 0; i < probabilisticDistancesIndices.size(); i++){
					sumOfAllInvertedDistances += 1 - probabilisticDistancesValues[i];
				}

				for (int i = 0; i < probabilisticDistancesIndices.size(); i++){
					double value = (1 - probabilisticDistancesValues[i]) / sumOfAllInvertedDistances;
					probabilisticDistancesValues[i] = value;
				}

				srand((unsigned)time(NULL));
				double p = ((double)rand() / (double)RAND_MAX);

				double minDiff = 1;
				fIndex = 0;

				for (int i = 0; i < probabilisticDistancesIndices.size(); i++){
					double tempDiff = abs(probabilisticDistancesValues[i] - p);
					if (tempDiff < minDiff){
						minDiff = tempDiff;
						fIndex = probabilisticDistancesIndices[i];
					}
				}

				mNodeDepth = male->at(mIndex)->depth_;
				fNodeDepth = female->at(fIndex)->depth_;

				// find max depth
				int maxDepth = fNodeDepth, depth;
				for (uint i = 0; i < female->at(fIndex)->size_; i++) {
					depth = female->at(fIndex + i)->depth_;
					maxDepth = depth > maxDepth ? depth : maxDepth;
				}

				fNodeDepthSize = maxDepth - fNodeDepth;
				nTries++;

				if (nTries > 4 || mNodeDepth + fNodeDepthSize <= male->maxDepth_) break;
			}

			if (nTries > 4 && mNodeDepth + fNodeDepthSize > male->maxDepth_) {
				ECF_LOG(state_, 5, "TreeCrxProbabilistic not successful.");
				return false;
			}

			child->clear();
			child->maxDepth_ = male->maxDepth_;
			child->minDepth_ = male->minDepth_;
			child->startDepth_ = male->startDepth_;

			// copy from male parent
			for (uint i = 0; i < mIndex; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
				child->at(i)->depth_ = male->at(i)->depth_;

			}
			// copy from female parent
			for (uint i = 0; i < female->at(fIndex)->size_; i++) {
				NodeP node = static_cast<NodeP> (new Node(female->at(fIndex + i)->primitive_));
				child->push_back(node);
			}
			// copy rest from male parent
			for (uint i = mIndex + male->at(mIndex)->size_; i < mRange; i++) {
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
			}

			// update node depths and subtree sizes
			child->update();

			return true;
		}

	};
	typedef boost::shared_ptr<TreeCrxProbabilistic> TreeCrxProbabilisticP;

	class TreeCrxSemantic : public CrossoverOp
	{
	public:
		void TreeCrxSemantic::registerParameters(StateP state)
		{
			myGenotype_->registerParameter(state, "crx.semantic", (voidP) new double(0), ECF::DOUBLE);
		}


		bool TreeCrxSemantic::initialize(StateP state)
		{
			voidP sptr = myGenotype_->getParameterValue(state, "crx.semantic");
			probability_ = *((double*)sptr.get());
			return true;
		}


		int TreeCrxSemantic::calculateSize(int avg)
		{
			int sigma = avg - 1;
			return avg + state_->getRandomizer()->getRandomInteger(-sigma, sigma);
		}

		//TODO - add type to arguments
		bool TreeCrxSemantic::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP ch)
		{
			Tree* male = (Tree*)(gen1.get());
			Tree* female = (Tree*)(gen2.get());
			Tree* child = new Tree();

			int type = 1;

			//logical function
		/*	if (type == 0){

				//create random tree
				Tree* randomTree = new Tree();
				randomTree->maxDepth_ = male->maxDepth_;
				randomTree->minDepth_ = male->minDepth_;
				randomTree->initMaxDepth_ = male->initMaxDepth_;
				randomTree->initMinDepth_ = male->initMinDepth_;
				randomTree->growBuild(male->primitiveSet_);
				ECF_LOG(state_, 5, "Random tree: " + randomTree->toString());

				//create negated random tree
				Tree* negatedRandomTree = new Tree();
				PrimitiveP subP(new Primitives::Sub);
				Node* nodeSub = new Node();
				nodeSub->setPrimitive(subP);
				negatedRandomTree->addNode(nodeSub);
				PrimitiveP one(new Primitives::Terminal);
				int* val = (int*)malloc(sizeof(int));
				*val = 1;
				one->setValue(val);
				one->setName("1");
				Node* nodeOne = new Node();
				nodeOne->setPrimitive(one);
				negatedRandomTree->addNode(nodeOne);
				for (int i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->push_back(node);
				}

				ECF_LOG(state_, 3, "Negated random tree: " + negatedRandomTree->toString());

				//set the root of the child to +
				Primitives::Add* addP = new Primitives::Add();
				child->addFunction((PrimitiveP)addP);

				//generate left subtree
				Tree* leftSubtree = new Tree();
				Primitives::Mul* mulP = new Primitives::Mul();
				leftSubtree->addFunction((PrimitiveP)mulP);
				leftSubtree->addNode(new Node(male->at(0)));
				leftSubtree->addNode(randomTree->at(0));

				//append left subtree
				child->addNode(leftSubtree->at(0));

				//create right subtree
				Tree* rightSubtree = new Tree();
				rightSubtree->addFunction((PrimitiveP)mulP);
				rightSubtree->addNode(negatedRandomTree->at(0));
				rightSubtree->addNode(new Node(female->at(0)));

				//append right subtree
				child->addNode(rightSubtree->at(0));
			}*/
			//symbolic regression or boolean
			if (type == 0 || type == 1){

				//create random tree
				Tree* randomTree = new Tree();
				randomTree->maxDepth_ = male->maxDepth_;
				randomTree->minDepth_ = male->minDepth_;
				randomTree->initMaxDepth_ = male->initMaxDepth_;
				randomTree->initMinDepth_ = male->initMinDepth_;
				randomTree->growBuild(male->primitiveSet_);
				//ECF_LOG(state_, 5, "Random tree: " + randomTree->toString());

				//create negated random tree
				Tree* negatedRandomTree = new Tree();
				PrimitiveP subP(new Primitives::Sub);
				Node* nodeSub = new Node();
				nodeSub->setPrimitive(subP);
				negatedRandomTree->addNode(nodeSub);
				PrimitiveP one(new Primitives::Terminal);
				int* val = (int*)malloc(sizeof(int));
				*val = 1;
				one->setValue(val);
				one->setName("1");
				Node* nodeOne = new Node();
				nodeOne->setPrimitive(one);
				negatedRandomTree->addNode(nodeOne);
				for (int i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->push_back(node);
				}

				//ECF_LOG(state_, 3, "Negated random tree: " + negatedRandomTree->toString());

				//set the root of the child to +
				PrimitiveP addP(new Primitives::Add);
				//child->addFunction((PrimitiveP)addP);
				Node* nodeAdd = new Node();
				nodeAdd->setPrimitive(addP);
				child->addNode(nodeAdd);

				//generate left subtree
				Tree* leftSubtree = new Tree();
				PrimitiveP mulP(new Primitives::Mul);
				Node* nodeMul = new Node();
				nodeMul->setPrimitive(mulP);
				leftSubtree->addNode(nodeMul);
				//leftSubtree->addFunction((PrimitiveP)mulP);
				for (int i = 0; i < male->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
					leftSubtree->push_back(node);
				}
				for (int i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					leftSubtree->push_back(node);
				}

				//append left subtree
				for (int i = 0; i < leftSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(leftSubtree->at(i)->primitive_));
					child->push_back(node);
				}

				//create right subtree
				Tree* rightSubtree = new Tree();
				rightSubtree->addNode(nodeMul);
				for (int i = 0; i < negatedRandomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(negatedRandomTree->at(i)->primitive_));
					rightSubtree->push_back(node);
				}
				for (int i = 0; i < female->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(female->at(i)->primitive_));
					rightSubtree->push_back(node);
				}

				//append right subtree
				for (int i = 0; i < rightSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(rightSubtree->at(i)->primitive_));
					child->push_back(node);
				}

				//ECF_LOG(state_, 3, "CHILD: " + child->toString());
			}
			//program
			else if (type == 2) {
				PrimitiveSetP primitives = male->primitiveSet_;

				int numberOfArguments;
				int numberOfTries = 0;
				PrimitiveP root;
				//find a primitive that has exactly two arguments
				do {
					root = primitives->getRandomPrimitive();
					numberOfArguments = root->getNumberOfArguments();
					numberOfTries++;
				} while (numberOfArguments != 2 && numberOfTries <= 10);
				//create the child tree
				child->addNode(new Node(root));
				child->addNode(new Node(male->at(0)));
				child->addNode(female->at(0));
			}
			return true;
		}
	};
	typedef boost::shared_ptr<TreeCrxSemantic> TreeCrxSemanticP;

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

			// dodati nas crx operator:
			crx.push_back((CrossoverOpP)(new TreeCrxHomologous));
			crx.push_back((CrossoverOpP)(new TreeCrxDeterministic));
			crx.push_back((CrossoverOpP)(new TreeCrxProbabilistic));
			crx.push_back((CrossoverOpP)(new TreeCrxSemantic));

			return crx;
		}


	};

}
typedef boost::shared_ptr<Tree::MyTree> MyTreeP;


int main(int argc, char **argv)
{
	StateP state(new State);

	// set the evaluation operator
	state->setEvalOp(new SymbRegEvalOp);


	Tree::MyTree*  tree = new Tree::MyTree;
	state->addGenotype((GenotypeP)tree);


	state->initialize(argc, argv);
	state->run();

	Population p = *(state->getPopulation());


	return 0;
}
