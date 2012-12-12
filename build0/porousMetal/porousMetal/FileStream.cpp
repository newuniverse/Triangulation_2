//
//  FileStreamClass.cpp
//  porousMetal
//
//  Created by 臻禹 鈕 on 12/09/12.
//  Copyright 2012年 東京大学. All rights reserved.
//
#include "porousMetalAPI.h"
using namespace std;

Pixel::Pixel()
{
    ConstValueManager cvm;
    pxl = new int*[cvm.x];
    for (int i = 0;  i < cvm.x; i++)
    {
		pxl[i] = new int[cvm.y];
    }
}

int** FileStream::readFile(string filename){
    ConstValueManager cvm;
    Pixel p;
    ifstream fin;
    fin.open(filename.c_str());
	if (!fin.is_open())
    {
		cout << "ファイルをオープンできません" << endl;
	}
    else
    {
        int k;
        for (int j = 0; j < 475; j++) 
        {
            for (int i = 0; i < 498; i++)
            {
                fin >> k;
                p.pxl[i][j] = k;;
            }
        }
    }
	fin.close();
    fin.clear();
    return p.pxl;
}

void FileStream::writeFile(Pixel p){
    ConstValueManager cvm;
    ofstream fout;
    fout.open("result.txt");
    if(!fout.is_open())
    {
		cout << "ファイルをオープンできません" << endl;
	}
    else
    {
        for (int j = 0; j < cvm.y; j++)
        {
            for (int i = 0; i < cvm.x; i++)
            {
                fout << p.pxl[i][j];
                fout << " ";
            }
            fout << "\n";
        }
    }
    fout.close();
    fout.clear();
}

void FileStream::writeFile(double* x, double* y, double* r, string str){
    ConstValueManager cvm;
    ofstream fout;
    fout.open(str.c_str());
    if(!fout.is_open())
    {
		cout << "ファイルをオープンできません" << endl;
	}
    else
    {
        //コーナーの2点
        //fout << 0 << " " << 0 << " "<< 0 << "\n";
        //fout << cvm.x << " " << 0 << " "<< 0 << "\n";
        //重心点
        for (int i = 3; i < cvm.sum; i++)
        {
            if(r[i] <= 0) break;
            fout << x[i] << " " << y[i]<< " " << r[i]*r[i]<< "\n";
        }
        //コーナーの2点
       //fout << 0 << " " << cvm.y <<  " " << 0 << "\n";
       //fout << cvm.x << " " << cvm.y <<  " " << 0 << "\n";
    }
    fout.close();
    fout.clear();
}



