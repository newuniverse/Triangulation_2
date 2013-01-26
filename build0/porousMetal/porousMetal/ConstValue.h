//
//  ConstValue.h
//  porousMetal
//
//  Created by 鈕 臻禹 on 12/11/12.
//  Copyright (c) 2012年 東京大学. All rights reserved.
//
#include "API.h"
#ifndef __porousMetal__ConstValue__
#define __porousMetal__ConstValue__

#include <iostream>
#endif /* defined(__porousMetal__ConstValue__) */

//定数値を一括管理するクラス、プログラムの全スコープで使う可能性があり、一度決まったら変更はしないので、静的に定義する
class ConstValue{
public:
    void Set();
    int GetX();
    int GetY();
    int GetZ();
    int GetDim();
    float GetSD();
    int GetRad();
    string GetFileName();
    enum phase{ VOXEL_PROCESSING = 0, VOXEL_DISTANCEFIELD, RENDERING, BOTH};
    int Phase;
    void MovePhase();
    
private:
    static int X_MAX ; //入力ボクセルのx,y,z成分の最大値
    static int Y_MAX ;
    static int Z_MAX ;
    static int DIMENSION;  //1 voxelの解像度
    static float SD_threshold;//真球度の閾値
    static int convergence_radius;//収束半径
    static string fileTobeRead;
};
