//
//  Calculate_Voronoi_vertex.cpp
//  porousMetal
//
//  Created by 鈕 臻禹 on 12/10/14.
//  Copyright (c) 2012年 東京大学. All rights reserved.
//

#include "porousMetalAPI.h"

dvector Calculate_Voronoi_vertex::solveMatrix(float a11, float a12, float a21, float a22, float b1, float b2){
    
    dmatrix A(2,2);
    A(0,0) = a11;  A(0,1) = a12;
    A(1,0) = a21;  A(1,1) = a22;

    dvector b(2);
    b[0] = b1;    b[1] = b2;

    permutation_matrix<> pm(A.size1());
    lu_factorize(A,pm);
    lu_substitute(A,pm,b);
    return b;
}