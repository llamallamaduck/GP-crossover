#ifndef Not_h
#define Not_h
#include "Primitive.h"

#include <algorithm>

namespace Tree {
	namespace Primitives {
		/**
		* \ingroup tree genotypes primitives
		* \brief Or function primitive (Tree genotype)
		*/
		template <class T>
		class NotT : public Primitive
		{
		public:
			NotT(void);
			void execute(void* result, Tree& tree);
			~NotT(void);
		};
		typedef NotT<bool> Not;


		template <class T>
		NotT<T>::NotT(void)
		{
			nArguments_ = 1;
			name_ = "NOT";
		}

		template <class T>
		void NotT<T>:: execute(void* result, Tree& tree)
		{
			std::vector<bool>& not = *(std::vector<bool>*)result;
			uint size = (uint) not.size();

			std::vector<bool> arg1(size);
			getNextArgument(&arg1, tree);

			for(uint i = 0; i < size; i++)
			not[i] = !arg1[i];
		}

		template <class T>
		NotT<T>::~NotT(void)
		{	}
	}
}
#endif