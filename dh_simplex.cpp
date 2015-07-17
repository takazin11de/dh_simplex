/*
 * dh_simplex.cpp
 * 
 * Copyright (c) 2014 takako takahito
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 */

#include "dh_simplex.hpp"


namespace dh_simplex{

	void DhSimplex::Init(std::vector<Simplex> &_simplexes,p_eval_func _eval_func, int n_){
		
		simplexes=_simplexes;
		
		eval_func=_eval_func;
	
		k_alpha = 1.0;
		k_beta = 0.5;
		k_gamma = 2.0;
		
		n=n_;
		
		s_g.resize(n);
		s_r.resize(n);
		s_e.resize(n);
		s_c.resize(n);
		
		s_g.clear();
		s_r.clear();
		s_e.clear();
		s_c.clear();
		
		Rank();
		std::cout << "A" << std::endl;
		std::cout << std::flush;
		Gravity();
		std::cout << "B" << std::endl;
		std::cout << std::flush;
	}

	void DhSimplex::Do(){
		double value1,value2;
		s_r.clear();
		s_e.clear();
		s_c.clear();
		//ラムダ式で各々の操作を記述したが，メンバ変数でもよかった．
		auto Refrection = [&]()->double{s_r = (1.0 + k_alpha)* s_g - 
				k_alpha * simplexes[s_max1.id];
				return (*eval_func)(s_r);};
		auto Expansion = [&]()->double{s_e = k_gamma * s_r +(1.0 - k_gamma)* s_g;
				return (*eval_func)(s_e);};
		auto Contraction = [&]()->double{s_c = k_beta * simplexes[s_max1.id] +
				(1.0 - k_beta)* s_g;
				return (*eval_func)(s_c);};
		auto Reduction = [&](){int i=0;
				for(auto it = simplexes.begin(); it != simplexes.end(); ++i, ++it){
					(*it) = 0.5 *(simplexes[s_min.id] + (*it));} };
	
		value1 = Refrection();
	
		if(value1 < s_min.value){
			value2 = Expansion();	
			if(value2 < value1){
				simplexes[s_max1.id] = s_e;
				Rank();
				Gravity();
				return;
			}else{
				simplexes[s_max1.id] = s_r;
				Rank();
				Gravity();
				return;
			}
		}
		if(s_max2.value < value1 < s_max1.value ){
			simplexes[s_max1.id] = s_r;
			s_max1.value = value1;
			value2 = Contraction();	
			if(value2 < value1){
				simplexes[s_max1.id] = s_c;
				Rank();
				Gravity();
				return;
			}else{
				Reduction();
				Rank();
				Gravity();
				return;
			}
		}
		if(s_max1.value <= value1 ){
			value2 = Contraction();
			if(value2 < s_max1.value){
				simplexes[s_max1.id] = s_c;
				Rank();
				Gravity();
				return;
			}else{
				Reduction();
				Rank();
				Gravity();
				return;
			}
		}
	
		simplexes[s_max1.id] = s_r;
		Rank();
		Gravity();
	}

	double DhSimplex::get_gap(){
		return s_max1.value - s_min.value;	
	}

	double DhSimplex::get_min(){
		return s_min.value;	
	}

	double DhSimplex::get_max(){
		return s_max1.value;
	}

	void DhSimplex::get_min_simplex(Simplex &simplex){
		
		simplex = simplexes[s_min.id];
	}

	void DhSimplex::Rank(){
		double value, min, max1, max2;
		bool f_once = true;
		int i = 0;
		for(auto it = simplexes.begin(); it != simplexes.end(); ++i, ++it){
			value = (*eval_func)(*it);
			if(f_once){
				f_once = false;
				min = max1 = max2 = value;
				s_max2.value = value;
				s_max2.id = i;
				s_max1.value = value;
				s_max1.id = i;
				s_min.value = value;
				s_min.id = i;
			}else{
				if(min > value){
					min = value;
				
					s_min.value = value;
					s_min.id = i;
				}
				if(max1 < value){
					max2 = max1;
					max1 = value;
				
					s_max2.value = s_max1.value;
					s_max2.id = s_max1.id;
				
					s_max1.value = value;
					s_max1.id = i;
				}else if(max2 < value){
					max2 = value;
				
					s_max2.value = value;
					s_max2.id = i;
				}
			}
		}
		
		
	}

	void DhSimplex::Gravity(){
		Simplex sum_of_simplex(n);
		sum_of_simplex.clear();
		int i = 0;
		for(auto it = simplexes.begin(); it != simplexes.end(); ++i, ++it){
			if(i == s_max1.id)
				continue;
			sum_of_simplex += (*it);
		}
		s_g = sum_of_simplex / (simplexes.size() - 1);
	}
};
