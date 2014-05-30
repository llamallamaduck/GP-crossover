#ifndef If_h
#define If_h
#include "Primitive.h"

namespace Tree {
namespace Primitives {

/**
 * \ingroup tree genotypes primitives
 * \brief Cos function primitive (Tree genotype)
 */
class If : public Primitive
{
public:
	If(void);
	void execute(void* result, Tree& tree);
	~If(void);
};

}
}

#endif 