#ifndef __porousMetal__Rendering__
#define __porousMetal__Rendering__

#include <iostream>

#endif /* defined(__porousMetal__Rendering__) */

#include <iostream>
#include "API.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <boost/numeric/ublas/matrix.hpp>   // 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp> // 三角行列用のヘッダ．前進消去，後退代入に必要
#include <boost/numeric/ublas/lu.hpp>       // LU分解，前進消去，後退代入用のヘッダ
#include <boost/numeric/ublas/io.hpp>       // ストリーム入出力用ヘッダ
using namespace boost::numeric::ublas;  // boost::numeric::ublas 名前空間を使用

typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;

class Rendering{
public:
    static int verN;   //点の総数
    static int triN;   //三角形の総数
    static int segN;   //線分の総数
    static int dimension;  //データの次元
    
    //2D version
    static float **ver; //Delaunay三角形頂点座標　N個頂点×(x,y,z)
    static float *radius;   //球の半径
    
    static int **tri; //三角形 頂点index 2D用(3点を格納) N個の三角形×３頂点
    static int **tri_neighbor;  //三角形の隣接三角形 N個の三角形×3近隣
    static float **voro_ver; //Voronoi vertex
   
    //3D version
    static int (**tetra); //四面体 3D用(4点を格納)
    static int (**tetra_neighbor);  //四面体の隣接四面体
    
    //Mesh後に増える頂点
    static std::vector< std::vector<float> > cross4Points_coord;   //交点座標格納
    static float **cross4Points_coor; //交点座標格納
    static int ***cross4Points_index; //球との交点のインデックスを i番目の三角形とj番目の球の4つの交点として入れる (n×3×4点) e.g. [i][2][0]には頂点2の向かいのvoro_verと辺の交点、[i][2][1]には頂点2の球の交点でさっきの辺との交点、[i][2][2]にはvoro_verと球心への辺の交点、[i][2][3]には...
    
    
    //３次元メッシュで計算され増加する点
    //static std::vector< std::vector<float> > newVerCoord;
    static float **newVerCoord;
    static int ***verOnSphereIndex;
    static int **verOnTetraSurfaceIndex;
    static int ***verOnTetraEdgeIndex;
    static float ****hexaCoord;
    
    
    //球の座標及び半径の読み込み
    static void ReadCoordinate(string filename);
    static void ReadCoordinate3D(string filename);
    
    //メッシュ生成
    static void Meshing();
    static void Meshing3D();
    
    //OpenGL関連
    static void OpenglRendering(int argc, char** argv); //OpenGLで描画、Main関数内に記述する
    static void InitValues();   //設定値などの初期化
    static void Init();   //描画環境の初期化
    static void Draw(unsigned char type);  //各要素の描画
    static void DrawTri(); //三角形・四面体を描く
    static void DrawSphere();  //円・球を描く
    static void DrawVoroSeg(); //Voronoi線分を描く
    static void DrawVoroVer(); //Voronou点を描く
    static void DrawPoint();   //三角形の頂点を描く
    static void DrawMesh(); //メッシュを描画
    static void Mouse(int btn, int state, int x, int y);   //Mouse
    static void Keyboard(unsigned char key, int x, int y); //Keyboard
    static void Reshape(int w, int h);
    static void Display(); //表示

    ~Rendering();   //デストラクタ
private:
    //OpenGL関連
    static int nextTriCounter;
    static bool TRI, VER, SEG, SPHERE, POINT, MESH;  //Keyboard押した時のフラグ
    static int mouseX, mouseY; //以下設定値関連
    static float qw, qx, qy, qz;
    static float zoom;
    static float shiftX, shiftY;
    static int width, height;
    
    //メッシュ計算用
    static int crossPointIndex; //交点の番号
    enum Pattern{ NORMAL = 0, OVERLAP, SEPARATE, IRREGULAR }; //メッシングパターンのフラグ
    static std::vector< int > ver_oppsite_overlapSph; //オーバーラップしている２つの球に向かいの球の頂点の番号を三角形番号と一緒に記憶
    static bool **tri_attribute;//Pattern:normalかoverlapか、overlapならその頂点をすべて保持
    static int *angle_attribute;//鈍角である頂点のインデックスを三角形ごとに保持
    
    static void InitMeshing();  //メッシング前に交点格納配列のメモリ確保
    static void Init3DMeshing();
    static bool CheckObtuseTri(float* ver1, float *ver2, float* ver3, float* voro, int triIndex);  //鈍角三角形か
    static bool CheckVoroVerPosition( float* ver0, float *ver1, float *ver2, float *ver3, float *voro, int tetraIndex );
    
    static int CheckPattern(float* ver1, float *ver2, float* ver3, float r1,  float r2, float r3, int triIndex,int label1, int label2, int label3); //Voronoiのパターンをチェック
    static int CheckPattern( float * ver0, float *ver1, float* ver2, float* ver3, float r0, float r1,  float r2, float r3, int tetraIndex);
    
    static void PushVoroVerIntoTri(float* ver1, float* ver2, float* ver3, float r1,  float r2, float r3, float* voro_ver); //鈍角三角形だった場合にvoronoi vertexを三角形内へ移動させる関数
   
    static void Generate_Mesh(float* ver1, float* ver2, float* ver3, float* voro_ver, int tri_index, int pattern, bool isObtuse );
    
    static void Generate_3DMesh(float* ver0, float* ver1, float* ver2,float* ver3,float* voro_ver, int tetra_index, int pattern, bool isVoroVerOutside );
    
    
    static dvector CalcVoroWithSurface( dvector voro, dvector ver0, dvector ver1, dvector ver2 );
    static dvector CalcVoroWithEdge( dvector voro, dvector s_v, dvector l_s_v );
    static dvector CalcOverlapCross( dvector voro,  dvector ve, dvector vref, float rad );
    static dvector CalcIntersectionWithSphere(dvector v_from, dvector v_to, float rad );
    
    static void solveMatrix(dmatrix A, dvector& b)
    {
        permutation_matrix<> pm(A.size1());
        lu_factorize(A,pm);
        lu_substitute(A,pm,b);
    }
    
    static std::vector <float> CalcuCrossPointVoroWithEdge( dvector s_v, dvector l_s_v, dvector voro ); //voronoi vertexと三角形の辺との交点を求める関数、 頂点をstdのvectorとして返す
    static std::vector <float> CalcuCrossIntersectionWithSphere(dvector v_from, dvector v_to, float rad );
    static std::vector <float> CalcuCrossOverlap( dvector ve, dvector vref, dvector voro, float rad );
    
    static bool calculateVectorProductSgn( dvector v1, dvector v2 ); //外積計算用の関数
    static bool calculateScalar3product( dvector v0, dvector v1, dvector v2 );//スカラ三重積計算
    
    static int checkSgn( dvector vec ); //３つの外積の符号の場合わけをチェックし、"鈍角"となっている頂点のインデックスを返す
    
    static bool CheckNoInfiniteVertex( int tri0, int tri1, int tri2, int tri3 );
 
};
