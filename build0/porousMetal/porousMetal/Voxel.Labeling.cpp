#include "Voxel.h"
#include "ConstValue.h"
#include <stdio.h>
#include <time.h>

//近傍のボクセルの探索, 1 voxelについての処理
void Voxel::SearchNeighborVoxel( int*** visitTable, float*** voxel, int& iSum, int& jSum, int& kSum, int& numSum,int labelIndex, queue<int>& xq, queue<int>& yq, queue<int>& zq, std::vector<int>& surface_x, std::vector<int>& surface_y, std::vector<int>& surface_z){
    int qx(0),qy(0),qz(0);
    int poreVoxelCounter(0);
    for (int i = 0; i < 6; i++) {
        qx = xq.front();
        qy = yq.front();
        qz = zq.front();
        switch (i) {    //各方向への探索
            case right: qx++; break;
            case down: qy++; break;
            case left: qx--; break;
            case up: qy--; break;
            case rear: qz++; break;
            case front: qz--; break;
            default: break;
        }
        if(!this->isValid(qx, qy, qz)) continue;
        if(visitTable[qx][qy][qz] == notVisited && i < 6)
        {
            visitTable[qx][qy][qz] = hasVisited;
            voxel[qx][qy][qz] = labelIndex;
            //cout << "labelindex = " << labelIndex;
            iSum += qx;
            jSum += qy;
            kSum += qz;
            numSum++;
            xq.push(qx);
            yq.push(qy);
            zq.push(qz);
        }
        if(visitTable[qx][qy][qz] == notVisited || visitTable[qx][qy][qz] == hasVisited)
        {
            poreVoxelCounter++;
        }
    }
    
    //表面のvoxelを保存
    if(poreVoxelCounter <= 3){
       /* //気孔の最大幅を測るため
        if(max_X_coordinate < xq.front()) max_X_coordinate = xq.front();
        if(max_Y_coordinate < yq.front()) max_Y_coordinate = yq.front();
        if(min_X_coordinate > xq.front()) min_X_coordinate = xq.front();
        if(min_Y_coordinate > yq.front()) min_Y_coordinate = yq.front();*/
        surface_x.push_back(xq.front());
        surface_y.push_back(yq.front());
        surface_z.push_back(zq.front());
    }
}


void Voxel::Labeling()
{
    srand((unsigned int)time(NULL));
    //visitTableの確保
    ConstValue cv;
    int x,y,z;
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    int ***visitTable;
    visitTable = new int**[x];
    for (int i = 0;  i < x; i++)
    {
		visitTable[i] = new int*[y];
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++) {
            visitTable[i][j] = new int[z];
        }
    }
    //visitTableにvoxelをコピー
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                switch ((int)voxel[i][j][k]) {
                    case 0:
                        visitTable[i][j][k] = notVisited;
                        break;
                    case 1:
                        visitTable[i][j][k] = isMaterial;
                        break;
                    default:
                        visitTable[i][j][k] = cantVisit;
                        break;
                }
            }
        }
    }
    //labelの値合わせ
    for (int i = 0; i < labelIndex; i++) {
        x_center.push_back(0);
        y_center.push_back(0);
        z_center.push_back(0);
        radius.push_back(0);
    }
    //Labeling開始　labelの値は2からで、2は外側のすべてのvoxelに割り当てられる
    queue<int> xq,yq,zq;
    int iSum,jSum,kSum,numSum;  //重心、半径計算用
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            for(int k = 0; k < z; k++){
                if(visitTable[i][j][k] == notVisited){
                    visitTable[i][j][k] = hasVisited;
                    //外の余白部分のvoxel値をinfにしないよう回避
                    voxel[i][j][k] = labelIndex;
                    iSum += i;
                    jSum += j;
                    kSum += k;
                    numSum++;
                    xq.push(i); //queueに入れる
                    yq.push(j);
                    zq.push(k);
                    max_X_coordinate = 0; max_Y_coordinate = 0;
                    min_X_coordinate = 500; min_Y_coordinate = 500;
                    do{ // 1 labelについての処理
                        SearchNeighborVoxel(visitTable, voxel, iSum, jSum, kSum, numSum, labelIndex, xq, yq, zq, surface_x, surface_y,surface_z);
                        xq.pop();   //先頭を除く
                        yq.pop();
                        zq.pop();
                    }
                    while(!xq.empty()); //queueが空ならループを終える
                    float xx = ((float)iSum/(float)numSum);
                    float yy = ((float)jSum/(float)numSum);
                    float zz = ((float)kSum/(float)numSum);
                    float r =  sqrt((float)(numSum/M_PI));
                    //内包するvoxel計算
                    int innerVoxSum(0);
                    for(int i = 0; i < surface_x.size(); i++)
                    {
                        double dist = sqrt(pow(surface_x[i] - xx, 2) + pow(surface_y[i] - yy, 2));
                        if(dist < r){
                            innerVoxSum++;
                        }
                    }
                    float containRate;
                    if(surface_x.size() != 0) containRate = ((float)innerVoxSum/surface_x.size());
                    cout << "contain =" << containRate << endl;
                    /**ここからFindSnowmanを入れる,引数にcontainRateの条件を入れて*
                    int findIterationLimit(300);
                    int findTimes(0);*/
                    if(numSum > volume_threa && containRate > 0.45) //小さすぎる気孔と奇形の気孔は近似しない
                    {
                        /*if (containRate < 0.7) {
                            while(true){
                                findTimes++;
                                if(FindSnowManPore() == true || findTimes >= findIterationLimit)break;
                            }
                            cout << "times = " << findTimes << endl;
                            if(findTimes >= findIterationLimit) cout << "Not Found" << endl;
                        }*/
                        x_center.push_back(xx); //重心
                        y_center.push_back(yy);
                        z_center.push_back(zz);
                        radius.push_back(r);    //半径
                        if(minimumRadius > r) minimumRadius = r;
                    }else{
                        //近似しないのは全て0,0,0,0で統一,Distance field処理後に一斉削除する
                        x_center.push_back(0);
                        y_center.push_back(0);
                        z_center.push_back(0);
                        radius.push_back(0);
                    }
                    surface_x.clear(); surface_y.clear();surface_z.clear();
                    iSum = 0; jSum = 0; kSum = 0; numSum = 0;
                    labelIndex++;
                    cout << "labelindex = "<< labelIndex<< endl;
                }
            }
        }
    }
    delete [] visitTable;
    labelLayer = new int**[x];
    for (int i = 0;  i < x; i++)
    {
		labelLayer[i] = new int*[y];
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++) {
            labelLayer[i][j] = new int[z];
        }
    }
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            for(int k = 0; k < z; k++)
                labelLayer[i][j][k] = voxel[i][j][k];
    WriteTextFile(voxel, "labeled");
    WriteBinaryFile();
    cout << "minimumRadius = "<< minimumRadius <<endl;
}




/////////////////////////////////////////////////////////////////////////////////////

//RANSAC手法で雪だるま探し
bool Voxel::FindSnowManPore()
{
    dvector b1(2);   //ベクトルb
    dvector b2(2);
    float rad1(0), rad2(0), disWithinTwoPoint(0);
    //std::vector<std::vector<int> > randomPoint;
    int **randomPoint = new int*[6]; //これにRandomizeを突っ込む
    for (int i = 0; i < 6; i++) {
        randomPoint[i] = new int[3];
    }
    
    float containRate(0);
    //任意6点を取り出す
    for (int i = 0; i < 6; i++ ) {
        int randomIndex = Randomize((int)surface_x.size());
        randomPoint[i][0] = surface_x[randomIndex];
        randomPoint[i][1] = surface_y[randomIndex];
        randomPoint[i][2] = surface_z[randomIndex];
        cout << "randomPoint = " << randomPoint[i][0] <<" "<<randomPoint[i][1] <<" "<<randomPoint[i][2] << endl;
    }
    //任意6点を使い、雪だるま探し
    if(surface_x.size() > 0) {
        //共通円の中心と半径の計算
        dmatrix A(2, 2);    //行列Aの計算
        A(0,0) = calculateMatrixElements(randomPoint[0][0], randomPoint[1][0]);
        A(0,1) = calculateMatrixElements(randomPoint[0][1], randomPoint[1][1]);
        A(1,0) = calculateMatrixElements(randomPoint[1][0], randomPoint[2][0]);
        A(1,1) = calculateMatrixElements(randomPoint[1][1], randomPoint[2][1]);
        
        b1[0] = calculateVectorElements(randomPoint[0][0], randomPoint[1][0], randomPoint[0][1], randomPoint[1][1]);
        b1[1] = calculateVectorElements(randomPoint[1][0], randomPoint[2][0], randomPoint[1][1], randomPoint[2][1]);
        //行列計算の例外処理
        try {
            solveMatrix(A, b1);
        } catch (...) {
            cout << "solve Matrix failed!" <<endl;
            return false;
        }
        //中心座標計算
        dvector vec(2);
        for (int i = 0; i < 2; i++) {
            vec[i] = (float)randomPoint[0][i];
        }
        rad1 = calculateRadius(b1, vec);
        
        A(0,0) = calculateMatrixElements(randomPoint[3][0], randomPoint[4][0]);
        A(0,1) = calculateMatrixElements(randomPoint[3][1], randomPoint[4][1]);
        A(1,0) = calculateMatrixElements(randomPoint[4][0], randomPoint[5][0]);
        A(1,1) = calculateMatrixElements(randomPoint[4][1], randomPoint[5][1]);
        
        b2[0] = calculateVectorElements(randomPoint[3][0], randomPoint[4][0], randomPoint[3][1], randomPoint[4][1]);
        b2[1] = calculateVectorElements(randomPoint[4][0], randomPoint[5][0], randomPoint[4][1], randomPoint[5][1]);
        try {
            solveMatrix(A, b2);
        } catch (...) {
            cout << "solve Matrix failed!" <<endl;
            return false;
        }
        for (int i = 0; i < 2; i++) {
            vec[i] = (float)randomPoint[3][i];
        }
        rad2 = calculateRadius(b2, vec);
        
        int innerVoxSum = 0;
        cout<< "center1 = " << b1[0] << " " << b1[1] <<endl;
        cout<< "center2 = " << b2[0] << " " << b2[1] <<endl;
        cout << "radius1 = " << rad1 << "\n" << "radius2 = " << rad2 <<endl;
        for(int i = 0; i < surface_x.size(); i++)
        {
            float dist = sqrt(pow(surface_x[i] - b1[0], 2) + pow(surface_y[i] - b1[1], 2));
            if(dist < rad1){
                innerVoxSum++;
            }
            dist = sqrt(pow(surface_x[i] - b2[0], 2) + pow(surface_y[i] - b2[1], 2));
            if(dist < rad2){
                innerVoxSum++;
            }
        }
        if(surface_x.size() != 0) containRate = ((float)innerVoxSum/(float)surface_x.size());
        cout << innerVoxSum <<endl;
        cout << "Snowman containRate =" << containRate << endl;
    }
    delete[] randomPoint;
    disWithinTwoPoint = calculateRadius(b1, b2);
    //２つの円が内包する元の気孔の表面点がほぼ全体の9割ならば、これはSnowman pore
    if(Check(containRate, rad1, rad2, disWithinTwoPoint, b1, b2) == true) {
        voxel[(int)b1[0]][(int)b1[1]][0] = 10;
        voxel[(int)b2[0]][(int)b2[1]][0] = 10;
        cout <<"Found!"<<endl;
        return true;
    }else return  false;
}

bool Voxel::Check(float containRate, float rad1, float rad2, float disWithinTwoPoint, dvector b1, dvector b2)
{
    //まずは気孔の幅と高さを計算
    int width = max_X_coordinate - min_X_coordinate;
    int height = max_Y_coordinate - min_Y_coordinate;
    //中心点２点が気孔内にあるかを確かめる
    max_Y_coordinate = -1;
    min_Y_coordinate = -1;
    max_X_coordinate = -1;
    min_X_coordinate = -1;
    for (int i = 0; i < (int)surface_x.size(); i++) {
        if(surface_x[i] == (int)b1[0] && min_Y_coordinate < 0 && max_Y_coordinate > 0) min_Y_coordinate = surface_y[i];
        if(surface_x[i] == (int)b1[0] && max_Y_coordinate < 0) max_Y_coordinate = surface_y[i]; //b1はyを比較
        if(surface_y[i] == (int)b2[1] && min_X_coordinate < 0 && max_X_coordinate > 0) min_X_coordinate = surface_x[i];
        if(surface_y[i] == (int)b2[1] && max_X_coordinate < 0) max_X_coordinate = surface_x[i]; //b2はxを比較
    }
    int maxY = Max(max_Y_coordinate, min_Y_coordinate);
    int minY = Min(max_Y_coordinate, min_Y_coordinate);
    int maxX = Max(max_X_coordinate, min_X_coordinate);
    int minX = Min(max_X_coordinate, min_X_coordinate);
    
    cout << "max x = " << max_X_coordinate << endl;
    cout << "min x = "  << min_X_coordinate << endl;
    if(containRate > 0.7 && containRate < 1.3 && disWithinTwoPoint > Max(rad1, rad2)
       && disWithinTwoPoint < (rad1+rad2) && b1[1] < maxY && b1[1] > minY && b2[0] < maxX && b2[0] > minX
       && (Max(2*rad1, 2*rad2) < Max(width,height)))
        return true;
    else return false;
}


float Voxel::calculateRadius(dvector center, dvector point){
    dvector temp = center - point;
    float r(0);
    for (int i= 0; i < 2; i++) {
        r += temp[i]*temp[i];
    }
    r = sqrt(r);
    return r;
}

float Voxel::calculateMatrixElements(int x1, int x2){
    return 2*(x1 - x2);
}

float Voxel::calculateVectorElements(int x1, int x2, int y1, int y2){
    return x1*x1 + y1*y1 - (x2*x2 + y2*y2);
}
//boostで行列計算
void Voxel::solveMatrix(dmatrix A, dvector& b){
    permutation_matrix<> pm(A.size1());
    lu_factorize(A,pm);
    lu_substitute(A,pm,b);
}

//乱数生成
int Voxel::Randomize(int range)
{
    int random = rand() % range;
    return random;
}
