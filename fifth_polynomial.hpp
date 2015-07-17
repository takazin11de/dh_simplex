#include "user_std.hpp"
#include "common_data.hpp"

#ifndef FIFTH_POLYNOMIAL_HPP
#define FIFTH_POLYNOMIAL_HPP

class fifth_polynomial{
public:
	fifth_polynomial() : f_init(false){}
	void Init();
	
	double xi;
	double xi_dot;
	double xi_ddot;
	double xf;
	double xf_dot;
	double xf_ddot;
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;
	double t_start;
	double t_end;
	double td;
	double td2;
	
private:
	bool f_init;

	
	
};

void FifthPolynomials(std::vector<fifth_polynomial> data_org,
			std::vector<xydata> &data,
			std::vector<xydata> &data_dot,
			std::vector<xydata> &data_ddot);

#endif