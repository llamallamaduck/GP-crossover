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
			Tree* child = new Tree();

			int type = 0;

			//boolean
			if (type == 0)
			{
				//create random tree
				Tree* randomTree = new Tree();
				randomTree->maxDepth_ = male->maxDepth_;
				randomTree->minDepth_ = male->minDepth_;
				randomTree->initMaxDepth_ = male->initMaxDepth_;
				randomTree->initMinDepth_ = male->initMinDepth_;
				randomTree->growBuild(male->primitiveSet_);

				//create negated random tree
				Tree* negatedRandomTree = new Tree();
				PrimitiveP subP(new Primitives::Not);
				Node* nodeSub = new Node();
				nodeSub->setPrimitive(subP);
				negatedRandomTree->addNode(nodeSub);
				for (int i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->push_back(node);
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
			}
			//symbolic regression
			if (type == 1)
			{
				//create random tree
				Tree* randomTree = new Tree();
				randomTree->maxDepth_ = male->maxDepth_;
				randomTree->minDepth_ = male->minDepth_;
				randomTree->initMaxDepth_ = male->initMaxDepth_;
				randomTree->initMinDepth_ = male->initMinDepth_;
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
				for (int i = 0; i < randomTree->size(); i++)
				{
					NodeP node = static_cast<NodeP> (new Node(randomTree->at(i)->primitive_));
					negatedRandomTree->push_back(node);
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
			}
			//program
			else if (type == 2) 
			{
				PrimitiveSetP primitives = male->primitiveSet_;

				int numberOfArguments;
				int numberOfTries = 0;
				PrimitiveP root;
				
				//find a primitive that has exactly two arguments
				do
				{
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
}