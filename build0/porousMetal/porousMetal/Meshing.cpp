#include "Rendering.h"

std::vector< std::vector<float> > Rendering::cross4Points_coord;
int*** Rendering::cross4Points_index;
int Rendering::crossPointIndex;
float** Rendering::cross4Points_coor;

std::vector<int> Rendering::ver_oppsite_overlapSph;
bool** Rendering::tri_attribute;
int* Rendering::angle_attribute;


//メッシュ生成
void Rendering::Generate_Mesh(float* ver1, float* ver2, float* ver3, float* voro_ver, int tri_index, int pattern, bool isObtuse)
{
    dvector voro(3);   //引数のvoronoi点を複製
    dvector* ver = new dvector[3]; //引数の三角形頂点を複製
    dvector replaced_voro(3);   //鈍角三角形の時、球との交点を求める際に三角形外にあるvoronoi vertexを変位させて、ここに格納
    for( int i = 0 ; i < 3; i++ ) ver[i].resize(3);
    
    for ( int i = 0 ; i < 3; i++ )
    {
        ver[ 0 ][ i ] = ver1[ i ];
        ver[ 1 ][ i ] = ver2[ i ];
        ver[ 2 ][ i ] = ver3[ i ];
        voro[ i ] = voro_ver[ i ];
    }
    
#pragma region voro and edge intersections
    
    //0番目の頂点に対するvoronoi点と三角形のエッジの交点 [ tri index ][ vertex index ][ 0 ]は
    if( cross4Points_index[ tri_index ][ 0 ][ 0 ] == -1 )   //まだ登録未完かのチェック
    { //                 [三角形の番号][頂点番号][voro verとedgeの交点の格納場所]
        std::vector<float> vec(3);
        vec = CalcuCrossPointVoroWithEdge( ver[ 2 ], ver[ 1 ], voro );
        
        //cross4Points_coord.push_back( vec );    //計算した交点を入れる              //##*****************ここに問題アリ、正しくpush_backされていない***************##//
        
        //normalでなければ
        if( pattern != NORMAL )   
        {
            if (tri_attribute[ tri_index ][ 0 ] == true) {  //overlapの向かいの頂点であれば
                dvector ve(3);
                for (int i = 0; i < 3; i ++) {
                    ve[i] = vec[i];
                }
                vec = CalcuCrossOverlap( ve, ver[ 1 ], voro, radius[ tri[ tri_index ][ 1 ] ]);
            }
        }
        for (int i = 0; i < 3; i++) cross4Points_coor[crossPointIndex][i] = vec[i];
        
        //鈍角三角形であれば,edgeとの交点を新しいvoronoi vertex の位置とする
        if( isObtuse == true && angle_attribute[ tri_index ] == 0 )  
        {
            cout << "push in process"<< endl;
            for (int i = 0 ; i < 3; i++)
            {
                replaced_voro[ i ] = vec[ i ];
                //voro_ver[ i ] = vec[ i ];               /////#################################
            }
        }
        
        //cout << "cross point = "<< vec[0] << ", " << vec[1]<<", "<< vec[2]<< endl;
        cross4Points_index[ tri_index ][ 0 ][ 0 ] = crossPointIndex; //triに計算した交点のインデックスを登録
        
        //##  neighbor triへの登録  ##//
        
        crossPointIndex++;
    }
    
    //1番目の頂点
    if( cross4Points_index[ tri_index ][ 1 ][ 0 ] == -1 )
    {
        std::vector<float> vec(3);
        vec = CalcuCrossPointVoroWithEdge( ver[ 2 ], ver[ 0 ], voro );
        //cross4Points_coord.push_back( vec );    //計算した交点を入れる              //##*****************ここに問題アリ、正しくpush_backされていない***************##//
        
        if( pattern != NORMAL)   //normalでなければ
        {
            if (tri_attribute[ tri_index ][ 1 ] == true) { //overlapの向かいの頂点であれば
                dvector ve(3);
                for (int i = 0; i < 3; i ++) {
                    ve[i] = vec[i];
                }
                vec = CalcuCrossOverlap( ve, ver[ 2 ], voro, radius[ tri[ tri_index ][ 2 ] ]);
            }
        }
        for (int i = 0; i < 3; i++) cross4Points_coor[crossPointIndex][i] = vec[i];
        
        //鈍角三角形であれば,edgeとの交点を新しいvoronoi vertex の位置とする
        if( isObtuse == true && angle_attribute[ tri_index ] == 1 )
        {
            for (int i = 0 ; i < 3; i++) {
                replaced_voro[ i ] = vec[ i ];
                //voro_ver[ i ] = vec[ i ];               /////#################################
            }
        }
        
        //cout << "cross point = "<< vec[0] << ", " << vec[1]<<", "<< vec[2]<< endl;
        //cross4Points_coord.push_back( CalcuCrossPointVoroWithEdge( ver[2], ver[0], voro ) );
        cross4Points_index[ tri_index ][ 1 ][ 0 ] = crossPointIndex; 
        //##  neighbor triへの登録  ##//
        crossPointIndex++;
    }
    //2番目の頂点
    if( cross4Points_index[ tri_index ][ 2 ][ 0 ] == -1 )
    {
        std::vector<float> vec(3);
        vec = CalcuCrossPointVoroWithEdge( ver[ 0 ], ver[ 1 ], voro );
        //cross4Points_coord.push_back( vec );    //計算した交点を入れる              //##*****************ここに問題アリ、正しくpush_backされていない***************##//
        
        if( pattern != NORMAL)   //normalでなければ
        {
            if (tri_attribute[ tri_index ][ 2 ] == true) { //overlapの向かいの頂点であれば
                dvector ve(3);
                for (int i = 0; i < 3; i ++) {
                    ve[i] = vec[i];
                }
                vec = CalcuCrossOverlap( ve, ver[ 0 ], voro, radius[ tri[ tri_index ][ 0 ] ]);
            }
        }
        for (int i = 0; i < 3; i++) cross4Points_coor[crossPointIndex][i] = vec[ i ];
    
        //鈍角三角形であれば,edgeとの交点を新しいvoronoi vertex の位置とする
        if( isObtuse == true && angle_attribute[ tri_index ] == 2 )
        {
            for (int i = 0 ; i < 3; i++) {
                replaced_voro[ i ] = vec[ i ];
                //voro_ver[ i ] = vec[ i ];               /////#################################
            }
        }
        //cout << "cross point = "<< vec[0] << ", " << vec[1]<<", "<< vec[2]<< endl;
        //cross4Points_coord.push_back( CalcuCrossPointVoroWithEdge( ver[1], ver[0], voro ) );
        cross4Points_index[ tri_index ][ 2 ][ 0 ] = crossPointIndex; //tri番号登録
        
        //##  neighbor triへの登録  ##//
        
        crossPointIndex++;
    }
#pragma end rigion  voro and edge intersections
    
#pragma rigion begin intersection with spheres
    //球との交点(3つ)の計算と登録
    for (int i = 0; i < 3; i++) //頂点のインデックス
    {
        dvector* v_to = new dvector[3];
        dvector zeroVec(3);
        for (int i = 0; i < 3; i++)  zeroVec[ i ] = 0;
        cout << "voro ver coordinate before= "<<voro_ver[ 0 ]<< " " <<voro_ver[ 1 ] << " "<< voro_ver[ 2 ] << endl;
        
        
        for (int j = 0; j < 3; j++)
        {
            if ( j == i ) {
                if( angle_attribute[ tri_index ] != i ){
                    v_to[ j ] = voro; //同じインデックスのを voroと頂点でできた線分と球の頂点とする, 鋭角三角形ならば
                }else
                    if( isObtuse == true /*&& angle_attribute[ tri_index ] == i */){
                   cout << "replace voro ver" << endl;
                   v_to[ j ] = replaced_voro;
                   for(int l = 0; l < 3; l++) voro_ver[l] = replaced_voro[l];     //鈍角三角形ならばvoronoi vertexを移動
               }
            }
            
            if( j != i ) {
                for (int k = 0; k < 3; k++) {
                    if (k != i && k != j) v_to[ k ] = ver[ j ];
                }
            }
        }
        if(isObtuse == true){
            if(i == 0) v_to[ 0 ] = replaced_voro;
            if(i == 1) v_to[ 1 ] = replaced_voro;
            if(i == 2) v_to[ 2 ] = replaced_voro;
        }
        cout << "voro ver coordinate after= "<<voro_ver[ 0 ]<< " " <<voro_ver[ 1 ] << " "<< voro_ver[ 2 ] << endl;
        for (int j = 0; j < 3; j++)
        {
            cout << " v_to coordinate = "<<v_to[ j ][ 0 ]<< " " << v_to[ j ][ 1 ] << " "<< v_to[ j ][ 2 ] << endl;
        }

        
        for (int l = 0; l < 3; l++)
        {   
            if( cross4Points_index[ tri_index ][ i ][ l + 1 ] == -1 ) //この三点の登録先は [ 1 ~ 3 ]
            {
                std::vector<float> vec(3);
                vec =  CalcuCrossIntersectionWithSphere( ver[ i ], v_to[ l ], radius[ tri[ tri_index ][ i ] ] );
                
                for (int i = 0; i < 3; i++){
                    cross4Points_coor[crossPointIndex][ i ] = vec[ i ];
                }
                cross4Points_index[ tri_index ][ i ][ l + 1 ] = crossPointIndex;  //triに計算した交点のインデックスを登録
                //cross4Points_coord.push_back( vec );    //計算した交点を入れる              //##*************ここに問題アリ、正しくpush_backされていない*********##//
                //neiに登録は今のところしない
                cout << "cross 3point = "<< vec[ 0 ] << ", " << vec[ 1 ]<<", "<< vec[ 2 ]<< endl;
                
                crossPointIndex++;
            }
        }
        
    }
    cout<<endl;
    //if(isObtuse == true) getchar();
 //cout << "crossPointIndex = "<< crossPointIndex << endl; //debug用これが12の倍数ならOK!
    //ver_oppsite_overlapSph.clear();
}
#pragma end rigion intersection with spheres

//オーバーラップしている球の交点を求める
std::vector<float> Rendering::CalcuCrossOverlap(dvector ve, dvector vref, dvector voro, float rad)
{
    std::vector< float > temp( 3 );
    float coeff( 0 );
    float norm( 0 );
    for ( int i = 0; i < 3; i++ )
    {
        norm += pow( voro[ i ] - ve[ i ], 2 );
        coeff += pow( vref[ i ] - ve[ i ], 2 );
    }
    coeff = sqrt( pow( rad, 2 ) - coeff );
    
    norm = sqrt( norm );
    
    for ( int i = 0; i < 3; i++ )
    {
        temp[i] = ve[i] + coeff * ( voro[ i ] - ve[i] ) / norm;
    }
    return temp;
}

//## v0 = v_from + (v_to - v_from).normalized * radius でv0を求める
std::vector <float> Rendering::CalcuCrossIntersectionWithSphere(dvector v_from, dvector v_to, float rad)
{
    std::vector<float> temp(3);
    float norm(0);
    for ( int i = 0; i < 3; i++ )
    {
        norm += pow( v_to[ i ] - v_from[ i ], 2 );
    }
    
    norm = sqrt(norm);
    
    for (int i = 0; i < 3; i++)
    {
        temp[ i ] = v_from[ i ] + rad * ( v_to[i] - v_from[i] )/norm;
        //cout << "temp = " << temp[i] << ", ";
    }
    //cout << "\n";
    return temp;
}

//## v0 = s*v1 + ( 1 - s ) * v2; ( v0 - voro )・( v1 - v2 ) = 0　でv0を求める##//
std::vector<float> Rendering::CalcuCrossPointVoroWithEdge(dvector s_v, dvector l_s_v, dvector voro)
{
    float s_coeff;
    dvector delta(3), delta_v(3);
    std::vector<float> temp(3);
    delta = l_s_v - s_v;
    delta_v = l_s_v - voro;
    s_coeff = (delta[0]*delta_v[0] + delta[1]*delta_v[1] + delta[2]*delta_v[2])/(pow(delta[0], 2) + pow(delta[1], 2) + pow(delta[2], 2));
    
    for (int i = 0; i < 3; i++)
    {
        temp[ i ] = s_coeff * s_v[ i ] + ( 1 - s_coeff ) * l_s_v[ i ];
        //cout <<  temp[i] << ", ";
    }
    //cout << "\n";
    return temp;
}

//三角形のメッシングパターン検査enumを返す
int Rendering::CheckPattern(float *ver1, float *ver2, float *ver3, float r1, float r2, float r3, int triIndex,int label1, int label2, int label3)
{
    int _return_value(0);
    float length_1_2(0);//頂点1-2のつくるエッジの長さ
    float length_2_3(0);
    float length_3_1(0);
    int overlapCounter(0); //オーバーラップしている球の組みの数をカウント
    for (int j = 0; j < 3; j++) {
        length_1_2 += pow(ver1[j] - ver2[j], 2);
        length_2_3 += pow(ver2[j] - ver3[j], 2);
        length_3_1 += pow(ver3[j] - ver1[j], 2);
    }
    
    length_1_2 = sqrt(length_1_2);
    length_2_3 = sqrt(length_2_3);
    length_3_1 = sqrt(length_3_1);
    
    _return_value = NORMAL;
    //labelが一緒ならOverlapとみなし、違うならSaparateする
    if(r2 + r3 >= length_2_3){
        /*if(label2 != label3) _return_value = SEPARATE;
        else*/
        _return_value = OVERLAP;
        tri_attribute[ triIndex ][ 3 ] = true;
        tri_attribute[ triIndex ][ 0 ] = true;
        ver_oppsite_overlapSph.push_back(0);
        overlapCounter++;
    }
    
    if(r3 + r1 >= length_3_1){
        /*if(label3 != label1) _return_value = SEPARATE;
        else*/
        _return_value = OVERLAP;
        tri_attribute[ triIndex ][ 3 ] = true;
        tri_attribute[ triIndex ][ 1 ] = true;
        ver_oppsite_overlapSph.push_back(1);
        overlapCounter++;
    }
    
    if(r1 + r2 >= length_1_2){
        /*if(label1 != label2) _return_value = SEPARATE;
        else*/
        _return_value = OVERLAP;
        tri_attribute[ triIndex ][ 3 ] = true;
        tri_attribute[ triIndex ][ 2 ] = true;
        ver_oppsite_overlapSph.push_back(2);
        overlapCounter++;
    }
    
    if( overlapCounter == 3) _return_value = IRREGULAR; //３つの円が相互に交わる場合
    
    switch (_return_value) {
        case NORMAL:
            //cout << "NORMAL Tri!" << endl; break;
        case OVERLAP:
            //cout << "OVERLAP Tri!" << endl; break;
        case IRREGULAR:
            //cout << "3OVERLAPS Tri!" << endl; break;
        default:
            break;
    }
    
    return _return_value;
}

//鈍角三角形かの検査 trueかfalseを返す
bool Rendering::CheckObtuseTri(float* ver1, float *ver2, float* ver3, float* voro, int triIndex)
{
    
    //c2 > a2 + b2;
    bool _returnValue(false);
    
    dvector voro_to_v1( 2 );
    dvector voro_to_v2( 2 );
    dvector voro_to_v3( 2 );
    
    dvector v3_to_v1( 2 );
    dvector v3_to_v2( 2 );
    dvector v1_to_v2( 2 );
    dvector v1_to_v3( 2 );
    dvector v2_to_v3( 2 );
    dvector v2_to_v1( 2 );
    
    dvector innerVecSgnPattern( 3 );
    dvector triVecSgnPattern( 3 );
    for (int i = 0; i < 2; i++) {
        voro_to_v1[ i ] = ver1[ i ] - voro[ i ];
        voro_to_v2[ i ] = ver2[ i ] - voro[ i ];
        voro_to_v3[ i ] = ver3[ i ] - voro[ i ];
        
        v3_to_v1[ i ] = ver1[ i ] - ver3[ i ];
        v3_to_v2[ i ] = ver2[ i ] - ver3[ i ];
        v1_to_v2[ i ] = ver2[ i ] - ver1[ i ];
        v1_to_v3[ i ] = ver3[ i ] - ver1[ i ];
        v2_to_v3[ i ] = ver3[ i ] - ver2[ i ];
        v2_to_v1[ i ] = ver1[ i ] - ver2[ i ];
    }
    
    //外積の符号計算
    innerVecSgnPattern[ 0 ] = calculateVectorProductSgn(voro_to_v2, voro_to_v3 );
    triVecSgnPattern[ 0 ] = calculateVectorProductSgn(v1_to_v2, v1_to_v3);
    innerVecSgnPattern[ 1 ] = calculateVectorProductSgn(voro_to_v3, voro_to_v1 );
    triVecSgnPattern[ 1 ] = calculateVectorProductSgn(v2_to_v3, v2_to_v1);
    innerVecSgnPattern[ 2 ] = calculateVectorProductSgn(voro_to_v1, voro_to_v2 );
    triVecSgnPattern[ 2 ] = calculateVectorProductSgn(v3_to_v1, v3_to_v2);
    
    int ver_index;
    if( innerVecSgnPattern[ 0 ] != triVecSgnPattern[ 0 ] ) ver_index = 0;
    else if( innerVecSgnPattern[ 1 ] != triVecSgnPattern[ 1 ] ) ver_index = 1;
    else if (innerVecSgnPattern[ 2 ] != triVecSgnPattern[ 2 ]) ver_index = 2;
    else ver_index = -1;
    
    
    //ver_index = checkSgn( innerVecSgnPattern );
    if( ver_index >= 0 )
    {
        _returnValue = true;
        angle_attribute[ triIndex ] = ver_index;
    }
    if( ver_index == -1 ) _returnValue = false;
    if( ver_index == -2 ) cout << "Irregular Triangle index = " << triIndex << endl;
    
    return _returnValue;
}

bool Rendering::calculateVectorProductSgn(dvector v1, dvector v2)
{
    float s = v1[ 0 ] * v2[ 1 ] - v1[ 1 ]* v2[ 0 ];
    
    return ( s > 0 ) ? true : false;      //正数ならtrueを返し、負ならfalse
}

int Rendering::checkSgn(dvector vec)
{
    if( vec[ 0 ] == true && vec[ 1 ] == true && vec[ 2 ] == true ) return -1;   //Voronoi点が三角形内でレギュラーな場合
    else if(vec[ 0 ] == false && vec[ 1 ] == true && vec[ 2 ] == true) return 0;    //0番目頂点の向かいの領域にVoronoi点がはみ出ている
    else if(vec[ 0 ] == true && vec[ 1 ] == false && vec[ 2 ] == true) return 1;
    else if(vec[ 0 ] == true && vec[ 1 ] == true && vec[ 2 ] == false) return 2;
    else return -2;             //Voronoi点がS"領域内にいる場合はイレギュラー
}

//初期化
void Rendering::InitMeshing()
{

    //交点の座標
    cross4Points_coor = new float*[ 12*triN ]; //とりあえず重複許すと12*triN個になる
    for (int i = 0; i < 12*triN; i++) {
         cross4Points_coor[i] = new float[ 3 ];
    }
    
    cross4Points_index = new int**[triN]; //交点のインデックス
    tri_attribute = new bool*[triN];
    angle_attribute = new int[ triN ];
    for (int i = 0; i < triN; i++) {
        cross4Points_index[i] = new int*[3];
        tri_attribute[i] = new bool[ 4 ];    //[0]頂点1  [1] [2] [3]0 or 1 or 2(normal, overlap, separate);
    }
    for (int i = 0; i < triN; i++)
    {
        for (int j = 0; j < 3; j++) {
            cross4Points_index[i][j] = new int[4];
        }
    }
    
    //indexの初期化、登録無しのは-1が入っている
    for (int i = 0; i < triN; i++)
    {
        angle_attribute[i] = -1;    //鈍角を持たない場合は-1を返す
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                tri_attribute[i][k] = false;
                cross4Points_index[i][j][k] = -1;
            }
        }
    }
    
    //交点の座標
    cross4Points_coord.resize(12*triN);
    for (int i = 0; i < triN*6; i++) cross4Points_coord[i].resize(3);
    
}


void Rendering::Meshing(){
    
    InitMeshing();
    
    for ( int i = 0; i < triN; i++ )
    {
        if( tri[ i ][ 0 ] != 0 && tri[ i ][ 1 ] != 0 && tri[ i ][ 2 ] != 0 ) //infinit vertexを持つ三角形は排除
        {
            float* v1 = ver[ tri[ i ][ 0 ] ];
            float* v2 = ver[ tri[ i ][ 1 ] ];
            float* v3 = ver[ tri[ i ][ 2 ] ];
            float* voroVer = voro_ver[ i ];
            float r1 = radius[tri[i][0]];
            float r2 = radius[tri[i][1]];
            float r3 = radius[tri[i][2]];
            int pattern = CheckPattern( v1, v2, v3, r1, r2, r3, i, 0, 0, 0 );
            bool isVoroVerOutside = CheckObtuseTri( v1, v2, v3, voroVer, i );
            if( pattern != IRREGULAR )//３つの円ともオーバーラップならメッシングしない
            Generate_Mesh(v1, v2, v3, voroVer, i, pattern, isVoroVerOutside );
        }
    }
}

//neibour への登録
/*
 
 int v0_index = tri[ tri_index ][ 0 ]; //tri_index番目の三角形の 0 番目の頂点のインデックス
 int v1_index = tri[ tri_index ][ 1 ];
 int v2_index = tri[ tri_index ][ 2 ];
 int neiTri0_index = tri_neighbor[ tri_index ][ 0 ]; //tri_index番目の三角形の 0 番目の頂点向かいの隣接三角形のインデックス
 int neiTri1_index = tri_neighbor[ tri_index ][ 1 ];
 int neiTri2_index = tri_neighbor[ tri_index ][ 2 ];
 
for (int i = 0; i < 3; i++)
{
    if( tri[ neiTri0_index ][ i ] != ( v1_index & v2_index ) )
    {
        int _index = neiTri0_index;
        cross4Points_index[ _index ][ i ][ 0 ] = crossPointIndex;
    }
}

for (int i = 0; i < 3; i++)
{
    if( tri[ neiTri1_index ][ i ] != ( v0_index & v2_index ) )
    {
        int _index = neiTri1_index;
        cross4Points_index[ _index ][ i ][ 0 ] = crossPointIndex;
    }
}
for (int i = 0; i < 3; i++)
{
    if( tri[ neiTri2_index ][ i ] != ( v0_index & v1_index ) )
    {
        int _index = neiTri2_index;
        cross4Points_index[ _index ][ i ][ 0 ] = crossPointIndex;
    }
}*/

