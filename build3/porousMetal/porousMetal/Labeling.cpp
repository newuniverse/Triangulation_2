//
//  Queue.cpp
//  porousMetal
//
//  Created by 臻禹 鈕 on 12/09/13.
//  Copyright 2012年 東京大学. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "porousMetalAPI.h"

ConstValueManager cvm;
 double* Labeling::r = new double[cvm.sum];
 double* Labeling::gCenterI = new double[cvm.sum];
 double* Labeling::gCenterJ = new double[cvm.sum];

Labeling::Labeling(){
    
}

void Labeling::QueueFunc(Pixel p){
   
    Pixel visitTable;
    int* iSum = new int[cvm.x];
    int* jSum = new int[cvm.y];
    int* numSum = new int[cvm.sum];
    
    int qx,qy;
    queue<int> xq,yq;
    
    for(int i = 0; i < cvm.x;i++){
		for (int j = 0; j < cvm.y; j++){
			p.pxl[0][j] = -1;
			p.pxl[cvm.x-1][j] = -1;
            p.pxl[i][0] = -1;
            p.pxl[i][cvm.y-1] = -1;
		}
	}
    
    for (int i = 0; i < cvm.x; i++) {
        for (int j = 0; j < cvm.y; j++) {
            visitTable.pxl[i][j] = p.pxl[i][j];
        }
    }
    
    
    for (int j = 0; j < cvm.y; j++){
        for (int i = 0; i < cvm.x; i++){
            switch(p.pxl[i][j]){
                case 1:
                    visitTable.pxl[i][j] = cvm.isMaterial;
                    break;
                case 0:
                    visitTable.pxl[i][j] = cvm.notVisited;
                    break;
                case -1:
                    visitTable.pxl[i][j] = cvm.cantVisit;
                    break;
                default :
                    visitTable.pxl[i][j] = cvm.cantVisit;
                    break;
            }
        }
    }

   		for(int j = 2; j < cvm.y-2; j++){
			for(int i = 2; i < cvm.x-2; i++){
				
				if(visitTable.pxl[i][j] == cvm.notVisited){
					visitTable.pxl[i][j] = cvm.hasVisited;
                    p.pxl[i][j] = cvm.labelIndex;
                    if(cvm.labelIndex > 2){
                        iSum[cvm.labelIndex] += i;
                        jSum[cvm.labelIndex] += j;
                        numSum[cvm.labelIndex]++;
                    }
                    xq.push(i);
                    yq.push(j);

					do{
						qx = xq.front();
						qy = yq.front();
						//search right
						if(visitTable.pxl[qx+1][qy] == cvm.notVisited){ 
							visitTable.pxl[qx+1][qy] = cvm.hasVisited;
							p.pxl[qx+1][qy] = cvm.labelIndex;
							if(cvm.labelIndex > 2){
								iSum[cvm.labelIndex] += qx+1;
								jSum[cvm.labelIndex] += qy;
								numSum[cvm.labelIndex]++;
							}
							xq.push(qx+1);
							yq.push(qy);
						}
						//search down
						
						if(visitTable.pxl[qx][qy+1] == cvm.notVisited){
							visitTable.pxl[qx][qy+1] = cvm.hasVisited;
							p.pxl[qx][qy+1] = cvm.labelIndex;
							if(cvm.labelIndex > 2){
								iSum[cvm.labelIndex] += qx;
								jSum[cvm.labelIndex] += qy+1;
								numSum[cvm.labelIndex]++;
							}
							xq.push(qx);
							yq.push(qy+1);
                            
						}
						//search left
						if(visitTable.pxl[qx-1][qy] == cvm.notVisited){
							visitTable.pxl[qx-1][qy] = cvm.hasVisited;
							p.pxl[qx-1][qy] = cvm.labelIndex;
							if(cvm.labelIndex > 2){
								iSum[cvm.labelIndex] += qx-1;
								jSum[cvm.labelIndex] += qy;
								numSum[cvm.labelIndex]++;
							}
							xq.push(qx-1);
							yq.push(qy);
						}
						//search up
						if(visitTable.pxl[qx][qy-1] == cvm.notVisited){
							visitTable.pxl[qx][qy-1] = cvm.hasVisited;
							p.pxl[qx][qy-1] = cvm.labelIndex;
							if(cvm.labelIndex > 2){
								iSum[cvm.labelIndex] += qx;
								jSum[cvm.labelIndex] += qy-1;
								numSum[cvm.labelIndex]++;
							}
							xq.push(qx);
							yq.push(qy-1);
						}
                        xq.pop();
						yq.pop();
					}while(!xq.empty());
                    if( cvm.labelIndex > 2){            
                        gCenterI[cvm.labelIndex] = ((double)iSum[cvm.labelIndex]/(double)numSum[cvm.labelIndex]);	             
                        gCenterJ[cvm.labelIndex] = ((double)jSum[cvm.labelIndex]/(double)numSum[cvm.labelIndex]);
                        r[cvm.labelIndex] = sqrt((double)(numSum[cvm.labelIndex]/M_PI));
                    }
                    cvm.labelIndex++;
				}
                /*
				if(p.pxl[k][l][n] == cvm.labelIndex-1 && cvm.labelIndex > 3){
					if((k - gCenterI[cvm.labelIndex-1])*(k - gCenterI[cvm.labelIndex-1]) + (l - gCenterJ[cvm.labelIndex-1])*(l - gCenterJ[cvm.labelIndex-1]) + (n - gCenterK[cvm.labelIndex-1])*(n - gCenterK[cvm.labelIndex-1]) <= r[cvm.labelIndex-1]*r[cvm.labelIndex-1]){
						insideNumSum[cvm.labelIndex-1]++;
					}
				}
                 */
			}
		}
    std::cout << cvm.labelIndex - 4 << "\n";
    
}

