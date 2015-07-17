
#include "mat.hpp"
#include "fifth_polynomial.hpp"

/*
A =
    1    1    1
    3    4    5
    6   12   20
inv(A)=
    10.00000   -4.00000    0.50000
  -15.00000    7.00000   -1.00000
    6.00000   -3.00000    0.50000
 */
void fifth_polynomial::Init(){
	if(f_init){
		//std::cout << "nothing to do" << std::endl;
		return;
	}
	f_init = true;
	
	ublas::vector<double> b(3);
	ublas::vector<double> ans(3);
	ublas::matrix<double> A(3,3);
	ublas::matrix<double> A_inv(3,3);
	
	this->td = 1 / (this->t_end - this->t_start);
	this->td2 = td * td;
	this->a0 = 0;
	this->a1 = xi_dot / (xf - xi) / td;
	this->a2 = xi_ddot / (xf - xi) / td2;
	
	// std::cout << td << std::endl;
	
	b[0] = 1 - (this->a0 + this->a1 + this->a2);
	b[1] = xf_dot / (xf - xi) - (this->a1 * td + 2 * this->a2 * td);
	b[2] = xf_ddot / (xf - xi) - (2 * this->a2 * td2);
	
	A(0,0) = 1; A(0,1) = 1; A(0,2) = 1;
	A(1,0) = 3 * td; A(1,1) = 4 * td; A(1,2) = 5 * td;
	A(2,0) = 6 * td2; A(2,1) = 12 * td2; A(2,2) = 20 * td2;
	
	MatInv(A_inv, A);
	
// 	std::cout << "b = " << b << std::endl;
// 	std::cout << "A = " << A << std::endl;
// 	std::cout << "A_inv = " << A_inv << std::endl;
	
	ans = prod(A_inv, b);
	
	
// 	std::cout << "ans = " << ans << std::endl;
	
	this->a3 = ans[0];
	this->a4 = ans[1];
	this->a5 = ans[2];
	
}

double FifthPolynomial(double t, fifth_polynomial &data);
double FifthPolynomial_dot(double t, fifth_polynomial &data);
double FifthPolynomial_ddot(double t, fifth_polynomial &data);


void FifthPolynomials(std::vector<fifth_polynomial> data_org,
			std::vector<xydata> &data,
			std::vector<xydata> &data_dot,
			std::vector<xydata> &data_ddot){
	data.resize(0);
	data_dot.resize(0);
	data_ddot.resize(0);
	int i = 0;
	for(double t=0;t<SIM_TIME;t+=STEP){
		if(data_org[i].t_end<=t){
			i++;
			if(data_org.size()<=i){
				i--;
				data.push_back(xydata(
					{t,
					data_org[i].xf
					}));
				data_dot.push_back(xydata(
					{t,
					0.0
					}));
				data_ddot.push_back(xydata(
					{t,
					0.0
					}));
				continue;
			}
		}
		
		data.push_back(xydata(
		{t, FifthPolynomial(t, data_org[i])}));
		
		data_dot.push_back(xydata(
		{t, FifthPolynomial_dot(t, data_org[i])}));
		
		data_ddot.push_back(xydata(
		{t, FifthPolynomial_ddot(t, data_org[i])}));
	}
}

double FifthPolynomial(double t, fifth_polynomial &data){
	double t_ = (t-data.t_start)/(data.t_end - data.t_start);
	data.Init();
	return data.xi + (data.xf - data.xi) *
		(data.a0 + data.a1 * t_+ data.a2 * pow(t_, 2)
		 + data.a3 * pow(t_, 3) + data.a4 * pow(t_, 4)
		 + data.a5 * pow(t_, 5));
}
double FifthPolynomial_dot(double t, fifth_polynomial &data){
	double t_ = (t-data.t_start)/(data.t_end - data.t_start);
	data.Init();
	return (data.xf - data.xi) *
		(data.a1 * data.td + 2 * data.a2 * t_ * data.td +
			3 * data.a3 * pow(t_, 2) * data.td +
			4 * data.a4 * pow(t_, 3) * data.td + 
			5 * data.a5 * pow(t_, 4) * data.td);
}
double FifthPolynomial_ddot(double t, fifth_polynomial &data){
	double t_ = (t-data.t_start)/(data.t_end - data.t_start);
	data.Init();
	return (data.xf - data.xi) *
		(2 * data.a2 * data.td2 + 6 * data.a3 * t_ * data.td2 + 
		12 * data.a4 * pow(t_, 2) * data.td2 + 
		20 * data.a5 * pow(t_, 3) * data.td2);
}
