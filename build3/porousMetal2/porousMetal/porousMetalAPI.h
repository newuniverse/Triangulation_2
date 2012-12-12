//
//  FileStream.h
//  porousMetal
//
//  Created by 臻禹 鈕 on 12/09/12.
//  Copyright 2012年 東京大学. All rights reserved.
//
#include <string.h>
using namespace std;
#include <queue>
#ifndef porousMetal_porousMetalAPI_h
#define porousMetal_porousMetalAPI_h

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
};

class Approximation
{
    
    
};

#endif
