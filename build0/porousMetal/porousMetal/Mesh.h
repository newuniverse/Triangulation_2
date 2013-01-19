#include "API.h"
#ifndef __porousMetal__Mesh__
#define __porousMetal__Mesh__

#include <iostream>

#endif /* defined(__porousMetal__Mesh__) */
using namespace boost::numeric::ublas;  // boost::numeric::ublas 名前空間を使用
typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;

class Mesh{
public:
    Mesh();
    int triN;
    int verN;
    int crossPointIndex = 0;
    bool isObtuseTri;
    std::vector<int> obtuseAngleIndex; 
    enum Pattern{ NORMAL = 0, SEPARATE, OVERLAP, IRREGULAR, NOMESH }; //メッシングパターンのフラグ
    void Init();
    bool CheckObtuseTri(float* ver1, float *ver2, float* ver3);  //鈍角三角形か
    void PushVoroVerIntoTri(float* ver1, float* ver2, float* ver3, float r1,  float r2, float r3, float* voro_ver); //isObtuseがtrueの時に
    int CheckPattern(float* ver1, float *ver2, float* ver3, float r1,  float r2, float r3, int label1, int label2, int label3); //Voronoiのパターンをチェック
    void Generate_Mesh_Normal(float* ver1, float* ver2, float* ver3, float* voro_ver, int tri_index);
    void Generate_Mesh_Separate();
    void Generate_Mesh_Overlap();
    
    std::vector< std::vector<float> > cross4Points_coord;
    
    //float **cross4Points_coor; //球との交点座標 i個の交点×(x,y,z);
    int ***cross4Points_index; //球との交点のインデックスを i番目の三角形とj番目の球の4つの交点として入れる (n×3×4点) e.g. [i][2][0]には頂点2の向かいのvoro_verと辺の交点、[i][2][1]には頂点2の球の交点でさっきの辺との交点、[i][2][2]にはvoro_verと球心への辺の交点、[i][2][3]には...

private:
    void CalculateIntersectionPoint();
    std::vector <float> CalcuCrossPointVoroWithEdge( dvector s_v, dvector l_s_v, dvector voro );
    
};