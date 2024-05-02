#include<iostream>
#include<vector>
#include<sstream>

int fac(int m) {
	if (m==0) {
		return 1;
	}
	return fac(m-1)*m;
}

int find_index_to_change(std::vector<int> input) {
	int current=0;
	for(auto i=0;i<input.size();++i) {
		if (input[i]!=0 && i!=input.size()-1) {
			current=i;
		}
	}
	return current;
}

bool all_before_index_zero(std::vector<int> input, int index) {
	for(auto i=0;i<index;++i) {
		if (input[i]!=0) {
			return false;
		}
	}
	return true;
}



int binomial(int n,int r) {
	int num=fac(n);
	int denom=fac(n-r)*fac(r);
	return num/denom;
}

std::vector<int> add(std::vector<int> v1, std::vector<int> v2) {
	if (v1.size()!=v2.size()) {
		throw std::length_error("not same length");
	}
	std::vector<int> sum=v1;
	for (auto i=0;i<v2.size();++i) {
		sum[i]+=v2[i];
	}
	return sum;
}

std::vector<int> next_vector(std::vector<int>& input, int d) {
	int n=input.size();
	int sum=0;
	for (auto i=0;i<n;++i) {
		sum+=input[i];
	}
	std::vector<int> output=input;
	int index=find_index_to_change(input);
	if (sum>d) {
		throw std::invalid_argument("vector sum too big");
	}
	if (input[n-1]==d) {
		std::cout<<"no next vector\n";
		return output;
	}
	if (all_before_index_zero(input,index)==1 && input[index]==1) {
		std::fill(output.begin(), output.end(), 0);
		output[index+1]=d;
		return output;
	}
	if (input[n-1]!=0 && index!=n-2) {
		output[index+1]=input[n-1]+1;
		output[index]-=1;
		output[n-1]=0;
		return output;
	}
	output[index]-=1;
	output[index+1]+=1;
	return output;
}




template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& xs) {
    os << "{";
    for (std::size_t j{0}; j < xs.size(); ++j) {
        os << (j ? ", " : "") << xs[j];
    }
    return os << "}";
}



int main() {

int n=1;
int d=8;

// vec is the standard starting vector, i.e. the biggest vector with sum of d.
std::vector<int> vec(n+1);
vec[0]=d;

// N is the number of possible derivations from vec, i.e. how many vectors there are which have sum of d
int N=binomial(n+d,d);

// some vectors for iteration
std::vector<int> iterator_vector_i=vec;
std::vector<int> iterator_vector_j=vec;

std::vector<int> iterator_for_vector_list=vec;
std::vector<std::vector<int>> vectors;
for (int i=0;i<N;++i) {
	vectors.push_back(iterator_for_vector_list);
	if (i!=N-1) {
		iterator_for_vector_list=next_vector(iterator_for_vector_list,d);
	}
}

std::vector<int> sum_matrix[N][N];
for (int i=0;i<N;++i) {
	for (int j=i;j<N;++j) {
		// std::cout<<"\n it_i is: "<<iterator_vector_i<<"\n it_j is"<<iterator_vector_j<<"\n";
		sum_matrix[i][j]=add(iterator_vector_i,iterator_vector_j);
		if (j==N-1 && i!=N-1) {
			iterator_vector_j=next_vector(iterator_vector_i,d);
		}
		else if (i!=N-1) {
			iterator_vector_j=next_vector(iterator_vector_j,d);
		}
	}
	if (i!=N-1) {
		iterator_vector_i=next_vector(iterator_vector_i,d);
	}
}

std::vector<std::vector<std::vector<int>>> tuples;
std::vector<int> zero{};

std::vector<std::vector<int>> current_tuple;
std::vector<int> iterator_vec=sum_matrix[0][0];
for (int i=0;i<N;++i) {
	for (int j=i;j<N;++j) {
		current_tuple={};
		if (sum_matrix[i][j]==zero) {
			continue;
		}
		else {
			iterator_vec=sum_matrix[i][j];
		}
		for (int k=0;k<N;++k) {
			for (int l=0;l<N;++l) {
				// std::cout<<i<<" "<<j<<" "<<k<<" "<<l<<"\n";
				if (sum_matrix[k][l]==iterator_vec && (i!=k || j!=l)) {
					current_tuple.push_back({k,l});
					sum_matrix[k][l]=zero;
				}
			}
		}
		sum_matrix[i][j]=zero;

		if (current_tuple.size()!=0) {
			current_tuple.push_back({i,j});
			tuples.push_back(current_tuple);
		}
	}
}

std::cout<<"\ntuples with sums that appear at least twice: \n"<<tuples<<"\n";

std::cout<<"\n"<<tuples.size()<<(tuples.size()==1 ? " sum appears" : " sums appear")<<" at least twice";

std::cout<<"\ncorresponding to the following additions:" ;
std::vector<int> iterate_tuple;
for (int r=0;r<tuples.size();++r) {
	std::vector<std::vector<int>> tuple_r=tuples[r];
	std::stringstream s;
	for (int m=0;m<tuple_r.size();++m) {
		iterate_tuple=tuple_r[m];
		s<<vectors[iterate_tuple[0]]<<" + "<<vectors[iterate_tuple[1]]<<((m==tuple_r.size()-1) ? "" : " = ");
	}
	std::cout<<"\n"<<s.str();
}


}
