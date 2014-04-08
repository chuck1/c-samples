#ifndef __ARRAY__
#define __ARRAY__

#include <cstdlib>
#include <cstdio>

template <typename T> class Array {
	public:
		Array() {
			n_ = 0;
			v_ = NULL;
		}
		void alloc(int n) {
			if(n <= 1) {
				printf("cant alloc %i\n", n);
				throw;
			}
			
			printf("allocating %i\n", n);
			
			if(v_ != NULL) {
				delete[] v_;
			}
			
			v_ = new T[n];
			n_ = n;
		}
		T& operator[](int i) {
			if(v_ == NULL) {
				printf("empty array\n");
				throw;
			}
			
			i = (i + n_) % n_;
			
			if(i >= n_) {
				printf("out of bounds\n");
				throw;
			}
			
			return v_[i];
		}
		
	public:
		int	n_;
		T*	v_;
};


#endif


