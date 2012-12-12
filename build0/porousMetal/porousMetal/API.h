//
//  API.h
//  porousMetal
//
//  Created by 鈕 臻禹 on 12/11/12.
//  Copyright (c) 2012年 東京大学. All rights reserved.
//

#include <sstream>;
#include <string.h>
#include <queue>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <boost/numeric/ublas/matrix.hpp>   // 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp> // 三角行列用のヘッダ．前進消去，後退代入に必要
#include <boost/numeric/ublas/lu.hpp>       // LU分解，前進消去，後退代入用のヘッダ
#include <boost/numeric/ublas/io.hpp>       // ストリーム入出力用ヘッダ
//using namespace boost::numeric::ublas;  // boost::numeric::ublas 名前空間を使用
typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;
using namespace std;
#ifndef porousMetal_API_h
#define porousMetal_API_h
#endif
