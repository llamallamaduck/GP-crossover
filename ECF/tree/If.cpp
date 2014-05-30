#include "../ECF_base.h"
#include "If.h"
#include <cmath>

namespace Tree {
namespace Primitives {

If::If(void)
{
	nArguments_ = 3;
	name_ = "IF";
}


If::~If(void)
{	}


void If::execute(void* result, Tree &tree)
{
	std::vector<bool>& res = *(std::vector<bool>*)result;
		uint size = (uint) res.size();

		std::vector<bool> arg(size), res1(size), res2(size);
		getNextArgument(&arg, tree);
		getNextArgument(&res1, tree);
		getNextArgument(&res2, tree);

		for(uint i = 0; i < size; i++)
			if(arg[i]) {
				res[i] = res1[i];
			} else {
				res[i] = res2[i];
			}
}

}
}