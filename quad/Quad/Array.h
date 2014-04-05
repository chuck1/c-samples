
template <typename T> class Array {
	public:
		void alloc(int n) {
			if(v_ != NULL) {
				delete[] v_;
			}
			
			v_ = new T[n];
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


