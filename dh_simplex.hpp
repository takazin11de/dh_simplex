/*
 * dh_simplex.hpp
 * 
 * Copyright (c) 2014 takako takahito
 * Released under the MIT license
 * http://opensource.org/licenses/mit-license.php
 * 
 * クラスdh_simplex::DhSimplexについて
 * メンバ関数Initにより初期化
 * Init(std::vector<Simplex> &シンプレックス,p_eval_func 評価関数)
 * 評価関数はシンプレックスを得て実数を返す関数である．
 * シンプレックスはublas::vector<double>型である．
 * Do()メンバ関数によって最適化処理を一回実行する．
 * get_gap()メンバ関数はシンプレックス評価値の最大最小の差を返す．
 * get_min()メンバ関数はシンプレックス評価値の最小を返す．
 * get_max()メンバ関数はシンプレックス評価値の最大を返す．
 * get_min_simplex()メンバ関数は評価値が最小のシンプレックスを参照で返す．
 */


#ifndef DH_SIMPLEX_HPP
#define DH_SIMPLEX_HPP

#include "mat.hpp"

namespace dh_simplex{
	typedef ublas::vector<double> Simplex;
	typedef double (*p_eval_func)(Simplex&);
	struct managed_simplex{
		int id;
		double value;
	};
	
	class DhSimplex{
		public:
		void Init(std::vector<Simplex> &_simplexes,p_eval_func eval_func, int n_);
		void Do();
		double get_min();
		double get_max();
		double get_gap();
		void get_min_simplex(Simplex &simplex);
	
		private:
		std::vector<Simplex> simplexes;
		p_eval_func eval_func;
		managed_simplex s_max1, s_max2, s_min;
		Simplex s_g, s_r, s_e, s_c;
		double k_alpha, k_beta, k_gamma;
		int n;
	
		void Rank();
		void Gravity();
	};
};



#endif 
