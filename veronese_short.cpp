#include<iostream>
#include<vector>
#include<sstream>

int fac(int m) {
	if (m==0) {
		return 1;
	}
	return fac(m-1)*m;
}

short find_index_to_change(std::vector<short> input) {
	short current=0;
	for(auto i=0;i<input.size();++i) {
		if (input[i]!=0 && i!=input.size()-1) {
			current=i;
		}
	}
	return current;
}

bool all_before_index_zero(std::vector<short> input, short index) {
	for(auto i=0;i<index;++i) {
		if (input[i]!=0) {
			return false;
		}
	}
	return true;
}



int binomial(short n,short r) {
	int num=fac(n);
	int denom=fac(n-r)*fac(r);
	return num/denom;
}

std::vector<short> add(std::vector<short> v1, std::vector<short> v2) {
	if (v1.size()!=v2.size()) {
		throw std::length_error("not same length");
	}
	std::vector<short> sum=v1;
	for (auto i=0;i<v2.size();++i) {
		sum[i]+=v2[i];
	}
	return sum;
}

std::vector<short> next_vector(std::vector<short>& input, short d) {
	int n=input.size();
	int sum=0;
	for (auto i=0;i<n;++i) {
		sum+=input[i];
	}
	std::vector<short> output=input;
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

/*
std::vector<std::vector<short>> vectors(short n,short d) {
	std::vector<int> vec(n+1);
	vec[0]=d;
	int N=binomial(n+d,d);
	std::vector<std::vector<short>> output;
	for (int i=0;i<N;++i) {
		output.push_back(vec);
		if (i!=N-1) {
			vec=next_vector(vec,d);
		}
	}
	return output;
}
*/



std::vector<std::vector<std::vector<short>>> sum_matrix(short n,short d) {
	std::vector<short> vec_i(n+1);
	std::vector<short> vec_j(n+1);
	vec_i[0]=d;
	vec_j[0]=d;
	int N=binomial(n+d,d);
	std::vector<std::vector<std::vector<short>>> sum(N, std::vector<std::vector<short>>(N, std::vector<short>(n)));
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

std::vector<std::vector<std::vector<short>>> tuples(short n, short d) {
	std::vector<short> iterator_vec=sum_matrix(n,d)[0][0];
	std::vector<short> zero{};
	std::vector<std::vector<short>> current_tuple;
	std::vector<std::vector<std::vector<short>>> output;
	int N=binomial(n+d,d);
	std::vector<std::vector<std::vector<short>>> sum=sum_matrix(n,d);
	for (short i=0;i<N;++i) {
		for (short j=i;j<N;++j) {
			current_tuple={};
			if (sum[i][j]==zero) {
				continue;
			}
			else {
				iterator_vec=sum[i][j];
			}
			for (short k=i;k<N;++k) {
				for (short l=i+1;l<N;++l) {
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

short n=5;
short d=6;

//std::cout<<"\ntuples with sums that appear at least twice: \n"<<tuples(n,d)<<"\n";

std::cout<<"\n"<<(tuples(n,d)).size()<<(tuples(n,d).size()==1 ? " sum appears" : " sums appear")<<" at least twice";


/*
for (int D=0;D<6;++D) {
	for (int N=0;N<6;++N) {
		std::cout<<(tuples(N,D)).size()<<" sums appear least twice for (n,d) = "<<"("<<N<<","<<D<<")\n";
	}
}
*/

/*
std::cout<<"\ncorresponding to the following additions:" ;
std::vector<int> iterate_tuple;
for (int r=0;r<tuples.size();++r) {
	std::vector<std::vector<int>> tuple_r=tuples[r];
	std::stringstream s;
	for (short m=0;m<tuple_r.size();++m) {
		iterate_tuple=tuple_r[m];
		s<<vectors[iterate_tuple[0]]<<" + "<<vectors[iterate_tuple[1]]<<((m==tuple_r.size()-1) ? "" : " = ");
	}
	std::cout<<"\n"<<s.str();
}
*/

}
