#include "../ECF_base.h"
#include "Permutation.h"
#include <map>


namespace Permutation
{

void PermutationCrsSPX::registerParameters(StateP state)
{
	myGenotype_->registerParameter(state, "crx.SPX", (voidP) new double(0), ECF::DOUBLE);
}


bool PermutationCrsSPX::initialize(StateP state)
{
	voidP sptr = myGenotype_->getParameterValue(state, "crx.SPX");
	probability_ = *((double*)sptr.get());
	return true;
}


bool PermutationCrsSPX::mate(GenotypeP gen1, GenotypeP gen2, GenotypeP child)
{
	Permutation* p1 = (Permutation*) (gen1.get());
	Permutation* p2 = (Permutation*) (gen2.get());

	// uzmimo radije smart pointer, da ne moramo rucno brisati visak
	PermutationP ch = boost::static_pointer_cast<Permutation> (child);

	// stvori kopiju genotipa
	PermutationP ch2(ch->copy());
	// dohvati jedinku dijete
	IndividualP myInd = state_->getContext()->child;
	// dohvati redni broj genotipa
	uint gId = child->getGenotypeId();

	// definiraj novi ch i ch2

    //Make ch clone of parent1, ch2 clone of parent2
    int capacity = ch->getSize();
    for(int i = 0; i < capacity;  i++) {
        ch->variables[i] = p1->variables[i];
        ch2->variables[i] = p2->variables[i];
    }

    // Pick two different positions, ensure pos1 < pos2:
    int pos1 = state_->getRandomizer()->getRandomInteger(capacity);
    int pos2 = state_->getRandomizer()->getRandomInteger(capacity);
    if(pos1==pos2) {
        if(pos1==0) {
            pos2++;
        } else {
            pos1--;
        }
    } else if(pos2<pos1) {
        int t = pos1;
        pos1 = pos2;
        pos2 = t;
    }

    // Swap selected element in first child
    int elem1 = p1->variables[pos1];
    int elem2 = p1->variables[pos2];
    ch->variables[pos1] = elem2;
    ch->variables[pos2] = elem1;

    // Find locations of selected elements in second child:
    for(int i = 0; i < capacity;  i++) {
        if(p2->variables[i]==elem1) {
            pos1 = i;
            break;
        }
    }
    for(int i = 0; i < capacity;  i++) {
        if(p2->variables[i]==elem2) {
            pos2 = i;
            break;
        }
    }

    // Swap selected element in second child
    ch2->variables[pos1] = elem2;
    ch2->variables[pos2] = elem1;

	FitnessP chFitness, ch2Fitness;
	// evaluiraj dijete uz ch
	state_->getAlgorithm()->evaluate(myInd);
	chFitness = myInd->fitness;

	// umetni ch2 u jedinku dijete
	myInd->at(gId) = (GenotypeP) ch2;
	// 'rucno' smo promijenili jedinku, pa to oznaci (za svaki slucaj...)
	myInd->fitness->setInvalid();
	// evaluiraj dijete uz ch2
	state_->getAlgorithm()->evaluate(myInd);
	ch2Fitness = myInd->fitness;

	// ako je ch bio bolji, trebamo ga vratiti
	if(chFitness->isBetterThan(ch2Fitness)) {
		myInd->at(gId) = (GenotypeP) ch;
		// ne zaboravimo fitness! (da se ne evaluira ponovo)
		myInd->fitness = chFitness;
	}

/*
	Permutation* ch = (Permutation*) (child.get());
    Permutation* ch2 = ch->copy();

    IndividualP ind =
    state_->getAlgorithm()->evaluate()


	int start = state_->getRandomizer()->getRandomInteger(p1->getSize());
	int end = state_->getRandomizer()->getRandomInteger(start, p1->getSize() - 1);

	std::map<int, int> subSet;

	int ind1 = state_->getRandomizer()->getRandomInteger(p1->getSize() - 1);
	int ind2 = state_->getRandomizer()->getRandomInteger(ind1, p1->getSize() - 1);

	// copy subvector into child
	for(int i = ind1; i <= ind2; i++) {
		ch->variables[i] = p1->variables[i];
		subSet[p1->variables[i]] = 0;
	}
	int indexChild = (ind2 + 1) % p1->getSize();
	std::map<int, int>::iterator iter = subSet.begin();

	// krecemo od indeksa za jedan vise od indeksa zadnjeg elementa podniza
	// i kopiramo iz drugog roditelja u dijete
	// ako naidjemo na element koji se vec nalazi u podnizu idemo dalje
	// postupak se zaustavlja kada popunimo cijelo dijete
	do {
		ind2 = (++ind2) % p1->getSize();
		if(subSet.find(p2->variables[ind2]) == subSet.end()) {
			ch->variables[indexChild] = p2->variables[ind2];
			indexChild = (++indexChild) % p1->getSize();
		}
	} while(indexChild != ind1);
*/
	return true;
}

}
