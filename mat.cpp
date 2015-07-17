#include "mat.hpp"

void MatInv(ublas::matrix<double>& ans,ublas::matrix<double>& mat){
	BOOST_UBLAS_CHECK(mat.size1() == mat.size2(), ublas::external_logic());
	ublas::matrix<double> mat_(mat);
	ans = ublas::identity_matrix<double>(mat_.size1());
	ublas::permutation_matrix<double> pm(mat_.size1());
	ublas::lu_factorize(mat_,pm);
    ublas::lu_substitute(mat_,pm,ans);
}


