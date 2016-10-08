#ifndef _CURVEFIT_H
#define _CURVEFIT_H //< ��С���˵��࣬����BOOST

#ifndef Q_MOC_RUN
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#endif
#include <iostream>
using namespace std;
using namespace boost::numeric::ublas;

#ifndef M_PI
# define M_PI 3.1415926535
#endif

namespace FIT{

	void LUanalyse( const matrix<double> & A, matrix<double> &L, matrix<double> &U);
	matrix<double> U_inverse(matrix<double> & u);
	matrix<double> L_inverse(matrix<double> & u);
	matrix<double> inverse(matrix<double> &matrixo);
	//sizeΪ�����С  x��y�ֱ�Ϊ��������   orderΪ��Ͻ���  aΪ��ϵõ���ϵ������
	void curvefit(long size, double x[], double y[],const unsigned int order, double a[]);
	int curvefitpoints(long size, double x[],double y[], int points[]);


}

#endif  //EOF