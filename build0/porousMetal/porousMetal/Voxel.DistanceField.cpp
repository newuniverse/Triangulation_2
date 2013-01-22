#include "Voxel.h"
#include "ConstValue.h"

void Voxel::ComputeDistanceField()
{
    cout << "begin distance field!"<< endl;
    
    ConstValue cv;
    int x(0), y(0), z(0);
    
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    
    cout << "begin propagation" << endl;
    //##########ここからループに
    
    bool continueToLoop( true );
    int loopCount( 0 );
    unsigned long SumOfSphereVolume( 0 );
    
    //voxelの初期化
    if(cv.GetDim() == 3) ReadBinaryFile(cv.GetFileName());
    if(cv.GetDim() == 2) ReadFile(cv.GetFileName());
    InitVoxel();
    if(cv.GetDim() == 2) WriteTextFile(voxel, "DistanceField"); //2D
    
    while (continueToLoop == true) {
        loopCount++;
        cout << "loop count = "<< loopCount <<endl;
        //step1####################################################
        cout << "step1: Propagation" << endl;
        //VDTで距離場計算
        if(loopCount > 1) UpdateVoxel();
        Vdt();
        //Distance fieldの結果を書き出し
        ostringstream s ;
        s << loopCount;
        if(cv.GetDim() == 3 && loopCount <= 1) WriteBinaryFile(voxel, "DistanceField");//3D
        if(cv.GetDim() == 2) WriteTextFile(voxel, "DistanceField" + s.str()); //2D
    
        //step2 ####################################################
        cout << "step2: Find LocalMaximum" << endl;
        //Raster scanでlocal maximumを探す
        for(int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                for (int k = 0; k < z; k++)
                {
                    int xx = i; int yy = j; int zz = k; float r(0);
                    if( FindLocalMaximum( voxel, xx, yy, zz, r ) == true && labelLayer[ i ][ j ][ k ] > 2 )
                    {
                        RemoveFromSearchTarget( xx, yy, zz, r );
                        x_temp_center.push_back( xx );
                        y_temp_center.push_back( yy );
                        z_temp_center.push_back( zz );
                        rad_temp_center.push_back( r );
                    }
                }
            }
        }
        //step3 ####################################################
        //RemoveFromSearchTarget + register sphere
        //pairで整理
        cout << "step3: RemoveTarget and Rigister" << endl;
        std::vector< std::pair<int, int> > data; //(label, index)を格納
        for (int i = 0; i < (int)x_temp_center.size(); i++)
        {
            int x = x_temp_center[ i ];
            int y = y_temp_center[ i ];
            int z = z_temp_center[ i ];
            data.push_back( std::pair<int, int>( labelLayer[ x ][ y ][ z ], i ) );
        }
        //ラベルごとにソート
        std::sort( data.begin(), data.end() );
    /*
    for (int i = 0; i < (int)data.size(); i++) {
        cout << data[i].first << " : " << data[i].second << endl;
    }*/
    
        //## labelに沿ったマッチング ##//
        std::vector<std::pair<int, int> >::iterator it; //(label, index)のイテレータ
        it = data.begin();
    
        int it_counter(0);
        int head(0);
        bool trueSignEvenOnce( false );
        std::vector< std::pair< float, int > > labeledSubGroup; //ラベルごとの再近似用円のサブグループ(radius, index)を格納

        //すべての気孔にたいするループ
        while (it != data.end())
        {
            head = it_counter;
            int _label = it->first;
        
            //####
            //std::vector< std::pair< float, int > > labeledSubGroup; //ラベルごとの再近似用円のサブグループ(radius, index)を格納
            //labeledSubGroup.push_back( std::pair<float, int >( rad_temp_center[it->second], it->second ) );
            //####
        
            //ラベルにそったサブグループへの振り分け
            while (it->first == _label || it != data.end() )
            {
                labeledSubGroup.push_back( std::pair<float, int>( rad_temp_center[it->second], it->second ) );
                it++;
                it_counter++;
                cout << "rand size = " << rad_temp_center.size() << endl;
                //std::cerr << "rad =" << /*rad_temp_center[it->second] <<*/", index = " << it->second <<endl;
                
            }
            //半径でソート
            std::sort( labeledSubGroup.rbegin(), labeledSubGroup.rend() );
        
            std::vector<std::pair<float, int> >::iterator subit;    //サブグループのイタレータ
            subit = labeledSubGroup.begin();
        
            //SubGroupについてのループ
            while ( subit != labeledSubGroup.end() ){
                //##ここから　Distance fieldを使った球近似の再修正 ##//
                dvector currentLocalMax( 3 );
                float currentRadius( 0 );
                float disWithinTwoMax;
            
                currentLocalMax[ 0 ] = x_temp_center[ subit->second ];
                currentLocalMax[ 1 ] = y_temp_center[ subit->second ];
                currentLocalMax[ 2 ] = z_temp_center[ subit->second ];
                currentRadius = rad_temp_center[ subit->second ];
                
                if(cv.GetDim() == 2) SumOfSphereVolume += round(M_PI * currentRadius*currentRadius);
                
                if(cv.GetDim() == 3) SumOfSphereVolume += round( 4 * M_PI * currentRadius*currentRadius*currentRadius / 3);
                
                //Removeして球を登録
                //RemoveFromSearchTarget(currentLocalMax[0], currentLocalMax[1], currentLocalMax[2], currentRadius );
                
                //cout<< "current volume = " <<round(SumOfSphereVolume * 1.2) << "total volume"<<(double)totalPoreVolume<<endl ;
                cout << "current radius = " << currentRadius << endl;
                if(radius[ _label ] == 0 && currentRadius >= 1 && _label > 2/*minimumRadius floor(SumOfSphereVolume) < (double)totalPoreVolume*/ )  //収束条件
                {
                    //continueToLoop = true;
                    trueSignEvenOnce = true;
                    x_center.push_back( currentLocalMax[ 0 ]);
                    y_center.push_back( currentLocalMax[ 1 ]);
                    z_center.push_back( currentLocalMax[ 2 ]);
                    radius.push_back( currentRadius );
                }//else continueToLoop = false;
                subit++;
            }
            labeledSubGroup.clear();
        }
        data.clear();
        x_temp_center.clear();
        y_temp_center.clear();
        z_temp_center.clear();
        rad_temp_center.clear();
        if( trueSignEvenOnce == true ) continueToLoop = true;
        else continueToLoop = false;
    }
    if(cv.GetDim() == 3) WriteBinaryFile(voxel, "Matching");
    if(cv.GetDim() == 2) WriteTextFile(voxel, "Matching");
    cout << "loop count = "<< loopCount <<endl;
}


bool Voxel::RemoveSphereFromOutputCandidate(dvector focus, float focusRad, dvector candidate, float candidateRad)
{
    float disWithinTwo;
    bool returnValue( false );
    disWithinTwo = calculateRadius(focus, candidate);
    if( disWithinTwo < focusRad )
    {
        if( candidateRad < sqrt( focusRad*focusRad - disWithinTwo*disWithinTwo )  ) returnValue = true;
    }
    return returnValue;
}

bool Voxel::FindLocalMaximum(float ***table, int &x, int &y, int &z, float &dis)
{
    int qx(0), qy(0), qz(0);
    bool flag(true);
    
    for( int i = 0; i < 26; i++ )
    {
        qx = x; qy = y; qz = z;
        switch (i) {
            case 0: qx++; break;
            case 1: qy++; break;
            case 2: qz++; break;
            case 3: qx++; qy++; break;
            case 4: qy++; qz++; break;
            case 5: qz++; qx++; break;
            case 6: qx++; qy++; qz++; break;
            case 7: qx--; break;
            case 8: qy--; break;
            case 9: qz--; break;
            case 10: qx--; qy--; break;
            case 11: qy--; qz--; break;
            case 12: qz--; qx--; break;
            case 13: qx--; qy--; qz--; break;
            case 14: qx++; qy--; break;
            case 15: qy--; qz++; break;
            case 16: qz++; qx--; break;
            case 17: qx--; qy++; break;
            case 18: qy++; qz--; break;
            case 19: qz--; qx++; break;
            case 20: qx++; qy++; qz--; break;
            case 21: qx++; qy--; qz--; break;
            case 22: qx++; qy--; qz++; break;
            case 23: qx--; qy++; qz++; break;
            case 24: qx--; qy++; qz--; break;
            case 25: qx--; qy--; qz++; break;
            default: break;
        }
        if( !this->isValid( x, y, z ) ) continue;
        if ( !this->isValid( qx, qy, qz ) ) continue;
        
        if( table[ qx ][ qy ][ qz ] < table[ x ][ y ][ z ] && table[ x ][ y ][ z ] > 0.0)
        {
            dis = table[ x ][ y ][ z ];
        }else flag = false;
    }
    //cout << "flag = " << flag << " x = " << x << " y = " << y << " z = " << z <<endl;
    return flag;
}

//ローカルマキシマムなボクセルを見つける(Forward Pass)(旧ver)
bool Voxel::FindLocalMaximums(float *** table, int& x, int& y, int& z, float& dis)
{
    //cout << "begin find local maximum!"<<endl;
    int qx(0), qy(0), qz(0);
    int local_max_coordinate[3];
    float local_max_dis(0);
    bool flag(true);
    for ( int i = 0; i < 26; i++ ) {
        qx = x;
        qy = y;
        qz = z;
        switch (i) {    //各方向への探索
            case 0: qx++; break;
            case 1: qy++; break;
            case 2: qz++; break;
            case 3: qx++; qy++; break;
            case 4: qy++; qz++; break;
            case 5: qz++; qx++; break;
            case 6: qx++; qy++; qz++; break;
            case 7: qx--; break;
            case 8: qy--; break;
            case 9: qz--; break;
            case 10: qx--; qy--; break;
            case 11: qy--; qz--; break;
            case 12: qz--; qx--; break;
            case 13: qx--; qy--; qz--; break;
            case 14: qx++; qy--; break;
            case 15: qy--; qz++; break;
            case 16: qz++; qx--; break;
            case 17: qx--; qy++; break;
            case 18: qy++; qz--; break;
            case 19: qz--; qx++; break;
            case 20: qx++; qy++; qz--; break;
            case 21: qx++; qy--; qz--; break;
            case 22: qx++; qy--; qz++; break;
            case 23: qx--; qy++; qz++; break;
            case 24: qx--; qy++; qz--; break;
            case 25: qx--; qy--; qz++; break;
            default: break;
        }
        if( !this->isValid(x,y,z) ) continue;
        if ( !this->isValid(qx,qy,qz) ) continue;
        //cout << "x = " << x << " y = " << y << " z = " << z << endl;
        //cout << "qx = " << qx << " qy = " << qy << " qz = " << qz << " i = "<< i << endl;
        //cout << "focus = " << table[x][y][z]<< " search target = " << table[qx][qy][qz] << " current local max = " << local_max_dis << endl;
        if( table[qx][qy][qz] > table[x][y][z] && table[qx][qy][qz] > local_max_dis)
        {
            local_max_dis = table[qx][qy][qz];
            local_max_coordinate[0] = qx;
            local_max_coordinate[1] = qy;
            local_max_coordinate[2] = qz;
            flag = false;
        }else flag = true;
    }
    if(flag == false){ x = local_max_coordinate[0]; y = local_max_coordinate[1]; z = local_max_coordinate[2]; dis = local_max_dis;} //探索ボクセルの変更
    return flag;
}

//探索区域からの削除
void Voxel::RemoveFromSearchTarget(int x, int y, int z, float r)
{
    //cout << "begin removing!" << endl;
    for(float th = 0.0; th < 180.0; th = th + 1.0)
    {
        for (float phi = 0.0; phi < 360.0; phi = phi + 1.0)
        {
            for(float rad = 0.0; rad <= r; rad = rad + 1.0)
            {
                int xx = ceil(rad * sin(th) * cos(phi));
                int yy = ceil(rad * sin(th) * sin(phi));
                int zz = ceil(rad * cos(th));
                if ( !this->isValid(x + xx,y + yy, z + zz) ) continue;
                voxel[x + xx][y + yy][z + zz] = 0.0;
            }
        }
    }
    //cout << "end removing!" << endl;
}

float Voxel::magnitudeOfVector( dvector vec )
{
    return sqrt( pow( vec[ 0 ], 2) + pow( vec[ 1 ], 2) + pow( vec[ 2 ], 2 ) );
}

//VDT
void Voxel::Vdt()
{
    ConstValue cv;
    int x(0), y(0), z(0);
    /*
    if(cv.GetDim() == 3) ReadBinaryFile(cv.GetFileName());
    if(cv.GetDim() == 2) ReadFile(cv.GetFileName());
    InitVoxel();*/
    
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();

    enum Paths{ F1 = 0, F2, F3, F4, B1, B2, B3, B4 };      //VDT用
    
    dvector*** vec = new dvector**[ x ];
    for (int i = 0; i < x; i++) {
        vec[ i ] = new dvector*[ y ];
    }
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            vec[ i ][ j ] = new dvector[ z ];
        }
    }
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                vec[ i ][ j ][ k ].resize(3);
                for( int l = 0; l < 3; l++ )
                    vec[ i ][ j ][ k ][ l ] = 0;
            }
        }
    }
    
    
    //VDT
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                VdtPropagation(vec, voxel, i, j, k, F1);
                //cout << "F1" <<endl;
            }
        }
    }
    for(int i = x-1; i >=0 ; i--){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                VdtPropagation(vec, voxel, i, j, k, F2);
                //cout << "F2" <<endl;
            }
        }
    }
    
    for(int i = x-1; i >= 0; i--){
        for (int j = y-1; j >= 0; j--) {
            for (int k = 0; k < z; k++) {
                VdtPropagation(vec, voxel, i, j, k, F3);
                //cout << "F3" <<endl;
            }
        }
    }
    
    for(int i = 0; i < x; i++){
        for (int j = y-1; j >= 0; j--) {
            for (int k = 0; k < z; k++) {
                VdtPropagation(vec, voxel, i, j, k, F4);
                //cout << "F4" <<endl;
            }
        }
    }
    
    for(int i = x-1; i >= 0; i--){
        for (int j = y-1; j >= 0; j--) {
            for (int k = z-1; k >= 0; k--) {
                VdtPropagation(vec, voxel, i, j, k, B1);
                //cout << "B1" <<endl;
            }
        }
    }
    
    for(int i = 0; i < x; i++){
        for (int j = y-1; j >= 0; j--) {
            for (int k = z-1; k >= 0; k--) {
                VdtPropagation(vec, voxel, i, j, k, B2);
                //cout << "B2" <<endl;
            }
        }
    }
    
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = z-1; k >= 0; k--) {
                VdtPropagation(vec, voxel, i, j, k, B3);
                //cout << "B3" <<endl;
            }
        }
    }
    
    for(int i = x-1; i >= 0; i--){
        for (int j = 0; j < y; j++) {
            for (int k = z-1; k >= 0; k--) {
                VdtPropagation(vec, voxel, i, j, k, B4);
                //cout << "B4" <<endl;
            }
        }
    }

}

//VDTプロパゲーション
void Voxel::VdtPropagation(dvector ***vec, float ***voxel, int x, int y, int z, int direction)
{
    dvector dir(3);
    float argmin(1000);
    //int qx(0), qy(0), qz(0);
    for ( int i = 0; i < 3; i++){
        
        for( int i = 0; i < 3; i++ ) dir[ i ] = inf;//dirの初期化
        
        switch (direction) {    //F1, F2, ... B1, B2, ...PATHの分岐
            case 0: //F1
                switch (i) {
                    case 0: dir[ 0 ] = 0; dir[ 1 ] = 0; dir[ 2 ] = -1;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = -1; dir[ 2 ] = 0;
                        break;
                    case 2: dir[ 0 ] = -1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 1: //F2
                switch (i) {
                    case 0: dir[ 0 ] = 1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = -1; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 2: //F3
                switch (i) {
                    case 0: dir[ 0 ] = 0; dir[ 1 ] = 1; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = 1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 3: //F4
                switch (i) {
                    case 0: dir[ 0 ] = -1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = 1; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 4: //B1
                switch (i) {
                    case 0: dir[ 0 ] = 0; dir[ 1 ] = 0; dir[ 2 ] = 1;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = 1; dir[ 2 ] = 0;
                        break;
                    case 2: dir[ 0 ] = 1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }

                break;
            case 5: //B2
                switch (i) {
                    case 0: dir[ 0 ] = -1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = 1; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }

                break;
            case 6: //B3
                switch (i) {
                    case 0: dir[ 0 ] = 0; dir[ 1 ] = -1; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = -1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 7: //B4
                switch (i) {
                    case 0: dir[ 0 ] = 1; dir[ 1 ] = 0; dir[ 2 ] = 0;
                        break;
                    case 1: dir[ 0 ] = 0; dir[ 1 ] = -1; dir[ 2 ] = 0;
                        break;
                    default:
                        break;
                }

                break;
            default:
            break;
        }
    
        if( !this->isValid( x + dir[0] , y + dir[1], z + dir[2])) continue;
        
        dvector tempVec(3);
        tempVec = vec[ x + (int)dir[ 0 ] ][ y + (int)dir[1] ][ z + (int)dir[2] ] + dir;
        
        if ( magnitudeOfVector( tempVec ) < argmin && magnitudeOfVector( tempVec ) < voxel[ x ][ y ][ z ] )
        {
            argmin = magnitudeOfVector(tempVec);
            vec[ x ][ y ][ z ] = tempVec;
            if( argmin < voxel[ x ][ y ][ z ]) voxel[ x ][ y ][ z ] = argmin;
        }
    }
}

//プロパゲーション
void Voxel::Propagation(float ***table, int x, int y, int z, int direction)
{
    int qx(0), qy(0), qz(0);
    float add_dis(0);
    for (int i = 0; i < 13; i++) {
        qx = x;
        qy = y;
        qz = z;
        add_dis = 0;
        switch(direction){ //forward
            case 0:
            {
                switch (i) {    //各方向への探索
                    case 0: qx++; add_dis = 1; break; //incident face
                    case 1: qy++; add_dis = 1; break;
                    case 2: qz++; add_dis = 1; break;
                    case 3: qy++; qz--; add_dis = sqrt(2); break; //incident edge
                    case 4: qx++; qy++; add_dis = sqrt(2); break;
                    case 5: qz++; qy++; add_dis = sqrt(2); break;
                    case 6: qx--; qy++; add_dis = sqrt(2); break;
                    case 7: qx--; qz++; add_dis = sqrt(2); break;
                    case 8: qz++; qx++; add_dis = sqrt(2); break;
                    case 9: qy++; qx--; qz--; add_dis = sqrt(3); break; //incident corner
                    case 10:qy++; qx++; qz--; add_dis = sqrt(3); break;
                    case 11:qy++; qx++; qz++; add_dis = sqrt(3); break;
                    case 12:qy++; qx--; qz++; add_dis = sqrt(3); break;
                    default: break;
                }
                break;
            }
            case 1: //backward
                switch (i) {    //各方向への探索
                    case 0: qx--; add_dis = 1; break; //incident face
                    case 1: qy--; add_dis = 1; break;
                    case 2: qz--; add_dis = 1; break;
                    case 3: qy--; qz++; add_dis = sqrt(2); break; //incident edge
                    case 4: qx--; qy--; add_dis = sqrt(2); break;
                    case 5: qz--; qy--; add_dis = sqrt(2); break;
                    case 6: qx++; qy--; add_dis = sqrt(2); break;
                    case 7: qx++; qz--; add_dis = sqrt(2); break;
                    case 8: qz--; qx--; add_dis = sqrt(2); break;
                    case 9: qy--; qx++; qz++; add_dis = sqrt(3); break; //incident corner
                    case 10:qy--; qx--; qz++; add_dis = sqrt(3); break;
                    case 11:qy--; qx--; qz--; add_dis = sqrt(3); break;
                    case 12:qy--; qx++; qz--; add_dis = sqrt(3); break;
                    default: break;
                }
                break;
        }
        if ( !this->isValid(qx,qy,qz) ) continue;
        if( !this->isValid(x,y,z)) continue;
        
        if(table[x][y][z] + add_dis < table[qx][qy][qz] )   //scalarで持っている
        {
            table[qx][qy][qz] = table[x][y][z] + add_dis;
        }
    }
}

//距離場計算のためのvoxelの初期化
void Voxel::InitVoxel()
{
    ConstValue cv;
    int x(0),y(0),z(0);
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                int _label = labelLayer[ i ][ j ][ k ];
                switch((int)voxel[ i ][ j ][ k ])
                {
                case 0://気孔
                        if( (int)radius[ _label ] > 0 || _label == 2) {
                            voxel[ i ][ j ][ k ] = 0;
                        }
                        else voxel[ i ][ j ][ k ] = inf;
                    break;
                case 1://金属
                    voxel[ i ][ j ][ k ] = 0;
                default:
                    break;
                }
            }
        }
    }
}

//２回目以上の距離場計算のためのVoxelの初期化
void Voxel::UpdateVoxel()
{
    ConstValue cv;
    int x(0),y(0),z(0);
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                if( (int)voxel[ i ][ j ][ k ] != 0 ) voxel[ i ][ j ][ k ] = inf;
            }
        }
    }
}

//境界条件
bool Voxel::isValid(int x, int y, int z){
    ConstValue cv;
    int xMax(0), yMax(0), zMax(0);
    xMax = cv.GetX();
    yMax = cv.GetY();
    zMax = cv.GetZ();
    if(x < 0 || x >= xMax) return false;
    if(y < 0 || y >= yMax) return false;
    if(z < 0 || z >= zMax) return false;
    return true;
}