#ifndef MAT_HPP
#define MAT_HPP

#include <boost/numeric/ublas/matrix.hpp>		//行列
#include <boost/numeric/ublas/vector.hpp>		//ベクトル
#include <boost/numeric/ublas/triangular.hpp>	//三角行列 前進消去、後退代入
#include <boost/numeric/ublas/io.hpp>	//ストリーム入出力
#include <boost/numeric/ublas/lu.hpp>	//lu分解


using namespace boost::numeric;

void MatInv(ublas::matrix<double>& ans,ublas::matrix<double>& mat);

template <class M>
double MatDet(const M& m)
{
    BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());
    ublas::matrix<double>       lu(m);
    ublas::permutation_matrix<> pm(m.size1());
    ublas::lu_factorize(lu, pm);
    double det(1);
    typedef ublas::permutation_matrix<>::size_type size_type;
    for (size_type i = 0; i < pm.size(); ++i) {
        det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
    }
    return det;
}


#endif
