#ifndef Or_h
#define Or_h
#include "Primitive.h"

namespace Tree {
	namespace Primitives {

		/**
		* \ingroup tree genotypes primitives
		* \brief Or function primitive (Tree genotype)
		*/
		template <class T>
		class OrT : public Primitive
		{
		public:
			OrT(void);
			void execute(void* result, Tree& tree);
			~OrT(void);
		};
		typedef OrT<bool> Or;

		template <class T>
		OrT<T>::~OrT(void)
		{	}

		template <class T>
		OrT<T>::OrT(void)
		{
			nArguments_ = 2;
			name_ = "OR";
		}

		template <class T>
		void OrT<T>::execute(void* result, Tree& tree)
		{
			std::vector<bool>& or = *(std::vector<bool>*)result;
			uint size = (uint)or.size();
					
			std::vector<bool> arg1(size), arg2(size);

			getNextArgument(&arg1, tree);
			getNextArgument(&arg2, tree);

			for (uint i = 0; i < size; i++)
				or[i] = arg1[i] || arg2[i];
		}
	}
}

#endif