//
//  OpneglRendering.h
//  porousMetal
//
//  Created by 鈕 臻禹 on 12/11/14.
//  Copyright (c) 2012年 東京大学. All rights reserved.
//

#ifndef __porousMetal__OpneglRendering__
#define __porousMetal__OpneglRendering__

#include <iostream>
#include "API.h"
#endif /* defined(__porousMetal__OpneglRendering__) */
class OpenglRendering{
public:
    typedef std::vector<std::vector<float>[3] >  Vector3;
    Vector3 vertex;
    Vector3 tri;
    typedef std::vector<std::vector<float>[6] > Segment3;
    Segment3 seg;
    void drawTri();
    void drawVoronoi();
    void drawTriPoints();
    void drawVoroVertices();
    void draw(unsigned char type);
private:
    int verN;
    int triN;
    int segN;
    
};