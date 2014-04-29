#ifndef And_h
#define And_h
#include "Primitive.h"

#include <algorithm>

namespace Tree {
	namespace Primitives {
		/**
		* \ingroup tree genotypes primitives
		* \brief Or function primitive (Tree genotype)
		*/
		template <class T>
		class AndT : public Primitive
		{
		public:
			AndT(void);
			void execute(void* result, Tree& tree);
			~AndT(void);
		};
		typedef AndT<bool> And;


		template <class T>
		AndT<T>::AndT(void)
		{
			nArguments_ = 2;
			name_ = "AND";
		}

		template <class T>
		void AndT<T>::execute(void* result, Tree::Tree& tree)
		{
			std::vector<bool>& and = *(std::vector<bool>*)result;
			uint size = (uint)and.size();

			std::vector<bool> arg1(size), arg2(size);

			getNextArgument(&arg1, tree);
			getNextArgument(&arg2, tree);

			for (uint i = 0; i < size; i++)
				and[i] = arg1[i] && arg2[i];
		}

		template <class T>
		AndT<T>::~AndT(void)
		{	};
}
#endif