#include "../ECF_base.h"
#include "Tree.h"
#include "TreeCrxSemantic.h"

#include "And.h"
#include "Or.h"
#include "Not.h"


namespace Tree
{
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
			Tree* child = (Tree*)(ch.get());

			int type = 0;

			//boolean
			if (type == 0)
			{
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
				PrimitiveP subP(new Primitives::Not);
				Node* nodeSub = new Node();
				nodeSub->setPrimitive(subP);
				negatedRandomTree->addNode(nodeSub);
				for (uint i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->addNode(node);
				}

				//set the root of the child to +
				PrimitiveP addP(new Primitives::Or);
				Node* nodeAdd = new Node();
				nodeAdd->setPrimitive(addP);
				child->addNode(nodeAdd);

				//generate left subtree
				Tree* leftSubtree = new Tree();
				PrimitiveP mulP(new Primitives::And);
				Node* nodeMul = new Node();
				nodeMul->setPrimitive(mulP);
				leftSubtree->addNode(nodeMul);
				//leftSubtree->addFunction((PrimitiveP)mulP);
				for (uint i = 0; i < male->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
					leftSubtree->addNode(node);
				}
				for (uint i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					leftSubtree->addNode(node);
				}

				//append left subtree
				for (uint i = 0; i < leftSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(leftSubtree->at(i)->primitive_));
					child->addNode(node);
				}

				//create right subtree
				Tree* rightSubtree = new Tree();
				rightSubtree->addNode(nodeMul);
				for (uint i = 0; i < negatedRandomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(negatedRandomTree->at(i)->primitive_));
					rightSubtree->addNode(node);
				}
				for (uint i = 0; i < female->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(female->at(i)->primitive_));
					rightSubtree->addNode(node);
				}

				//append right subtree
				for (uint i = 0; i < rightSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(rightSubtree->at(i)->primitive_));
					child->addNode(node);
				}
				child->update();
			}
			//symbolic regression
			if (type == 1)
			{
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
				for (uint i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->addNode(node);
				}

				//set the root of the child to +
				PrimitiveP addP(new Primitives::Add);
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
				for (uint i = 0; i < male->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
					leftSubtree->addNode(node);
				}
				for (uint i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					leftSubtree->addNode(node);
				}

				//append left subtree
				for (uint i = 0; i < leftSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(leftSubtree->at(i)->primitive_));
					child->addNode(node);
				}

				//create right subtree
				Tree* rightSubtree = new Tree();
				rightSubtree->addNode(nodeMul);
				for (uint i = 0; i < negatedRandomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(negatedRandomTree->at(i)->primitive_));
					rightSubtree->addNode(node);
				}
				for (uint i = 0; i < female->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(female->at(i)->primitive_));
					rightSubtree->addNode(node);
				}

				//append right subtree
				for (uint i = 0; i < rightSubtree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(rightSubtree->at(i)->primitive_));
					child->addNode(node);
				}

				child->update();
			}
			//program
			else if (type == 2) 
			{
				PrimitiveSetP primitives = male->primitiveSet_;

				int numberOfArguments;
				int numberOfTries = 0;
				PrimitiveP root;
				child->initMaxDepth_ = male->initMaxDepth_;
				child->initMinDepth_ = male->initMinDepth_;
				child->maxDepth_ = male->maxDepth_;
				child->minDepth_ = male->minDepth_;
			
				//find a primitive that has exactly two arguments
				do
				{
					root = primitives->getRandomPrimitive();
					numberOfArguments = root->getNumberOfArguments();
					numberOfTries++;
				} while (numberOfArguments != 2 && numberOfTries <= 10);

				if (numberOfTries >= 10){
					return false;
				}

				child->clear();

				//create the child tree
				child->push_back(static_cast<NodeP> (new Node(root)));

				uint currentIndex = 1;
				for (uint i = 0; i < male->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
					child->push_back(node);
					child->at(i)->depth_ = male->at(i)->depth_ + 1;
					currentIndex++;
				}

				for (uint i = 0; i < female->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(female->at(i)->primitive_));
					child->push_back(node);
					child->at(i + currentIndex)->depth_ = female->at(i)->depth_ + 1;
				} 
			}
			child->update();
			return true;
		}
}