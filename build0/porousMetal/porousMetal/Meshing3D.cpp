#include "Rendering.h"
float ** Rendering::newVerCoord;
int *** Rendering::verOnSphereIndex;
int ** Rendering::verOnTetraSurfaceIndex;
int *** Rendering::verOnTetraEdgeIndex;
float **** Rendering::hexaCoord;

void Rendering::Meshing3D(){
    
    Init3DMeshing();
    
    for ( int i = 0; i < triN; i++ )
    {
        if(  CheckNoInfiniteVertex(tri[ i ][ 0 ], tri[ i ][ 1 ], tri[ i ][ 2 ], tri[ i ][ 3 ]) == true ) //infinit vertexを持つ三角形は排除
        {
            float* v0 = ver[ tri[ i ][ 0 ] ];
            float* v1 = ver[ tri[ i ][ 1 ] ];
            float* v2 = ver[ tri[ i ][ 2 ] ];
            float* v3 = ver[ tri[ i ][ 3 ] ];
            float* voroVer = voro_ver[ i ];
            float r0 = radius[ tri[ i ][ 0 ] ];
            float r1 = radius[ tri[ i ][ 1 ] ];
            float r2 = radius[ tri[ i ][ 2 ] ];
            float r3 = radius[ tri[ i ][ 0 ] ];
            int pattern = CheckPattern(v0, v1, v2, v3, r0, r1, r2, r3, i);
            bool isVoroVerOutside = CheckVoroVerPosition( v0, v1, v2, v3, voroVer, i );
            if( pattern != IRREGULAR ) Generate_3DMesh( v0, v1, v2, v3, voroVer, i, pattern, isVoroVerOutside );//３つの円ともオーバーラップならメッシングしない
        }
    }
}

void Rendering::Generate_3DMesh(float *ver0, float *ver1, float *ver2, float *ver3, float *voro_ver, int tetra_index, int pattern, bool isVoroVerOutside){
    
    dvector voro( 3 );   //引数のvoronoi点を複製
    dvector* voro_edge = new dvector[ 6 ];//辺上にある，Voronoiとの交点
    dvector* ver = new dvector[ 4 ]; //引数の四面体頂点を複製
    dvector replaced_voro( 3 );   //鈍角三角形の時、球との交点を求める際に三角形外にあるvoronoi vertexを変位させて、ここに格納
    for( int i = 0 ; i < 6; i++ )
    {
        if( i < 4 ) ver[i].resize( 3 );
        voro_edge[ i ].resize( 3 );
    }
        
    
    for ( int i = 0 ; i < 3; i++ )
    {
        ver[ 0 ][ i ] = ver0[ i ];
        ver[ 1 ][ i ] = ver1[ i ];
        ver[ 2 ][ i ] = ver2[ i ];
        ver[ 3 ][ i ] = ver3[ i ];
        voro[ i ] = voro_ver[ i ];
    }
    
    dvector temp(3);
    
    if( isVoroVerOutside == true  )
    {
        int obtuseIndex = angle_attribute[ tetra_index ];
        dvector* v = new dvector[ 3 ];
        for( int i = 0 ; i < 3; i++ ) ver[i].resize( 3 );
        int counter(0);
        for ( int j = 0; j < 4; j++) {
            if( j != obtuseIndex ){
                v[ counter ] = ver[ j ];
                counter++;
            }
        }
        temp = CalcVoroWithSurface( voro, v[ 1 ], v[ 2 ], v[ 3 ]);
        replaced_voro = temp;
    }
    temp.clear();
    temp.resize(3);
    if(isVoroVerOutside == false){
        hexaCoord[ tetra_index ][ 0 ][ 0 ] = voro_ver;
        //hexaCoord[ tetra_index ][ 0 ][ 1 ] =
    }
    /*
   //四面体の4面上にあるVoronoi点からの垂足
    for( int current = 0; current < 4 ; current++)
    {
       
        //if( verOnTetraSurfaceIndex[ tetra_index ][ current ] == -1 ){
            dvector temp(3);
            dvector* v = new dvector[ 3 ];
            for( int i = 0 ; i < 3; i++ ) ver[i].resize( 3 );
        
            int counter(0);
            for ( int j = 0; j < 4; j++) {
                if( j != current ){
                    v[ counter ] = ver[ j ];
                    counter++;
                }
            }
            
            temp = CalcVoroWithSurface( voro, v[ 1 ], v[ 2 ], v[ 3 ]);
        
            if( pattern == OVERLAP ){}                                                  //####
        
            for (int j = 0; j < 3; j++) newVerCoord[ crossPointIndex ][ j ] = temp[ j ];
        
            if( isVoroVerOutside == true  && angle_attribute[ tetra_index ] == current )
            {
                replaced_voro= temp;
            }
            verOnTetraSurfaceIndex[ tetra_index ][ current ] = crossPointIndex;
            crossPointIndex++;
        //}
    }
    
    //エッジ上にある点
    int cnter(0);
    for( int current = 0; current < 4 ; current++ )
    {

        dvector* temp = new dvector[ 3 ];
        dvector* v = new dvector[ 3 ];
        for( int i = 0 ; i < 3; i++ )
        {
            ver[i].resize( 3 );
            temp[ i ].resize(3);
        }
            
        int counter(0);
        for ( int j = 0; j < 4; j++) {
            if( j != current ){
                v[ counter ] = ver[ j ];
                counter++;
            }
        }
        
        for(int i = 0 ; i< 3;i++) {
            temp[ i ] = CalcVoroWithEdge( voro, ver[ current ], v[ i ] );
            for (int j = current; j < 3; j++){
                for( int k = 0; k< 3;k++ )
                {
                    newVerCoord[ crossPointIndex ][ k ] = temp[ j ][ k ];
                    voro_edge[ cnter ] = temp[ j ];
                    cnter++;
                }
                verOnTetraEdgeIndex[ tetra_index ][ current ][ j ] = crossPointIndex;
                verOnTetraEdgeIndex[ tetra_index ][ j + 1 ][ current ] = crossPointIndex;
                crossPointIndex++;
            }
        }
        if( pattern == OVERLAP ){}
    }
    
    //Sphere上にある点
    for( int current = 0; current < 4 ; current++ )
    {
        dvector* temp = new dvector[ 7 ];
        for (int i = 0; i < 7; i++) temp[ i ].resize( 3 );
        
        
    }
    */
    

}

dvector Rendering:: CalcVoroWithSurface(dvector voro, dvector v1, dvector v2, dvector v3 )
{
    dvector temp(2);
    dvector b(2);
    dmatrix A(2, 2);    //行列Aの計算
    for(int i = 0; i <3 ;i++)
    {
        A(0,0) += ( v1[ i ] - v3[ i ] ) * ( v2[ i ] - v1 [ i ]);
        A(0,1) += (v2[ i ] - v3[ i ]) * ( v2[ i ] - v1[ i ] );
        A(1,0) += (v1[ i ] - v3[ i ]) * ( v3[ i ] - v1[ i ] );
        A(1,1) += ( v2[ i ] - v3[ i ] ) * ( v3[ i ] - v1[ i ] );
        b[0] += ( voro[ i ] - v3[ i ] )*( v2[ i ] - v1[ i ] );
        b[1] += ( voro[ i ] - v3[ i ] )*( v3[ i ] - v1[ i ] );
    }
   
    //行列計算の例外処理
    try {
        solveMatrix(A, b);
    } catch (...) {
        cout << "solve Matrix failed!" <<endl;
        //return false;
    }
    temp = b[ 0 ]*v1 + b[ 1 ]*v2 + (1- b[0]- b[1])*v3;
    return temp;
}

dvector Rendering:: CalcVoroWithEdge(dvector voro, dvector s_v, dvector l_s_v)
{
    float s_coeff;
    dvector delta(3), delta_v(3);
    dvector temp(3);
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


dvector Rendering:: CalcOverlapCross(dvector voro, dvector ve, dvector vref, float rad)
{
    dvector result( 3 );
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
        result[i] = ve[i] + coeff * ( voro[ i ] - ve[i] ) / norm;
    }
    return result;
}

dvector Rendering:: CalcIntersectionWithSphere(dvector v_from, dvector v_to, float rad )
{
    dvector temp(3);
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




void Rendering::Init3DMeshing()
{
    //hexaは triN, hexaN, verN, coorの構造，直接レンダリング・吐き出せるように
    hexaCoord = new float***[ triN ];
    
    for (int i = 0; i < triN; i++)
        hexaCoord[ i ] = new float **[ 12 ];
    
    for (int i = 0; i < triN; i++) 
        for( int j = 0; j < 12; j++ )
        hexaCoord[ i ][ j ] = new float *[ 8 ];
    
    for (int i = 0; i < triN; i++)
        for( int j = 0; j < 12; j++ )
            for( int k = 0; k < 8; k++ )
                hexaCoord[ i ][ j ][ k ] = new float[ 3 ];
    
    /*
    newVerCoord = new float*[ 42*triN ];
    for( int i = 0; i < 42*triN; i++ )  newVerCoord[ i ] = new float[ 3 ];
    
    verOnSphereIndex = new int**[ triN ];
    verOnTetraSurfaceIndex = new int*[ triN ];
    verOnTetraEdgeIndex = new int**[ triN ];
    
    tri_attribute = new bool*[triN];
    angle_attribute = new int[ triN ];
    
    for (int i = 0; i < triN; i++) {
        verOnSphereIndex[ i ] = new int*[ 4 ];  //四頂点に7つずつ
        verOnTetraEdgeIndex[ i ] = new int*[ 4 ]; //四頂点に3つずつ
        
        verOnTetraSurfaceIndex[ i ] = new int[ 4 ]; //四頂点に1つずつ
        tri_attribute[i] = new bool[ 4 ];    //[0]頂点1  [1] [2] [3]0 or 1 or 2(normal, overlap, separate);
    }
    for (int i = 0; i < triN; i++)
    {
        for (int j = 0; j < 3; j++) {
            verOnSphereIndex[i][j] = new int[ 7 ];
            verOnTetraEdgeIndex[ i ][ j ] = new int[ 3 ];
        }
    }
    
    for (int i = 0; i < triN; i++)
    {
        angle_attribute[i] = -1;    //鈍角を持たない場合は-1を返す
        for (int j = 0; j < 4; j++)
        {
            verOnTetraSurfaceIndex[i][j] = -1;
            tri_attribute[i][j] = false;
            for (int k = 0; k < 7; k++)
            {
                verOnSphereIndex[i][j][k] = -1;
                if( k < 3 )verOnTetraEdgeIndex[ i ][ j ][ k ] = -1;
            }
        }
    }
    */
}

bool Rendering::CheckNoInfiniteVertex(int tri0, int tri1, int tri2, int tri3)
{
    bool _returnValue;
    if( tri0 != 0 && tri1 != 0 && tri2 != 0 && tri3!= 0 ) _returnValue = true ;
    else _returnValue = false;
    return _returnValue;
}

int Rendering::CheckPattern(float *ver0, float *ver1, float *ver2, float *ver3, float r0, float r1, float r2, float r3, int tetraIndex)
{
    int _return_value(NORMAL);
    float length_0_1(0);
    float length_0_2(0);
    float length_0_3(0);
    
    float length_1_2(0);
    float length_1_3(0);
    float length_2_3(0);
    
    int overlapCounter(0); //オーバーラップしている球の組みの数をカウント
    
    for (int j = 0; j < 3; j++)
    {
        length_0_1 += (ver0[ j ] - ver1[ j ])*(ver0[ j ] - ver1[ j ]);
        length_0_2 += (ver0[ j ] - ver2[ j ])*(ver0[ j ] - ver2[ j ]);
        length_0_3 += (ver0[ j ] - ver3[ j ])*(ver0[ j ] - ver3[ j ]);
        
        length_1_2 += (ver1[ j ] - ver2[ j ])*(ver1[ j ] - ver2[ j ]);
        length_1_3 += (ver1[ j ] - ver3[ j ])*(ver1[ j ] - ver3[ j ]);
        length_2_3 += (ver2[ j ] - ver3[ j ])*(ver2[ j ] - ver3[ j ]);
    }
    length_0_1 = sqrt(length_0_1);
    length_0_2 = sqrt(length_0_2);
    length_0_3 = sqrt(length_0_3);
    length_1_2 = sqrt(length_1_2);
    length_1_3 = sqrt(length_1_3);
    length_2_3 = sqrt(length_2_3);
    
    if( r0 + r1 >= length_0_1 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 2 ] = true;
        tri_attribute[ tetraIndex ][ 3 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    if( r0 + r2 >= length_0_2 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 1 ] = true;
        tri_attribute[ tetraIndex ][ 3 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    if( r0 + r3 >= length_0_3 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 1 ] = true;
        tri_attribute[ tetraIndex ][ 2 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    
    if( r1 + r2 >= length_1_2 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 0 ] = true;
        tri_attribute[ tetraIndex ][ 3 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    
    if( r1 + r3 >= length_1_3 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 0 ] = true;
        tri_attribute[ tetraIndex ][ 2 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    
    if( r2 + r3 >= length_2_3 )
    {
        _return_value = OVERLAP;
        tri_attribute[ tetraIndex ][ 0 ] = true;
        tri_attribute[ tetraIndex ][ 1 ] = true;
        //ver_oppsite_overlapSph.push_back()
        overlapCounter++;
    }
    if( overlapCounter >= 6) _return_value = IRREGULAR;  //############ここは４か6か？
    
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

//スカラ三重積を計算して、Voronoi点が四面体内外にあるかを判定
bool Rendering::CheckVoroVerPosition(float *ver0, float *ver1, float *ver2, float *ver3, float *voro, int tetraIndex)
{
    bool flag;
    dvector voro_to_v0( 3 );
    dvector voro_to_v1( 3 );
    dvector voro_to_v2( 3 );
    dvector voro_to_v3( 3 );
    
    dvector v3_to_v0( 3 );
    dvector v3_to_v1( 2 );
    dvector v3_to_v2( 2 );
    dvector v1_to_v0( 3 );
    dvector v1_to_v2( 3 );
    dvector v1_to_v3( 3 );
    dvector v0_to_v1( 3 );
    dvector v0_to_v2( 3 );
    dvector v0_to_v3( 3 );
    dvector v2_to_v0( 3 );
    dvector v2_to_v1( 3 );
    dvector v2_to_v3( 3 );

    for (int i = 0; i < 3; i++) {
        voro_to_v0[ i ] = ver0[ i ] - voro[ i ];
        voro_to_v1[ i ] = ver1[ i ] - voro[ i ];
        voro_to_v2[ i ] = ver2[ i ] - voro[ i ];
        voro_to_v3[ i ] = ver3[ i ] - voro[ i ];
        
        v3_to_v0[ i ] = ver0[ i ] - ver3[ i ];
        v3_to_v1[ i ] = ver1[ i ] - ver3[ i ];
        v3_to_v2[ i ] = ver2[ i ] - ver3[ i ];
        
        v1_to_v0[ i ] = ver0[ i ] - ver1[ i ];
        v1_to_v2[ i ] = ver2[ i ] - ver1[ i ];
        v1_to_v3[ i ] = ver3[ i ] - ver1[ i ];
        
        v2_to_v0[ i ] = ver0[ i ] - ver2[ i ];
        v2_to_v3[ i ] = ver3[ i ] - ver2[ i ];
        v2_to_v1[ i ] = ver1[ i ] - ver2[ i ];
        
        v0_to_v1[ i ] = ver1[ i ] - ver0[ i ];
        v0_to_v2[ i ] = ver2[ i ] - ver0[ i ];
        v0_to_v3[ i ] = ver3[ i ] - ver0[ i ];
    }
    
    dvector innerVecSgnPattern( 4 );
    dvector triVecSgnPattern( 4 );
    
    innerVecSgnPattern[ 3 ] = calculateScalar3product( voro_to_v0, voro_to_v1, voro_to_v2 );
    triVecSgnPattern[ 3 ] = calculateScalar3product(v3_to_v0, v3_to_v1, v3_to_v2);
    innerVecSgnPattern[ 0 ] = calculateScalar3product(voro_to_v1, voro_to_v2, voro_to_v3);
    triVecSgnPattern[ 0 ] = calculateScalar3product(v0_to_v1, v0_to_v2, v0_to_v3);
    innerVecSgnPattern[ 1 ] = calculateScalar3product(voro_to_v2, voro_to_v3, voro_to_v0);
    triVecSgnPattern[ 1 ] = calculateScalar3product(v1_to_v2, v1_to_v3, v1_to_v0);
    innerVecSgnPattern[ 2 ] = calculateScalar3product(voro_to_v3, voro_to_v0, voro_to_v1);
    triVecSgnPattern[ 2 ] = calculateScalar3product(v2_to_v3, v2_to_v0, v2_to_v1);
    
    int ver_oppsite_voro(-1);
    for (int i = 0; i < 4; i++) {
        if( innerVecSgnPattern[ i ] != triVecSgnPattern[ i ] ) ver_oppsite_voro = i;
    }
    
    if( ver_oppsite_voro >= 0 )
    {
        flag = true;
        angle_attribute[ tetraIndex ] = ver_oppsite_voro;
    }
    if( ver_oppsite_voro == -1 ) flag = false;
    if( ver_oppsite_voro == -2 ) cout << "Irregular Triangle index = " << tetraIndex << endl;
    return flag;
}



bool Rendering::calculateScalar3product(dvector v0, dvector v1, dvector v2)
{
    //正ならtrue,負ならfalseを返す仕様
    
    float volume = v0[ 0 ] * v1[ 1 ] * v2[ 2 ] + v0[ 1 ] * v1[ 2 ] * v2[ 0 ] + v0[ 2 ] * v1[ 0 ] * v2[ 1 ] - ( v0[ 2 ] * v1[1] * v2[ 0 ] + v0[ 1 ] * v1[ 0 ] * v2[ 2 ] + v0[ 0 ] * v1[ 2 ] * v2[ 1 ] );
    
    return ( volume > 0.0 ) ? true : false;
}
