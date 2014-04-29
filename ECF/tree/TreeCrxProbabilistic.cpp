#include "../ECF_base.h"
#include "Tree.h"
#include "TreeCrxProbabilistic.h"
#include <time.h>

namespace Tree
{
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
			while (1)
			{
				//probabilisticDistancesIndices[i] contains a female index whose distance 
				//is stored in probabilisticDistancesValues[i]
				std::vector<int> probabilisticDistancesIndices;
				//probabilisticDistancesValues[i] contains probabilistic distance of 
				//probabilisticDistancesValues[i]-th node from chosen male node
				std::vector<double> probabilisticDistancesValues;

				// choose random crx point in male parent
				mIndex = state_->getRandomizer()->getRandomInteger(0, mRange - 1);

				// chose female crx point with expected size less or equal to the male's subtree
				uint subtreeSize = calculateSize(male->at(mIndex)->size_);

				double maleMax = male->at(mIndex)->primitive_->maxComputedValue;
				double maleMin = male->at(mIndex)->primitive_->minComputedValue;
				double sumOfAllDistances = 0;

				//j iterates over sizes
				for (uint j = 1; j < (1 + 2 * subtreeSize) && j < femaleSizeIndexes.size(); j++)
				{
					//index iterates over subtrees of j size
					for (uint index = 0; index < femaleSizeIndexes[j].size(); index++)
					{
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
					if (nTries > 4)
					{
						ECF_LOG(state_, 5, "TreeCrxProbabilistic not successful.");
						return false;
					}
					continue;
				}

				//calculate d'
				for (int i = 0; i < probabilisticDistancesIndices.size(); i++)
				{
					double value = probabilisticDistancesValues[i] / sumOfAllDistances;
					if (value < 0.000000000001)
						value = 0;
					probabilisticDistancesValues[i] = value;
				}

				//calculate p
				double sumOfAllInvertedDistances = 0;
				for (int i = 0; i < probabilisticDistancesIndices.size(); i++)
				{
					sumOfAllInvertedDistances += 1 - probabilisticDistancesValues[i];
				}

				for (int i = 0; i < probabilisticDistancesIndices.size(); i++)
				{
					double value = (1 - probabilisticDistancesValues[i]) / sumOfAllInvertedDistances;
					probabilisticDistancesValues[i] = value;
				}

				srand((unsigned)time(NULL));
				double p = ((double)rand() / (double)RAND_MAX);

				double minDiff = 1;
				fIndex = 0;

				for (int i = 0; i < probabilisticDistancesIndices.size(); i++)
				{
					double tempDiff = abs(probabilisticDistancesValues[i] - p);
					if (tempDiff < minDiff)
					{
						minDiff = tempDiff;
						fIndex = probabilisticDistancesIndices[i];
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

			if (nTries > 4 && mNodeDepth + fNodeDepthSize > male->maxDepth_) {
				ECF_LOG(state_, 5, "TreeCrxProbabilistic not successful.");
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