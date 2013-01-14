#include "API.h"
#ifndef __porousMetal__Voxel__
#define __porousMetal__Voxel__

#include <iostream>
#include <list>
#include <boost/numeric/ublas/matrix.hpp>   // 普通の行列用のヘッダ
#include <boost/numeric/ublas/triangular.hpp> // 三角行列用のヘッダ．前進消去，後退代入に必要
#include <boost/numeric/ublas/lu.hpp>       // LU分解，前進消去，後退代入用のヘッダ
#include <boost/numeric/ublas/io.hpp>       // ストリーム入出力用ヘッダ
using namespace boost::numeric::ublas;  // boost::numeric::ublas 名前空間を使用
#endif /* defined(__porousMetal__Voxel__) */
typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;

class Voxel{
public:
    float*** voxel;   //読み込まれるボクセルデータ保存
    int*** labelLayer;
    int labelIndex = 2; //ラベル -1,0,1はDistance fieldでも使うので
    int inf = 10000;
    int dimension = 2;  //全体処理時の次元
    enum flag { cantVisit = -4, notVisited = -3, hasVisited = -2, isMaterial = -1}; //visitTableで使う
    enum direction { right = 0, down, left, up, rear, front};   //近傍探索で使う
    Voxel();    //コンストラクタでvoxelをインスタンス化
    void ReadFile(string filename);    //voxelファイル読み込み
    int ReadBinaryFile(string filename);    
    void WriteBinaryFile();   //voxel書き出し
    void WriteBinaryFile(float ***table, string filename);
    void WriteTextFile(float ***table, string filename);
    void WriteSphere(); //重心、半径書き出し->CGALに渡す用
    void WriteCsvData( string filename );
    std::vector<float> csvContainer; //sphericity deviation
    void Labeling();    //ラベリング
    void ComputeDistanceField();    //距離場計算
private:
    std::vector<int> surface_x;  //Poreの表面部のボクセルを保存
    std::vector<int> surface_y;
    std::vector<int> surface_z;
    std::vector<float> x_center;    //重心x座標
    std::vector<float> y_center;
    std::vector<float> z_center;
    std::vector<float> radius;  //半径
    std::vector<float> volume; //体積
    std::vector<int> x_temp_center;
    std::vector<int> y_temp_center;
    std::vector<int> z_temp_center;
    std::vector<float> rad_temp_center;
    
    bool isFirstLoop;//一回目のラベリング内ループは余白部分のピクセルに対するもの
    int volume_threa = 16;  //pore体積の閾値、これより低いものは無視し、球近似を行わない、書き出さない
    float minimumRadius = 500; //Distance Field計算する際に気孔の半径の閾値として、Labelingしたときに一番小さい半径を選択
    void SearchNeighborVoxel( int*** visitTable, float*** voxel, int& iSum, int& jSum, int& kSum, int& numSum, int labelIndex, queue<int>& xq, queue<int>& yq, queue<int>& zq, std::vector<int>& surface_x, std::vector<int>& surface_y, std::vector<int>& surface_z);   //近傍探索
    void Propagation(float *** table, int x, int y, int z, int direction);  //CDT
    
    void VdtPropagation( dvector ***vec, float***voxel, int x, int y, int z, int direction );   //VDTのプロパゲーション
    
    void Vdt();
    
    bool FindLocalMaximums(float *** table, int& x, int& y, int& z, float& dis);//fowrward Passで
    
    bool FindLocalMaximum( float ***table, int&x, int& y, int& z, float &dis );
    
    void RemoveFromSearchTarget(int x, int y, int z, float r);
    
    bool isValid(int x, int y, int z);
    
    //以下Racsac用の関数など
    int max_X_coordinate;
    int max_Y_coordinate;
    int min_X_coordinate;
    int min_Y_coordinate;
    
    bool FindSnowManPore(); //SnowMan
    bool Check(float containRate, float rad1, float rad2, float disWithinTwoPoint, dvector b1, dvector b2);
    
    void ContainRate();
    
    int Randomize(int range);
    
    void solveMatrix( dmatrix A, dvector& b);
    
    dvector LeastSquareSphere();//最小二乗平均球面の計算
    float Sum( std::vector<int> surface, int n );
    float Sum( std::vector<int> surface1, int n1, std::vector<int> surface2, int n2 );

    float calculateMatrixElements( int x1, int x2 );
    float calculateVectorElements( int x1, int x2, int y1, int y2 );
    float calculateRadius( dvector center, dvector point);
    
    float magnitudeOfVector(dvector vec);
    
    template<class T> T Max(const T& a, const T& b)
    {
        return ( a > b ) ? ( a ) : ( b );
    }
    template<class T> T Min(const T& a, const T& b){
        return ( a < b ) ? ( a ) : ( b );
    }
    void InitVoxel();
};