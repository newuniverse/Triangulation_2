//
//  FileStream.h
//  porousMetal
//
//  Created by 臻禹 鈕 on 12/09/12.
//  Copyright 2012年 東京大学. All rights reserved.
//
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
#ifndef porousMetal_porousMetalAPI_h
#define porousMetal_porousMetalAPI_h
using namespace boost::numeric::ublas;  // boost::numeric::ublas 名前空間を使用
typedef boost::numeric::ublas::vector<double> dvector;
typedef boost::numeric::ublas::matrix<double> dmatrix;
using namespace std;
//定数を管理するクラス
class ConstValueManager{
    public:
    enum flag { cantVisit, notVisited, hasVisited, isMaterial };
    int sum;
    int x,y;
    double pixelRatio;
    int pixelNumThreshold;
    int labelIndex;
    float resolution;
    ConstValueManager();
};

class Pixel{
    public: 
    int **pxl;
    Pixel();
};

//ファイル入出力クラス
class FileStream
{
    public:
    int** readFile(string filename);
    void writeFile(Pixel p);
    void writeFile(double* x,double* y, double* r, string str);
};

class Labeling{
    public:
    void QueueFunc(Pixel p);
    static double* r;
    static double* gCenterI;
    static double* gCenterJ;
    Labeling();
    private:
    void PixelContainRate();
};


class Relaxation{
    public:
    void BasedRelaxation();
    void RungeKutta();
};

class Calculate_Voronoi_vertex
{
    public:
    dvector solveMatrix(float a11, float a12, float a21, float a22, float b1, float b2);
};

/*
class opengl_Rendering{
    public:
    float reso = 0.012;
    static int verN;
    static float (*ver)[3];
    static int triN;
    static int (*tri)[3];
    static float (*rad);
    static int dimension;
    static float (*norT)[3];
    static float (*norV)[3];
    static float (*lap)[3];
    static int *neiN;
    
    opengl_Rendering();
    static int mouseX, mouseY;
    static float qw, qx, qy, qz;
    static float zoom;
    static float shiftX, shiftY;
    static bool wire, flat, smooth, cur, circle;
    static int width, height;
    static float aveE;
    static void myinit();
    static void myReshape(int w, int h);
    static void mouse(int btn, int state, int x, int y);
    static void keyboard(unsigned char key, int x, int y);
   static void allocateMem();
   static void readMesh(char* file_name);
   static void normalizeSize(float length);
    static void computeTriangleNormals();
   static void computeVertexNormals();
   static void computeLaplacians();
   static void laplacianSmoothing();
   static void drawWire();
   static void drawPoint();
   static void drawCircle();
   static void flatShading();
   static void smoothShading();
   static void curvature();
    static void display();
   static void deleteMesh();
    
    static void glutControl();
    
};
*/

#endif
