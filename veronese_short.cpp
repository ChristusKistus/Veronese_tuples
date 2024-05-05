#include<iostream>
#include<vector>
#include<sstream>
#include<algorithm>

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
	if (r==0) {
		return 1;
	}
	return (binomial(n,r-1)*(n-r+1))/(r);
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
	if (sum!=d) {
		throw std::invalid_argument("vector sum incorrect");
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


std::vector<std::vector<short>> vectors(short n,short d) {
	std::vector<short> vec(n+1);
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
	std::vector<std::vector<std::vector<short>>> sum=sum_matrix(n,d);
	std::vector<short> iterator_vec;
	std::vector<std::vector<short>> current_tuple;
	std::vector<std::vector<std::vector<short>>> output;
	int N=binomial(n+d,d);
	for (short i=0;i<N;++i) {
		for (short j=i;j<N;++j) {
			current_tuple={};
			if (sum[i][j].size()==0) {
				continue;
			}
			else {
				iterator_vec=sum[i][j];
			}
			for (short k=i;k<N;++k) {
				for (short l=k;l<N;++l) {
					// std::cout<<i<<" "<<j<<" "<<k<<" "<<l<<"\n";
					if (sum[k][l]==iterator_vec && (i!=k || j!=l)) {
						current_tuple.push_back({k,l});
						sum[k][l]={};
					}
				}
			}
			sum[i][j]={};
			if (current_tuple.size()!=0) {
				current_tuple.push_back({i,j});
				output.push_back(current_tuple);
			}
		}
	}
	return output;
}

std::string pad(std::string s, int n) {
	if (n==0) {
		return s;
	}
	for (int i=0;i<n;++i) {
		s+=" ";
	}
	return s;
}

std::string pad_center(std::string s, int n) {
	if (n==0) {
		return s;
	}
	for (int i=0;i<n/2;++i) {
		s=" "+s;
	}
 	for (int i=0;i<(n-n/2);++i) {
		s=s+" ";
	}
	return s;
}



int matrix_max_length(std::vector<std::vector<int>> in) {
	int max=3;
	for (int i=0;i<in.size();++i) {
		for (int j=0;j<in[0].size();++j) {
			if (std::to_string(in[i][j]).length()>max) {
				max=std::to_string(in[i][j]).length();
			}
		}
	}
	return max;
}

void print_matrix(std::vector<std::vector<int>> in, char x, char y) {
	int max_length=matrix_max_length(in);
	std::cout<<"\n    ┃";
	for (int k=0;k<in[0].size();++k) {
		if (k==0) {
			std::cout<<x<<"=0"<<pad("",max_length-3);
			continue;	
		}
		std::cout<<"┃"<<pad_center(std::to_string(k),max_length-(std::to_string(k)).length());
	}
	std::cout<<"\n━━━━";
	for (int k=0;k<in[0].size();++k) {
		std::cout<<"╋";
		for (int l=0;l<max_length;++l) {
			std::cout<<"━";
		}
	}
	std::cout<<"\n";
	for (int i=0;i<in.size();++i) {
		for (int j=0;j<in[i].size();++j) {
			if (i==0 && j==0) {
				std::cout<<y<<"=0 ";
			}
			if (i!=0 && j==0) {
				std::cout<<" "<<i<<"  ";
				//std::cout<<" "<<pad(std::to_string(i),max_length-(std::to_string(i)).length());
			}
			std::cout<<"┃"<<pad(std::to_string(in[i][j]),max_length-(std::to_string(in[i][j])).length());
		}
		std::cout<<"\n━━━━";
		for (int k=0;k<in[0].size();++k) {
			std::cout<<(i!=in.size()-1 ? "╋" : "┻");
			for (int l=0;l<max_length;++l) {
				std::cout<<"━";
			}
		}
		std::cout<<"\n";
	}
}




int main() {

short n=4;
short d=3;

auto output=tuples(n,d);
auto vector_output=vectors(n,d);

//std::cout<<"\ntuples with sums that appear at least twice: \n"<<output<<"\n";

std::cout<<"\n"<<(output).size()<<(output.size()==1 ? " sum appears" : " sums appear")<<" at least twice";


unsigned long eq_counter=0;
for (int r=0;r<output.size();++r) {
	std::vector<std::vector<short>> tuple_r=output[r];
	eq_counter+=((tuple_r.size()-1)*(tuple_r.size()))/2;
}
std::cout<<"\nthese sums create a total number of "<<eq_counter<<" equations.";






/*
std::cout<<"\ncorresponding to the following additions:" ;
std::vector<short> iterate_tuple;
for (int r=0;r<output.size();++r) {
	std::vector<std::vector<short>> tuple_r=output[r];
	std::stringstream s;
	for (short m=0;m<tuple_r.size();++m) {
		iterate_tuple=tuple_r[m];
		s<<vector_output[iterate_tuple[0]]<<" + "<<vector_output[iterate_tuple[1]]<<((m==tuple_r.size()-1) ? "" : " = ");
	}
	std::cout<<"\n"<<s.str();
}
*/


//output how many sums appear exactly 0,1,2,3... times
int max_tup_length;
for (int r=0;r<output.size();++r) {
	if (max_tup_length<output[r].size()) {
		max_tup_length=output[r].size();
	}
}
std::cout<<max_tup_length;
std::vector<short> iterate_tuple3;
std::vector<int> display_vec(max_tup_length+1,0);
for (int i=0;i<max_tup_length+1;++i) {
	display_vec[i]=i;
}
int size_r=0;
std::vector<int> multiplicity(max_tup_length+1,0);
for (int r=0;r<output.size();++r) {
	size_r=output[r].size();
	multiplicity[size_r]+=1;
}
std::cout<<"\nmultiplicity of sums ot \n"<<display_vec<<"\n are (excluding those that appear ocne):\n"<<multiplicity<<"\n";









/*
// output number of equations with sums of vectors v,w with v0+w0 = (something between 0 and 2d)
std::vector<short> iterate_tuple2;
std::vector<int> sum_at_0_count(2*d,0);
std::vector<int> display_vec(2*d,0);
for (int r=0;r<2*d;++r) {
	display_vec[r]=r;
}
int current_0_sum;
for (int r=0;r<output.size();++r) {
	std::vector<std::vector<short>> tuple_r=output[r];
	iterate_tuple2=tuple_r[0];
	current_0_sum=(vector_output[iterate_tuple2[0]])[0]+(vector_output[iterate_tuple2[1]])[0];
	sum_at_0_count[current_0_sum]+=binomial(tuple_r.size(),2);
}
std::cout<<"\nnumber of equations with sum of \n"<<display_vec<<"\n at place 0:\n"<<sum_at_0_count<<"\n";
*/







//output matrix that shows the number of equations for n,d
/*
int dim_n=6;
int dim_d=6;
unsigned long count=0;
std::vector<std::vector<short>> it_tup;
std::vector<std::vector<int>> n_d_matrix(dim_n, std::vector<int>(dim_d,0));
for (int i=0;i<dim_n;++i) {
	for (int j=0;j<dim_d;++j) {
		count=0;
		auto tup=tuples(i,j);
		for (int r=0;r<tup.size();++r) {
			it_tup=tup[r];
			count+=((it_tup.size()-1)*(it_tup.size()))/2;
		}
		n_d_matrix[i][j]=count;
	}
}
std::cout<<"\nmatrix of number of equations(i dont know the formula): \n";
print_matrix(n_d_matrix,'d','n');
std::cout<<"\nWHAT THE FUCK IS THE FORMULA TELL ME AHHHHHHHHHHHHHHHHHH";
*/






//output the matrix of the number of non-unique sums for n,d
/*
int dim_x=6;
int dim_y=6;
std::vector<std::vector<int>> x_y_matrix(dim_x, std::vector<int>(dim_y,0));
for (int i=0;i<dim_x;++i) {
	for (int j=0;j<dim_y;++j) {
		x_y_matrix[i][j]=binomial(2*j+i,i)-(i+1)*(i+1);
	}
}
std::cout<<"\n\nmatrix of number of non-unique sums(using formula): \n";
print_matrix(x_y_matrix,'d','n');
*/





//output a test matrix
/*
int dim_x2=6;
int dim_y2=6;
std::vector<std::vector<int>> x2_y2_matrix(dim_x2, std::vector<int>(dim_y2,0));
for (int i=0;i<dim_x2;++i) {
	for (int j=0;j<dim_y2;++j) {
		x2_y2_matrix[i][j]=binomial(binomial(i+1,2)+1,2);
	}
}
std::cout<<"\n\nmatrix: test function\n";
print_matrix(x2_y2_matrix,'d','n');

*/




}
