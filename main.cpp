#include <cstdlib>
#include <ctime>
#include <ostream>
#include <iterator>
#include <algorithm>

std::size_t random_index(std::size_t first, std::size_t last)
{
	double r= std::rand()* 1./ RAND_MAX;
	std::size_t index= first+ r* (last- first);
	return index;
}

template <std::size_t N>
std::ostream &operator<< (std::ostream &out, std::size_t const (&a)[N])
{
	std::ostream_iterator<std::size_t> it(out<< "[", ", ");
	std::copy(a, a+ N, it);
	return out<< "\b\b]\n";
}

template <std::size_t N>
std::ostream &operator<< (std::ostream &out, double const (&a)[N])
{
	std::ostream_iterator<double> it(out<< "[", ", ");
	std::copy(a, a+ N, it);
	return out<< "\b\b]\n";
}

template <std::size_t N>
struct Wheel
{
	double mutation;
	double P[N];
	double selector[N];
	std::size_t down_mutator[N];
	std::size_t down_mutator_size;

	Wheel(double mutation= 1./ N/ 2)
	: mutation(mutation)
	{
		std::fill(P, P+ N, 1./ N);
		configure_selector();
		configure_down_mutator();
	}
	
	std::ostream &print(std::ostream &out) const
	{
		return out<< "P: "<< P
			<< "selector: "<< selector
			<< "mutator: "<< down_mutator
			<< "mutator size: "<< down_mutator_size<< '\n';
	}

	void configure_selector()
	{
		double sum= 0;
		for(std::size_t n= 0; n< N; ++ n)
			selector[n]= sum+= P[n];
	}
	
	void configure_down_mutator()
	{
		std::size_t size= 0;
		for(std::size_t n= 0; n< N; ++ n)
			if(P[n]>= mutation)
			{
				down_mutator[size]= n;
				++ size;
			}
		down_mutator_size= size;
	}

	std::size_t select_index() const
	{
		double r= std::rand()* 1./ RAND_MAX;
		for(std::size_t n= 0; n< N; ++ n)
			if(r<= selector[n])
				return n;
		return N;
	}

	void mutate_down()
	{
		std::size_t mutate_index= random_index(0, down_mutator_size);
		P[down_mutator[mutate_index]]-= mutation;
		double sum_compensate= 0;
		for(std::size_t n= 0; n< N; ++ n)
			if(n!= mutate_index)
				sum_compensate+= 1- P[n];
		for(std::size_t n= 0; n< N; ++ n)
			if(n!= mutate_index)
				P[n]+= (1- P[n])* mutation/ sum_compensate;

		configure_down_mutator();
		configure_selector();
	}
};

#include <sstream>
#include <stack>

double rpn_eval(std::string const &expr, bool &ok)
{
	static std::istringstream in;
	in.clear();
	in.str(expr);
	static std::stack<double> stack;
	stack= std::stack<double>();
	
	for(std::string token; in>> token;)
	{
		static std::stringstream convert;
		convert.clear();
		convert.str(token);
	}

	return 42;
}

#include <iostream>

struct RpnGene: Wheel<13>
{
	void mutate()
	{
		mutate_down();
	}

	std::string select() const
	{
		static std::string ops[]= { "1", "2", "3", "4", "5", "6", "7", "8", "9", "+", "-", "*", "/" };
		return ops[select_index()];
	}
};

template <std::size_t N>
struct RpnDna
{
	RpnGene genes[N];

	void mutate()
	{
		std::size_t mutate_gene_index= random_index(0, N);
		genes[mutate_gene_index].mutate();
	}

	void fitness() const
	{
		
	}
};

int main()
{
	std::srand(unsigned(std::time(0)));
	bool ok= true;
	std::cout<< rpn_eval("1 1 +", ok)<< '\n';
}

