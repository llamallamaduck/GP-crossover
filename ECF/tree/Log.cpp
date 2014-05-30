#include "../ECF_base.h"
#include "Log.h"
#include <cmath>

namespace Tree {
namespace Primitives {

	Log::Log(void)
	{
		nArguments_ = 1;
		name_ = "log";
	}

	void Log::execute(void* result, Tree& tree)
	{
		double& arg = *(double*)result;
	    getNextArgument(&arg, tree);

		if(arg > 1)
			arg = log10(arg);
		else
			arg = 0;
	}
	
	Log::~Log()
	{	}
}
}
