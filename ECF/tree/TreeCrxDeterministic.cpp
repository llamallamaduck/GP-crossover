#include "../ECF_base.h"
#include "Tree.h"
#include "TreeCrxDeterministic.h"

namespace Tree
{
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
			while (1) 
			{
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
				for (uint j = 1; j < (1 + 2 * subtreeSize) && j < (uint)femaleSizeIndexes.size(); j++)
				{
					//index iterates over subtrees of j size
					for (uint index = 0; index < (uint)femaleSizeIndexes[j].size(); index++)
					{
						femaleMax = female->at(femaleSizeIndexes[j].at(index))->primitive_->maxComputedValue;
						femaleMin = female->at(femaleSizeIndexes[j].at(index))->primitive_->minComputedValue;
						double tmpDistance = 0.5 * (abs(maleMax - femaleMax) + abs(maleMin - femaleMin));
						if (abs(tmpDistance) < abs(distance))
						{
							distance = tmpDistance;
							fIndex = index;
						}
					}
				}

				mNodeDepth = male->at(mIndex)->depth_;
				fNodeDepth = female->at(fIndex)->depth_;

				// find max depth
				int maxDepth = fNodeDepth, depth;
				for (uint i = 0; i < female->at(fIndex)->size_; i++)
				{
					depth = female->at(fIndex + i)->depth_;
					maxDepth = depth > maxDepth ? depth : maxDepth;
				}

				fNodeDepthSize = maxDepth - fNodeDepth;
				nTries++;

				if (nTries > 4 || mNodeDepth + fNodeDepthSize <= male->maxDepth_) break;
			}

			if (nTries > 4 && mNodeDepth + fNodeDepthSize > male->maxDepth_)
			{
				ECF_LOG(state_, 5, "TreeCrxDeterministic not successful.");
				return false;
			}

			child->clear();
			child->maxDepth_ = male->maxDepth_;
			child->minDepth_ = male->minDepth_;
			child->startDepth_ = male->startDepth_;

			// copy from male parent
			for (uint i = 0; i < mIndex; i++)
			{
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
				child->at(i)->depth_ = male->at(i)->depth_;

			}

			// copy from female parent
			for (uint i = 0; i < female->at(fIndex)->size_; i++)
			{
				NodeP node = static_cast<NodeP> (new Node(female->at(fIndex + i)->primitive_));
				child->push_back(node);
			}

			// copy rest from male parent
			for (uint i = mIndex + male->at(mIndex)->size_; i < mRange; i++)
			{
				NodeP node = static_cast<NodeP> (new Node(male->at(i)->primitive_));
				child->push_back(node);
			}

			// update node depths and subtree sizes
			child->update();

			return true;
		}

	}
