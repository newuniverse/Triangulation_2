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
    //VDTで距離場計算
    Vdt();
    cout << "end propagation"<<endl;
    
    //Distance fieldの結果を書き出し
    if(cv.GetDim() == 3) WriteBinaryFile(voxel, "DistanceField");//3D
    if(cv.GetDim() == 2) WriteTextFile(voxel, "DistanceField"); //2D
    
    int counter(0);
    //forward passでlocal maximumを探す
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                //if( voxel[i][j][k] + 1.0 >= 0.01 ){ //除外されていないボクセル,除外は-1になるので
                    int xx = i; int yy = j; int zz = k; float r(0);
                    //do{} while (FindLocalMaximums(voxel, xx, yy, zz, r) != true);  //propagation方向で探す
                if( FindLocalMaximum( voxel, xx, yy, zz, r ) == true && labelLayer[ i ][ j ][ k ] > 2){
                    //RemoveFromSearchTarget(xx, yy, zz, r);
                    x_temp_center.push_back( xx );
                    y_temp_center.push_back( yy );
                    z_temp_center.push_back( zz );
                    rad_temp_center.push_back( r );
                    counter++;
                }
                /*
                    if( r >= minimumRadius && labelLayer[ i ][ j ][ k ] > 2 ) {
                        RemoveFromSearchTarget( xx, yy, zz, r );  //見つかったら一定範囲を探索範囲から消す
                        x_temp_center.push_back( xx );
                        y_temp_center.push_back( yy );
                        z_temp_center.push_back( zz );
                        rad_temp_center.push_back( r );
                        //cout << "xf = " << xx << "yf = " << yy << "zf = " << zz << "rf = "<< r << endl;
                    }
                 */
                //}
            }
        }
    }
    
    cout << "end forward pass" << endl;
    for (int i = 0; i < (int)x_temp_center.size(); i++)
    {
        int x = x_temp_center[ i ];
        int y = y_temp_center[ i ];
        int z = z_temp_center[ i ];
        voxel[x][y][z] = -1.0;
    }

    if(cv.GetDim() == 3) WriteBinaryFile(voxel, "Matching");
    if(cv.GetDim() == 2) WriteTextFile(voxel, "Matching");
       //pairで整理
    cout << "begin matching!" <<endl;
    cout << "number = " << x_temp_center.size() << "; counter = "<< counter <<endl;
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
    
    //## ここまででラベルでソートされている ##//
    
    for (int i = 0; i < (int)data.size(); i++) {
        cout << data[i].first << " : " << data[i].second << endl;
    }
    
    //## labelに沿ったマッチング ##//
    std::vector<std::pair<int, int> >::iterator it; //(label, index)のイテレータ
    it = data.begin();
    
    int it_counter(0);
    int head(0);
    int _maxRadiusIndex(0);
    int* _coordinate = new int[3];
    
    for (int i = 0; i < 3; i ++) {
        _coordinate[i] = 0;
    }
    
    while (it != data.end())
    {
        head = it_counter;
        int _label = it->first;
        float _maxRadius = rad_temp_center[it->second];
        
        //####
        std::vector< std::pair< float, int > > labeledSubGroup; //ラベルごとの再近似用円のサブグループ(radius, index)を格納
        labeledSubGroup.push_back( std::pair<float, int >( rad_temp_center[it->second], it->second ) );
        //####
        
        _maxRadiusIndex = it->second;
        _coordinate[0] = x_temp_center[it->second];
        _coordinate[1] = y_temp_center[it->second];
        _coordinate[2] = z_temp_center[it->second];
        
        //同じラベルを持つ中で、最大の半径をもつ球の半径と座標をまず探す
        while (it->first == _label)
        {
            ++it;
            ++it_counter;
            
            labeledSubGroup.push_back( std::pair<float, int>( rad_temp_center[it->second], it->second ) );
            
            if (rad_temp_center[it->second] > _maxRadius) {
                _maxRadius = rad_temp_center[it->second];
                _maxRadiusIndex = it->second;
                _coordinate[0] = x_temp_center[it->second];
                _coordinate[1] = y_temp_center[it->second];
                _coordinate[2] = z_temp_center[it->second];
            }
        }
        
        std::sort( labeledSubGroup.rbegin(), labeledSubGroup.rend()/*, std::greater< float >()*/ );//半径でソート
        
        for (int i = 0; i < (int)labeledSubGroup.size(); i++) {
            cout << "subgroup = "<< labeledSubGroup[i].first << " : " << labeledSubGroup[i].second << endl;
        }
        
        //もう一度RemoveFromTargetを大きい半径から，するためvoxelを再度初期化
        //if(cv.GetDim() == 3) ReadBinaryFile(cv.GetFileName);
        //if(cv.GetDim() == 2) ReadFile(cv.GetFileName);
        
        std::vector<std::pair<float, int> >::iterator subit;
        subit = labeledSubGroup.begin();
        
        //forward & backward pathで見つけた最大半径の球からRemoveFromTargetをしなおして，近似精度を高める
        float spheSumVolume(0);
        while ( subit != labeledSubGroup.end() /*&& spheSumVolume <= volume[ _label ]*/ ){
            //RemoveFromSearchTarget( x_temp_center[subit->second], y_temp_center[subit->second], z_temp_center[subit->second], subit->first );
            cout <<  "r index= " << subit->second << " radius = " << subit->first << endl;
            //##ここから　Distance fieldを使った球近似の再修正 ##//
            if (cv.GetDim() == 2) spheSumVolume += M_PI * pow( subit->first, 2 );
            if (cv.GetDim() == 3) spheSumVolume += 4 * M_PI * pow( subit->first, 3 ) / 3;
            
            cout << "sphere sum = " <<spheSumVolume << " ;label"<< _label <<" true volume = " << volume[ _label ];
            
            
            
            //あるlabelに登録されている半径が0の場合(近似条件に合わなかった気孔 ContainRate < 0.5)
            if(radius[ _label ] == 0) {
                cout << "pushback0" << endl;
                x_center.push_back(x_temp_center[ subit->second ]);
                y_center.push_back(y_temp_center[ subit->second ]);
                z_center.push_back(z_temp_center[ subit->second ]);
                radius.push_back(rad_temp_center[ subit->second ]);
            }
            
            //近似されていて登録済みの場合
            else{
                //まず重心と体積保存近似を消し去る、0にするのはあとで吐き出さない条件で使う
                cout << "pushback1" << endl;
                x_center[ _label ] = 0;
                y_center[ _label ] = 0;
                z_center[ _label ] = 0;
                radius[ _label ] = 0;
                x_center.push_back(x_temp_center[ subit->second ]);
                y_center.push_back(y_temp_center[ subit->second ]);
                z_center.push_back(z_temp_center[ subit->second ]);
                radius.push_back(rad_temp_center[ subit->second ]);
            }
            ++subit;
        }
        
        labeledSubGroup.clear();
        /*
        //閾値に使う際は最大半径の半分
        cout << "\n";
        cout << "maxRadius = " << _maxRadius << endl;
        cout << "maxRadiusLabel = "<< _maxRadiusIndex << endl;
        cout << "max X = " << _coordinate[0] <<endl;
        
        //同ラベルの先頭に戻ってコンストレインをかける
        for (int i = head; i < it_counter; i++)
        {
            float xx = (float)x_temp_center[data[i].second] - (float)_coordinate[0];
            float yy = (float)y_temp_center[data[i].second] - (float)_coordinate[1];
            float zz = (float)z_temp_center[data[i].second] - (float)_coordinate[2];
            float dis = sqrt(xx*xx + yy*yy + zz*zz);
            
            cout << "X = " << x_temp_center[data[i].second] << endl;
            
            //最大半径を持つ球の半径以上離れている場合は半径閾値は使わない
            if(dis > (_maxRadius * 1.2) && data[i].first > 2){
                cout << "far away  :"<<"label = "<< data[i].first << ", index = "<<data[i].second<<", " << "radius = " << rad_temp_center[data[i].second] <<endl;
                voxel[x_temp_center[data[i].second]][y_temp_center[data[i].second]][z_temp_center[data[i].second]] = -100;//local maximumの表示
                
                //##ここから　Distance fieldを使った球近似の再修正 ##//
                //あるlabelに登録されている半径が0の場合(近似条件に合わなかった気孔 ContainRate < 0.5)
                if(radius[data[i].first] == 0) {
                    x_center.push_back(x_temp_center[data[i].second]);
                    y_center.push_back(y_temp_center[data[i].second]);
                    z_center.push_back(z_temp_center[data[i].second]);
                    radius.push_back(rad_temp_center[data[i].second]);
                }
                //近似されていて登録済みの場合
                else{
                    //まず重心と体積保存近似を消し去る、0にするのはあとで吐き出さない条件で使う
                    x_center[data[i].first ] = 0;
                    y_center[data[i].first ] = 0;
                    z_center[data[i].first ] = 0;
                    radius[data[i].first] = 0;
                    x_center.push_back(x_temp_center[data[i].second]);
                    y_center.push_back(y_temp_center[data[i].second]);
                    z_center.push_back(z_temp_center[data[i].second]);
                    radius.push_back(rad_temp_center[data[i].second]);
                }
            }
            //最大半径閾値使う
            else if(rad_temp_center[ data[i].second ] > (3*_maxRadius/5) && data[i].first > 2) {
                cout << "very close:"<<"label = "<< data[i].first << ", index = "<<data[i].second<<", "<< "radius = " << rad_temp_center[data[i].second] <<endl;
                voxel[x_temp_center[data[i].second]][y_temp_center[data[i].second]][z_temp_center[data[i].second]] = -100;
                //##ここから　Distance fieldを使った球近似の再修正 ##//
                //あるlabelに登録されている半径が0の場合(近似条件に合わなかった気孔)
                if(radius[data[i].first] == 0) {
                    x_center.push_back(x_temp_center[data[i].second]);
                    y_center.push_back(y_temp_center[data[i].second]);
                    z_center.push_back(z_temp_center[data[i].second]);
                    radius.push_back(rad_temp_center[data[i].second]);
                }
                //近似されていて登録済みの場合
                else{
                    //まず重心と体積保存近似を消し去る、0にするのはあとで吐き出さない条件で使う
                    x_center[data[i].first ] = 0;
                    y_center[data[i].first ] = 0;
                    z_center[data[i].first ] = 0;
                    radius[data[i].first] = 0;
                    x_center.push_back(x_temp_center[data[i].second]);
                    y_center.push_back(y_temp_center[data[i].second]);
                    z_center.push_back(z_temp_center[data[i].second]);
                    radius.push_back(rad_temp_center[data[i].second]);
                }
            }
        }
       */
    }
         
    delete[] _coordinate;
    if(cv.GetDim() == 3) WriteBinaryFile(voxel, "Matching");
    if(cv.GetDim() == 2) WriteTextFile(voxel, "Matching");
    cout << "end distance field!"<<endl;
}


bool Voxel::FindLocalMaximum(float ***table, int &x, int &y, int &z, float &dis)
{
    int qx(0), qy(0), qz(0);
    bool flag(true);
    
    for( int i = 0; i < 25; i++ )
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
        
        if( table[ qx ][ qy ][ qz ] <= table[ x ][ y ][ z ] && table[ x ][ y ][ z ] > 0.0)
        {
            dis = table[ x ][ y ][ z ];
        }else flag = false;
    }
    cout << "flag = " << flag << " x = " << x << " y = " << y << " z = " << z <<endl;
    return flag;
}

//ローカルマキシマムなボクセルを見つける(Forward Pass)
bool Voxel::FindLocalMaximums(float *** table, int& x, int& y, int& z, float& dis)
{
    //cout << "begin find local maximum!"<<endl;
    int qx(0), qy(0), qz(0);
    int local_max_coordinate[3];
    float local_max_dis(0);
    bool flag(true);
    for ( int i = 0; i < 25; i++ ) {
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
                voxel[x + xx][y + yy][z + zz] = 1000.0;
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
    
    if(cv.GetDim() == 3) ReadBinaryFile(cv.GetFileName());
    if(cv.GetDim() == 2) ReadFile(cv.GetFileName());
    
    InitVoxel();
    
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
void Voxel::InitVoxel(){
    ConstValue cv;
    int x(0),y(0),z(0);
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    
    for(int i = 0; i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                switch((int)voxel[i][j][k])
                {
                    case 0:
                        voxel[i][j][k] = inf;
                        break;
                    case 1:
                        voxel[i][j][k] = 0;
                    default:
                        break;
                }
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