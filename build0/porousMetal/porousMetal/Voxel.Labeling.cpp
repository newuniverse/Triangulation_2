#include "Voxel.h"
#include "ConstValue.h"
#include <stdio.h>
#include <time.h>

//近傍のボクセルの探索, 1 voxelについての処理
void Voxel::SearchNeighborVoxel( int*** visitTable, float*** voxel, int& iSum, int& jSum, int& kSum, int& numSum, const int labelIndex, queue<int>& xq, queue<int>& yq, queue<int>& zq, std::vector<int>& surface_x, std::vector<int>& surface_y, std::vector<int>& surface_z){
    
    bool isSurfaceVoxel(false);
    int qx(0),qy(0),qz(0);
    for (int i = 0; i < 6; i++)
    {
        qx = xq.front();
        qy = yq.front();
        qz = zq.front();
        //cout << "direction index = "<<i <<endl;
        switch (i) {    //各方向への探索
            case 0: qx++; break;
            case 1: qy++; break;
            case 2: qz++; break;
            case 3: qx--; break;
            case 4: qy--; break;
            case 5: qz--; break;
            default: break;
        }
        //cout <<"(x, y, z ) = "<< qx <<" " << qy << " " << qz <<endl;
        if(!this->isValid(qx, qy, qz)) continue;
        if(!this->isValid( xq.front(), yq.front(), zq.front())) continue;
        
        if(visitTable[qx][qy][qz] == notVisited )
        {
            visitTable[qx][qy][qz] = hasVisited;
            voxel[qx][qy][qz] = (float)labelIndex*1.0;
            if(labelIndex > 2){
                iSum += qx;
                jSum += qy;
                kSum += qz;
                numSum++;
            }
            xq.push(qx);
            yq.push(qy);
            zq.push(qz);
        }
        if( visitTable[ qx ][ qy ][ qz ] == isMaterial ) isSurfaceVoxel = true;
    }
    
    //表面のvoxelを保存
    if(isSurfaceVoxel == true)
    {
        surface_x.push_back( xq.front() );
        surface_y.push_back( yq.front() );
        surface_z.push_back( zq.front() );
    }
}


void Voxel::Labeling()
{
    //srand((unsigned int)time(NULL));
    //visitTableの確保
    ConstValue cv;
    totalPoreVolume = 0;
    int x,y,z;
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    cout << x << "" << y<< " " << z;
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
                //cout << "voxel = "<<(int)voxel[i][j][k] <<endl;
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
    for (int i = 0; i <= labelIndex; i++) {
        x_center.push_back(0);
        y_center.push_back(0);
        z_center.push_back(0);
        radius.push_back(0);
    }
    //cout << radius.size() << endl;
    //Labeling開始　labelの値は2からで、2は外側(空気部分)のすべてのvoxelに割り当てられる
    //labelIndex =3;
    queue<int> xq,yq,zq;
    int iSum, jSum, kSum, numSum;  //重心、半径計算用
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            for(int k = 0; k < z; k++){
                if(visitTable[i][j][k] == notVisited){
                    visitTable[i][j][k] = hasVisited;
                    //外の余白部分のvoxel値をinfにしないよう回避
                    voxel[i][j][k] = (float)labelIndex*1.0;
                    iSum += i;
                    jSum += j;
                    kSum += k;
                    numSum++;
                    xq.push(i); //queueに入れる
                    yq.push(j);
                    zq.push(k);
                   
                    do{ // 1 labelについての処理
                        SearchNeighborVoxel(visitTable, voxel, iSum, jSum, kSum, numSum, labelIndex, xq, yq, zq, surface_x, surface_y,surface_z);
                        xq.pop();   //先頭を除く
                        yq.pop();
                        zq.pop();
                        //cout <<"queuesize = " <<xq.size() << endl;
                    }while(!xq.empty()); //queueが空ならループを終える
                    
                    //cout << "number of voxel = " << numSum <<endl;
                    
                    //cout << "vox2 = "<<voxel[i][j][k] << endl;
                    
                    //最小二乗平均球面の計算　最小二乗中心法(LSC)
                    dvector temp;
                    dvector center(3);
                    temp = LeastSquareSphere();
                    float solvedRad(0);
                    if( cv.GetDim() == 3 ){
                        for( int i = 0; i < 3; i++ ) center[ i ] = temp[ i ] / (-2);
                        solvedRad = sqrt( center[ 0 ]*center[ 0 ] + center[ 1 ]*center[ 1 ] + center[ 2 ]*center[ 2 ] - temp[ 3 ] );
                    }
                    if( cv.GetDim() == 2){
                        for( int i = 0; i < 2; i++ ) center[ i ] = temp[ i ] / (-2);
                        center[2] = 0;
                        solvedRad = sqrt( center[ 0 ]*center[ 0 ] + center[ 1 ]*center[ 1 ] - temp[ 2 ] );
                    }
                    
                    float minRadius(1000);
                    float maxRadius(0);
                    for (int i = 0; i < (int)surface_x.size(); i++)
                    {
                        dvector point(3);
                        point[ 0 ] = surface_x[ i ];
                        point[ 1 ] = surface_y[ i ];
                        point[ 2 ] = surface_z[ i ];
                        float rad = calculateRadius( center, point );
                        if( rad > maxRadius) maxRadius = rad;
                        if( rad < minRadius ) minRadius = rad;
                    }
                    
                    float correctionValue(0);
                    if(solvedRad < 5) correctionValue = 0.4;
                    else if(solvedRad < 30) correctionValue = 0.2;
                    else if(solvedRad < 45) correctionValue = 0.1;
                    else if(solvedRad < cv.GetX())correctionValue = 0.02;
                    
                    float sphericityDeviation = 1.0 - ((maxRadius - minRadius) / maxRadius) + correctionValue;
                    if(labelIndex > 2 && solvedRad < cv.GetX() && solvedRad > 1)
                    {
                        csvContainer.push_back( sphericityDeviation );
                        csvContainer2.push_back( solvedRad );
                    }
                    
                  
                    
               /////////////////////////////////////////////////////////////////
                    
                    float xx = ( ( float )iSum / ( float )numSum);
                    float yy = ( ( float )jSum / ( float )numSum );
                    float zz = ( ( float )kSum / ( float )numSum);
                    float r(0);
                    if(cv.GetDim() == 2)  r =  sqrt( ( float )( numSum / M_PI ) );
                    if(cv.GetDim() == 3)  r = pow( 3.0 * (float)numSum / (4.0*M_PI) , 1.0/3.0);
                    
                    if(labelIndex > 2){
                        if( /*numSum > volume_threa &&*/ sphericityDeviation > 0.7 )
                        {
                            x_center.push_back( xx ); //重心
                            y_center.push_back( yy );
                            z_center.push_back( zz );
                            radius.push_back( r );    //半径
                            volume.push_back( numSum );   //体積(voxel総数)
                            if(minimumRadius > r) minimumRadius = r;
                        }else{
                            //近似しないのは全て0,0,0,0で統一,Distance field処理後に一斉削除する
                            x_center.push_back( 0 );
                            y_center.push_back( 0 );
                            z_center.push_back( 0 );
                            radius.push_back( 0 );
                            volume.push_back( numSum );
                            totalPoreVolume += (unsigned long)numSum;//近似されない気孔の総体積を格納
                        }
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
    labelLayer = new int**[x];//labelLayerは距離場計算時に参照する
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
    
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            for(int k = 0; k < z; k++){
                labelLayer[i][j][k] = voxel[i][j][k];
                
            }
        }
    }
    
    if(cv.GetDim() == 2) WriteTextFile(voxel, "labeled");
    if(cv.GetDim() == 3) WriteBinaryFile();
    WriteCsvData("SphericityDeviation.csv");
    csvContainer.clear();
    csvContainer2.clear();
    cout << "minimumRadius = "<< minimumRadius <<endl;
    cout << "total volume of unapproximated pores = " << totalPoreVolume << endl;
    cout << "candidate sphere sum = " << radius.size() - 2 <<endl;
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
    for (int i= 0; i < 3; i++) {
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

dvector Voxel::LeastSquareSphere()
{
    ConstValue cv;
    dvector returnVector;
    if (cv.GetDim() == 3)
    {
        dmatrix A( 4, 4 );
        dvector b( 4 );
        A( 0, 0 ) = Sum( surface_x, 2 );
        A( 0, 1 ) = Sum( surface_x, 1, surface_y, 1 );
        A( 0, 2 ) = Sum( surface_x, 1, surface_z, 1 );
        A( 0, 3 ) = Sum( surface_x, 1 );
        A( 1, 0 ) = Sum( surface_x, 1, surface_y, 1 );
        A( 1, 1 ) = Sum( surface_y, 2 );
        A( 1, 2 ) = Sum( surface_y, 1, surface_z, 1 );
        A( 1, 3 ) = Sum( surface_y, 1 );
        A( 2, 0 ) = Sum( surface_x, 1, surface_z, 1 );
        A( 2, 1 ) = Sum( surface_y, 1, surface_z, 1 );
        A( 2, 2 ) = Sum( surface_z, 2 );
        A( 2, 3 ) = Sum( surface_z, 1);
        A( 3, 0 ) = Sum( surface_x, 1 );
        A( 3, 1 ) = Sum( surface_y, 1 );
        A( 3, 2 ) = Sum( surface_z, 1);
        A( 3, 3 ) = surface_x.size();
        
        b[ 0 ] = (-1)*(Sum( surface_x, 3 ) + Sum( surface_x, 1, surface_y, 2) + Sum( surface_x, 1, surface_z, 2));
        b[ 1 ] = (-1)*(Sum( surface_y, 1, surface_x, 2 ) + Sum( surface_y, 3 ) + Sum( surface_y, 1, surface_z, 2 ));
        b[ 2 ] = (-1)*(Sum( surface_z, 1, surface_x, 2 ) + Sum( surface_z, 1, surface_y, 2 ) + Sum( surface_z, 3 ));
        b[ 3 ] = (-1)*( Sum( surface_x, 2 ) + Sum( surface_y, 2 ) + Sum( surface_z, 2 ) );
        
        try {
            solveMatrix( A, b );
        } catch (...) {
            cout << "solve Matrix failed!" <<endl;
            for (int i = 0; i < 4; i++) b[i] = 0;
        }
        returnVector = b;
    }
    if( cv.GetDim() == 2 )
    {
        dmatrix A( 3, 3 );
        dvector b( 3 );
        A( 0, 0 ) = Sum( surface_x, 2 );
        A( 0, 1 ) = Sum( surface_x, 1, surface_y, 1 );
        A( 0, 2 ) = Sum( surface_x, 1 );

        A( 1, 0 ) = Sum( surface_x, 1, surface_y, 1 );
        A( 1, 1 ) = Sum( surface_y, 2 );
        A( 1, 2 ) = Sum( surface_y, 1 );
      
        A( 2, 0 ) = Sum( surface_x, 1 );
        A( 2, 1 ) = Sum( surface_y, 1 );
        A( 2, 2 ) = surface_x.size();
        
        
        b[ 0 ] = (-1)*( Sum( surface_x, 3 ) + Sum( surface_x, 1, surface_y, 2) );
        b[ 1 ] = (-1)*(Sum( surface_y, 1, surface_x, 2 ) + Sum( surface_y, 3 ) );
        b[ 2 ] = (-1)*(Sum( surface_x, 2 ) + Sum( surface_y, 2 ) );
        
        try {
            solveMatrix( A, b );
        } catch (...) {
            cout << "solve Matrix failed!" <<endl;
        }
        returnVector = b;
    }
    //cout << returnVector[ 0 ] << " " << returnVector[ 1 ] << " " << returnVector[ 2 ]<<endl;
    return returnVector;
}

float Voxel::Sum(std::vector<int> surface, int n)
{
    float returnValue(0);
    for (int i = 0; i < (int)surface.size(); i++) {
        returnValue += pow((float)surface[ i ], (float)n);
        //cout << "要素 = "<< pow((float)surface[ i ], (float)n) << "元 = "<< surface[ i ] <<endl;
    }
    return returnValue;
}

float Voxel::Sum(std::vector<int> surface1, int n1, std::vector<int> surface2, int n2 )
{
    float returnValue(0);
    for (int i = 0; i < (int)surface1.size(); i++) {
        returnValue += ( pow((float)surface1[ i ], (float)n1) * pow((float)surface2[ i ], (float)n2) );
        //cout << "要素 = "<< pow((float)surface1[ i ], (float)n1) * pow((float)surface2[ i ], (float)n2) <<endl;
    }
    return returnValue;
}

//boostで行列計算
void Voxel::solveMatrix(dmatrix A, dvector& b)
{
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
