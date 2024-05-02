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


std::vector<std::vector<int>> vectors(int n,int d) {
	std::vector<int> vec(n+1);
	vec[0]=d;
	int N=binomial(n+d,d);
	std::vector<std::vector<int>> output;
	for (int i=0;i<N;++i) {
		output.push_back(vec);
		if (i!=N-1) {
			vec=next_vector(vec,d);
		}
	}
	return output;
}




std::vector<std::vector<std::vector<int>>> sum_matrix(int n,int d) {
	std::vector<int> vec_i(n+1);
	std::vector<int> vec_j(n+1);
	vec_i[0]=d;
	vec_j[0]=d;
	int N=binomial(n+d,d);
	std::vector<std::vector<std::vector<int>>> sum(N, std::vector<std::vector<int>>(N, std::vector<int>(n)));
	for (int i=0;i<N;++i) {
		for (int j=i;j<N;++j) {
			sum[i][j]=add(vec_i,vec_j);
			if (j==N-1 && i!=N-1) {
				vec_j=next_vector(vec_i,d);
			}
			else if (i!=N-1) {
				vec_j=next_vector(vec_j,d);
			}
		}
		if (i!=N-1) {
			vec_i=next_vector(vec_i,d);
		}
	}

	return sum;
}

std::vector<std::vector<std::vector<int>>> tuples(int n, int d) {
	std::vector<int> iterator_vec=sum_matrix(n,d)[0][0];
	std::vector<int> zero{};
	std::vector<std::vector<int>> current_tuple;
	std::vector<std::vector<std::vector<int>>> output;
	int N=binomial(n+d,d);
	std::vector<std::vector<std::vector<int>>> sum=sum_matrix(n,d);
	for (int i=0;i<N;++i) {
		for (int j=i;j<N;++j) {
			current_tuple={};
			if (sum[i][j]==zero) {
				continue;
			}
			else {
				iterator_vec=sum[i][j];
			}
			for (int k=0;k<N;++k) {
				for (int l=0;l<N;++l) {
					// std::cout<<i<<" "<<j<<" "<<k<<" "<<l<<"\n";
					if (sum[k][l]==iterator_vec && (i!=k || j!=l)) {
						current_tuple.push_back({k,l});
						sum[k][l]=zero;
					}
				}
			}
			sum[i][j]=zero;
			if (current_tuple.size()!=0) {
				current_tuple.push_back({i,j});
				output.push_back(current_tuple);
			}
		}
	}
	return output;
}


int main() {

int n=2;
int d=2;

//std::cout<<"\ntuples with sums that appear at least twice: \n"<<tuples(n,d)<<"\n";

//std::cout<<"\n"<<(tuples(n,d)).size()<<(tuples(n,d).size()==1 ? " sum appears" : " sums appear")<<" at least twice";



for (int D=0;D<5;++D) {
	for (int N=0;N<5;++N) {
		std::cout<<(tuples(N,D)).size()<<" sums appear least twice for (n,d) = "<<"("<<N<<","<<D<<")\n";
	}
}


/*
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
*/

}
