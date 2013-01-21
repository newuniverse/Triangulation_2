#include "Rendering.h"
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glaux.lib" )
#include "ConstValue.h"
ConstValue cv;
//Keyboard押した時のフラグ
bool Rendering::TRI = false;
bool Rendering::VER = false;
bool Rendering::SEG = false;
bool Rendering::SPHERE = false;
bool Rendering::MESH = false;
bool Rendering::POINT = true;

//OpenGL操作用のパラメータ
int Rendering::mouseX;
int Rendering:: mouseY; 
float Rendering::qw = 0;
float Rendering::qx = 0;
float Rendering::qy = 0;
float Rendering::qz = 0;
float Rendering::zoom = 1;
float Rendering::shiftX = 0;
float Rendering::shiftY = 0;
int Rendering::width;
int Rendering::height;

int Rendering::verN;   //点の総数
int Rendering::triN;   //三角形の総数
int Rendering::segN;   //線分の総数
int Rendering::dimension;  //データの次元

float** Rendering::ver; //点(x, y, z)
int** Rendering::tri; //三角形 2D用(3点を格納)
float** Rendering::voro_ver;
int** Rendering::tri_neighbor;  //三角形の隣接三角形
float* Rendering::radius;   //半径

int Rendering::nextTriCounter = 0;  //三角形一個ずつ描画する debug用

void Rendering::OpenglRendering(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 850);
    glutCreateWindow("Triangulation");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutMouseFunc(Mouse);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
}

void Rendering::Draw(unsigned char type){
    Init();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    switch (type) {
        case 't':
            DrawTri();
            break;
        case 's':
            DrawSphere();
            break;
        case 'g':
            DrawVoroSeg();
            break;
        case 'v':
            DrawVoroVer();
            break;
        case 'p':
            DrawPoint();
            break;
        case 'm':
            DrawMesh();
        default:
            break;
    }
}

//メッシュの描画
void Rendering::DrawMesh(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE_LOOP);
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glColor3f(0, 0, 1);
    for (int i = 0; i < triN; i++) {    //各三角形についてのループ
        if(cross4Points_index[ i ][ 0 ][ 0 ] >= 0){
            cout << "drawing mesh" <<endl;
            for (int j = 0; j < 3; j++) {   //各頂点についてのループ
                if(tri_attribute[ i ][ j ] == false ){  //頂点がoverlapの向かいでなければ
                    for (int k = 0; k < 3; k++) {
                        if(k != j){ //2つの四辺形についてのループ
                            //if( angle_attribute[ i ] != j ){    //頂点が鈍角でなければ
                                
                                glColor3f(0, 0, 1);
                                glBegin(GL_POLYGON);
                            
                                glVertex3fv(voro_ver[i]);   //voronoi点
                                int _index = cross4Points_index[ i ][ j ][ 0 ];
                                glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //voro とedgeの交点
                                                        
                                _index = cross4Points_index[ i ][ k ][ j + 1 ];
                                glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //球とedgeの交点
                            
                                _index = cross4Points_index[ i ][ k ][ k + 1];
                                glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //球とvoro沿線交点
                            
                                glEnd();
                           // }
                        }
                    }
                }else{  //頂点がoverlapの向かいであれば
                    int overlapPointIndex = cross4Points_index[ i ][ j ][ 0 ];
                    //std::vector<int> index(2);
                    int index[2];
                    int counter(0);
                    int k(0);
                    while (k < 3) {
                        if(k != j){
                            index[counter] = cross4Points_index[ i ][ k ][ k + 1] ;
                            counter++;
                        }
                        k++;
                    }
                    for (int k = 0; k < 3; k++) {
                        if(k != j){
                            //index.push_back( cross4Points_index[ i ][ k ][ k + 1] );
                        }
                    }
                    glColor3f(1, 0, 1);
                    glBegin(GL_POLYGON);
                    glVertex3fv(voro_ver[i]);   //voronoi点
                    glVertex3f( cross4Points_coor[ index[0] ][ 0 ], cross4Points_coor[ index[0] ][ 1 ], cross4Points_coor[ index[0] ][ 2 ] );
                    cout << cross4Points_coor[ index[0] ][ 0 ] <<endl;
                    
                    glVertex3f(cross4Points_coor[ overlapPointIndex ][ 0 ], cross4Points_coor[ overlapPointIndex ][ 1 ], cross4Points_coor[ overlapPointIndex ][ 2 ]);
                    
                    glVertex3f( cross4Points_coor[ index[1] ][ 0 ], cross4Points_coor[ index[1] ][ 1 ], cross4Points_coor[ index[ 1 ] ][ 2 ] );
                    glEnd();
                }
            }
        }
    }
}

//球の描画
void Rendering::DrawSphere(){   //まだ2D版
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_LIGHTING);
    float red;
    glColor3f(0.3, 0.1, 0.8);
    
    GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    
    if( cv.GetDim() == 3 )
    {
        for (int i = 1; i <= verN; i++)
        {
            cout << "radius sum =" << radius[ i ] <<endl;
            glPushMatrix();
            glTranslatef(ver[ i ][ 0 ], ver[ i ][ 1 ], ver[ i ][ 2 ] );
            glutSolidSphere( radius[ i ] , 16, 16);
            glPopMatrix();
            /*
            for (float th = 0.0; th <= 360.0; th = th + 5.0)
            {
                for (float phi = 0.0; phi <= 180.0; phi = phi + 5.0) {
                    float th2 = th + 15.0;
                    float th_rad = th / 180.0 * M_PI;
                    float th2_rad = th2 / 180.0 * M_PI;
                
                    float phi2 = phi + 15.0;
                    float phi_rad = phi / 180.0 * M_PI;
                    float phi2_rad = phi2 / 180.0 * M_PI;
                
                    float x1 = radius[ i ] * cos( th_rad ) * sin( phi_rad );
                    float x2 = radius[ i ] * cos( th2_rad ) * sin( phi_rad );
                    float x3 = radius[ i ] * cos( th2_rad ) * sin( phi2_rad );
                    float x4 = radius[ i ] * cos( th_rad ) * sin( phi2_rad );
                
                    float y1 = radius[ i ] * sin( th_rad ) * sin( phi_rad );
                    float y2 = radius[ i ] * sin( th2_rad ) * sin( phi_rad );
                    float y3 = radius[ i ] * sin( th2_rad ) * sin( phi2_rad );
                    float y4 = radius[ i ] * sin( th_rad ) * sin( phi2_rad );
                
                    float z1 = radius[ i ] *  cos( phi_rad );
                    float z2 = radius[ i ] *  cos( phi_rad );
                    float z3 = radius[ i ] *  cos( phi2_rad );
                    float z4 = radius[ i ] *  cos( phi2_rad );
                
                    glBegin(GL_POLYGON);
                    glVertex3f( ver[ i ][ 0 ], ver[ i ][ 1 ], ver[ i ][ 2 ] );
                    glVertex3f( ver[ i ][ 0 ] + x1, ver[ i ][ 1 ] + y1, ver[ i ][ 2 ] + z1 );
                    glVertex3f( ver[ i ][ 0 ] + x2, ver[ i ][ 1 ] + y2, ver[ i ][ 2 ] + z2 );
                    glVertex3f( ver[ i ][ 0 ] + x3, ver[ i ][ 1 ] + y3, ver[ i ][ 2 ] + z3 );
                    glVertex3f( ver[ i ][ 0 ] + x4, ver[ i ][ 1 ] + y4, ver[ i ][ 2 ] + z4 );
                    glEnd();
                }
            }*/
        }
    }
    if( cv.GetDim() == 2 ){
        for(int i = 0; i < verN; i++ ){
            for (float th1 = 0.0;  th1 <= 360.0;  th1 = th1 + 1.0)
            {
                if (i < verN/4) {
                    red = 0.2;
                }else if (red < 2*verN/4){
                    red = 0.4;
                }else if(red < 3*verN/4){
                    red = 0.6;
                }else{
                    red = 0.8;
                }
                //glColor3f(red, (float)i/verN, 1 - (float)i/verN);
                float th2 = th1 + 15.0;
                float th1_rad = th1 / 180.0 * M_PI;
                float th2_rad = th2 / 180.0 * M_PI;
                float x1 = radius[i] * cos(th1_rad);
                float y1 = radius[i] * sin(th1_rad);
                float x2 = radius[i] * cos(th2_rad);
                float y2 = radius[i] * sin(th2_rad);
                glBegin(GL_TRIANGLES);
                glVertex3f( ver[i][0], ver[i][1], .0 );
                glVertex3f( x1+ver[i][0], y1+ver[i][1], .0 );
                glVertex3f( x2+ver[i][0], y2+ver[i][1], .0);
                glEnd();
            }
        }
    }
}

//Delaunay三角形の描画
void Rendering::DrawTri() 
{
    glLineWidth(1);
    glColor3f(0, 1.0, 0.0);
    for(int i = 0; i < triN ; i++){
        int *t = tri[i];
        glBegin(GL_TRIANGLES);
        if(t[0] != 0 && t[1] != 0 && t[2] != 0){
            glVertex3fv(ver[t[0]]);
            glVertex3fv(ver[t[1]]);
            glVertex3fv(ver[t[2]]);
        }
        glEnd();
    }
    //debug用
    //今の三角形のvoronoi vertexを描く
    glPointSize(5);
    glColor3f(1, 0.3, 0.0);
    float *v = voro_ver[nextTriCounter];
    glBegin(GL_POINTS);
    glVertex3f(v[0], v[1], v[2]);
    glEnd();
    
    //今の三角形を描く
    glColor3f(1, 0.0, 0.0);
    int *t = tri[nextTriCounter];
    glBegin(GL_TRIANGLES);
    if(t[0] != 0 && t[1] != 0 && t[2] != 0){
        glVertex3fv(ver[t[0]]);
        glVertex3fv(ver[t[1]]);
        glVertex3fv(ver[t[2]]);
    }
        glEnd();
    /*
    //今の三角形のメッシュを描く
    int i(0);
    i = nextTriCounter;
    if(cross4Points_index[ i ][ 0 ][ 0 ] >= 0){
        cout << "drawing mesh" <<endl;
        for (int j = 0; j < 3; j++) {   //各頂点についてのループ
            if(tri_attribute[ i ][ j ] == false ){  //頂点がoverlapの向かいでなければ
                for (int k = 0; k < 3; k++) {
                    if(k != j){ //2つの四辺形についてのループ
                        if( angle_attribute[ i ] != j ){    //頂点が鈍角でなければ
                        
                        glColor3f(0, 0, 1);
                        //glBegin(GL_POLYGON);
                        glBegin(GL_LINE_LOOP);
                        glVertex3fv(voro_ver[i]);   //voronoi点
                        int _index = cross4Points_index[ i ][ j ][ 0 ];
                        glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //voro とedgeの交点
                        
                        _index = cross4Points_index[ i ][ k ][ j + 1 ];
                        glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //球とedgeの交点
                        
                        _index = cross4Points_index[ i ][ k ][ k + 1];
                        glVertex3f( cross4Points_coor[ _index ][ 0 ], cross4Points_coor[ _index ][ 1 ], cross4Points_coor[ _index ][ 2 ] );  //球とvoro沿線交点
                        
                        glEnd();
                        }
                    }
                }
            }else{  //頂点がoverlapの向かいであれば
                int overlapPointIndex = cross4Points_index[ i ][ j ][ 0 ];
                //std::vector<int> index(2);
                int index[2];
                int counter(0);
                int k(0);
                while (k < 3) {
                    if(k != j){
                        index[counter] = cross4Points_index[ i ][ k ][ k + 1] ;
                        counter++;
                    }
                    k++;
                }
                for (int k = 0; k < 3; k++) {
                    if(k != j){
                        //index.push_back( cross4Points_index[ i ][ k ][ k + 1] );
                    }
                }
                //glColor3f(1, 0, 1);
                glBegin(GL_POLYGON);
                glVertex3fv(voro_ver[i]);   //voronoi点
                glVertex3f( cross4Points_coor[ index[0] ][ 0 ], cross4Points_coor[ index[0] ][ 1 ], cross4Points_coor[ index[0] ][ 2 ] );
                cout << cross4Points_coor[ index[0] ][ 0 ] <<endl;
                
                glVertex3f(cross4Points_coor[ overlapPointIndex ][ 0 ], cross4Points_coor[ overlapPointIndex ][ 1 ], cross4Points_coor[ overlapPointIndex ][ 2 ]);
                
                glVertex3f( cross4Points_coor[ index[1] ][ 0 ], cross4Points_coor[ index[1] ][ 1 ], cross4Points_coor[ index[ 1 ] ][ 2 ] );
                glEnd();
            }
        }
    }

    /*
     glColor3f(0, 0.0, 1.0);
     for (int j = 0; j < 3; j++) {
     if(tri[tri_neighbor[nextTriCounter][j]][0] != 0 && tri[tri_neighbor[nextTriCounter][j]][1] != 0 && tri[tri_neighbor[nextTriCounter][j]][2] != 0){
     glBegin(GL_LINES);
     glVertex3f(voro_ver[nextTriCounter][0], voro_ver[nextTriCounter][1], voro_ver[nextTriCounter][2]);
     glVertex3f(voro_ver[tri_neighbor[nextTriCounter][j]][0], voro_ver[tri_neighbor[nextTriCounter][j]][1], voro_ver[tri_neighbor[nextTriCounter][j]][2]);
     glEnd();
     }
     }
     
    int debug = 0;
    glLineWidth(3);
    glColor3f(0.1, 0.5, 0.8);
    glBegin(GL_POINTS);
    if(t[0] != 0 && t[1] != 0 && t[2] != 0){
        glVertex3fv(ver[t[debug]]);
    }
    glEnd();
    glColor3f(0.8, 0.5, 0.1);
    int *n_t = tri_neighbor[nextTriCounter];
    glBegin(GL_TRIANGLES);
    if(t[0] != 0 && t[1] != 0 && t[2] != 0){
        glVertex3fv(ver[tri[n_t[debug]][0]]);
        glVertex3fv(ver[tri[n_t[debug]][1]]);
        glVertex3fv(ver[tri[n_t[debug]][2]]);
    }
    glEnd();
    glColor3f(0.5, 0.8, 0.1);
    glBegin(GL_POINTS);
    if(t[0] != 0 && t[1] != 0 && t[2] != 0){
        glVertex3fv(ver[tri[n_t[debug]][debug]]);
    }
    glEnd();*/
    nextTriCounter++;
    cout << "tri number = "<<nextTriCounter << endl;
}

//Voronoi Vertexを描画
void Rendering::DrawVoroVer(){
    glColor3f(1, 0.3, 0.0);
    glPointSize(5);
    for (int i = 0; i < triN; i++) {
        if(tri[i][0] != 0 && tri[i][1] != 0 && tri[i][2] != 0){
            float *v = voro_ver[i];
            glBegin(GL_POINTS);
            glVertex3f(v[0], v[1], v[2]);
            glEnd();
         }
    }
}

//Votonoi segment描画
void Rendering::DrawVoroSeg()
{
    glLineWidth(3);
    glColor3f(1, 0.3, 0.0);
    for (int i = 0; i < triN; i++) {
        if(tri[i][0] != 0 && tri[i][1] != 0 && tri[i][2] != 0){
            for (int j = 0; j < 3; j++) {
                if(tri[tri_neighbor[i][j]][0] != 0 && tri[tri_neighbor[i][j]][1] != 0 && tri[tri_neighbor[i][j]][2] != 0){//infinite vertexを含む三角形は使わない
                    glBegin(GL_LINES);
                    glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                    glVertex3f(voro_ver[tri_neighbor[i][j]][0], voro_ver[tri_neighbor[i][j]][1], voro_ver[tri_neighbor[i][j]][2]);
                    glEnd();
                }
            }
        }
    }
}

//球の中心の描画
void Rendering::DrawPoint(){
    glPointSize(1);
    glColor3f(0, 1.0, 0.0);
    for(int i = 0; i < triN ; i++){
        int *t = tri[i];
        glBegin(GL_POINTS);
        if(t[0] != 0 && t[1] != 0 && t[2] != 0){
            glVertex3fv(ver[t[0]]);
            glVertex3fv(ver[t[1]]);
            glVertex3fv(ver[t[2]]);
        }
        glEnd();
    }
}

//OpenGL操作用パラメータ初期化
void Rendering::InitValues()
{
    qw = 1;
    qx = qy =  qz = 0;
    zoom = 1;
    shiftX = shiftY = 0;
    POINT = true;
    TRI = false;
    VER = false;
    SEG = false;
    SPHERE = false;
}

//Keyboard function
void Rendering::Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 't':
            TRI = !TRI;
            Display();
            break;
        case 's':
            SPHERE = !SPHERE;
            Display();
            break;
        case 'g':
            SEG = !SEG;
            Display();
            break;
        case 'v':
            VER = !VER;
            Display();
            break;
        case 'p':
            POINT = !POINT;
            Display();
            break;
        case 'm':
            MESH = !MESH;
            Display();
            break;
        default: break;
    }
}

//Mouse function
void Rendering::Mouse(int btn, int state, int x, int y)
{
    if(state == GLUT_DOWN){
        mouseX = x;
        mouseY = y;
    }
    else if(btn==GLUT_LEFT_BUTTON){
        float mx = -0.0025f*(x - mouseX)/zoom;
        float my = 0.0025f*(y - mouseY)/zoom;
        float c = (float)cos(my);
        float s = (float)sin(my);
        float rw = c*qw - s*qx;
        float rx = c*qx + s*qw;
        float ry = c*qy - s*qz;
        float rz = c*qz + s*qy;
        c = (float)cos(mx);
        s = (float)sin(mx);
        qw = c*rw - s*ry;
        qx = c*rx + s*rz;
        qy = c*ry + s*rw;
        qz = c*rz - s*rx;
        float n = (float)sqrt(qw*qw + qx*qx + qy*qy + qz*qz);
        if(n != 0){
            qw /= n;
            qx /= n;
            qy /= n;
            qz /= n;
        }
        else{
            qw = 1.0f;
            qx = qy = qz = 0.0f;
        }
        Display();
    }
    else if(btn==GLUT_RIGHT_BUTTON){
        zoom -= 0.0025f*(y - mouseY);
        if(zoom > 20.0f) zoom = 20.0f;
        else if(zoom < 0.05f) zoom = 0.05f;
        Reshape(width, height);
        Display();
    }
    else if(btn==GLUT_MIDDLE_BUTTON){
        shiftX += 0.25f*(x - mouseX)/zoom;
        shiftY += 0.25f*(y - mouseY)/zoom;
        Display();
    }
}

//視点が変わったら再描画
void Rendering::Reshape(int w, int h)
{
    width = w;
    height = h;
    float scale = 250.0;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (w <= h)
        glOrtho(-scale/zoom, scale/zoom,
                -scale * (GLfloat) h / (GLfloat) w /zoom,
                scale* (GLfloat) h / (GLfloat) w /zoom,
                -1000000.0, 100000.0);
    else
        glOrtho(-scale * (GLfloat) w / (GLfloat) h / zoom,
                scale * (GLfloat) w / (GLfloat) h /zoom,
                -scale/zoom, scale/zoom, -1000000.0, 1000000.0);
    glMatrixMode(GL_MODELVIEW);
}

//OpenGL表示パラメータ初期化
void Rendering::Init()
{
    GLfloat mat_specular[]={0.5, 0.5, 0.5, 1.0};
    GLfloat mat_diffuse[]={0.5, 0.5, 1.0, 1.0};
    GLfloat mat_ambient[]={0.5, 0.5, 0.5, 1.0};
    GLfloat mat_shininess={20.0};
    GLfloat light_ambient[]={0.1, 0.1, 0.1, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
    
    /* set up ambient, diffuse, and specular components for light 0 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    /* define material proerties for front face of all polygons */
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glShadeModel(GL_SMOOTH);   /* smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0);   /* enable light 0 */
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    glDepthFunc(GL_LEQUAL);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

//描画function
void Rendering::Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(shiftX, shiftY, 0);
    float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
    if(s != 0)
        glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
    
    if(TRI == true){
        Draw('t');
    }
    if (SPHERE == true) {
        Draw('s');
    }
    if (SEG == true){
        Draw('g');
    }
    if(VER == true){
        Draw('v');
    }
    if(POINT == true){
        Draw('p');
    }
    if(MESH == true){
        Draw('m');
    }
    glFlush();
    glutSwapBuffers();
}

//球の座標、半径データ読み込み3D ver
void Rendering::ReadCoordinate3D(string filename)
{
    ifstream fin;
    fin.open(filename.c_str());
    fin >> dimension;
    fin >> verN;
    cout << dimension << endl;
    cout << verN << endl;
    ver = new float*[verN + 1];
    radius = new float[verN + 1 ];
    for (int i = 0;  i <= verN; i++)
    {
		ver[i] = new float[3];
    }
    for(int i = 1; i <= verN; i++){
        float* v = ver[i];
        fin >> v[ 0 ];
        fin >> v[ 1 ];
        fin >> v[ 2 ]; 
        fin >> radius[ i ];
        radius[ i ] = sqrtf( radius[ i ] );
        cout << v[0] << " " << v[1] << " " << v[2] <<  " radius = " << radius[ i ] << endl;
    }
    fin >> triN;
    cout << triN << endl;
    tri = new int*[triN];
    tri_neighbor = new int*[triN];
    voro_ver = new float*[triN];
    for (int i = 0;  i < triN; i++)
    {
		tri[i] = new int[4];
        tri_neighbor[i] = new int[4];
        voro_ver[i] = new float[3];
    }
    
    cout << "triangle" <<endl;
    for(int i = 0; i < triN; i++){
        int* t = tri[i];
        fin >> t[0];
        fin >> t[1];
        fin >> t[2];
        fin >> t[3];
        cout << t[0] << " " << t[1] << " " << t[2] << " "<< t[3] << endl;
    }
    
    cout << "triangle neighbor" << endl;
    for(int i = 0; i < triN; i++){
        int* n = tri_neighbor[i];
        fin >> n[0];
        fin >> n[1];
        fin >> n[2];
        fin >> n[3];
        cout << n[0] << " " << n[1] << " " << n[2] << " "<< n[3] << endl;
    }

    cout << "Voronoi vertices" << endl;
    for(int i = 0; i < triN; i++){
        float* v = voro_ver[i];
        fin >> v[0];
        fin >> v[1];
        fin >> v[2];
        cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    fin.close();
    fin.clear();
}

//球の座標、半径データ読み込み2D ver
void Rendering::ReadCoordinate(string filename)
{
    ifstream fin;
    fin.open(filename.c_str());
    fin >> verN;
    fin >> triN;
    fin >> dimension;
    cout << verN << endl;
    cout << triN << endl;
    cout << dimension << endl;
    //cout << "vertex" <<endl;
    ver = new float*[verN];
    radius = new float[verN];
    for (int i = 0;  i < verN; i++)
    {
		ver[i] = new float[3];
    }
    tri = new int*[triN];
    tri_neighbor = new int*[triN];
    voro_ver = new float*[triN];
    for (int i = 0;  i < triN; i++)
    {
		tri[i] = new int[3];
        tri_neighbor[i] = new int[3];
        voro_ver[i] = new float[3];
    }
    for(int i=1; i<verN; i++){
        float* v = ver[i];
        fin >> v[0];
        fin >> v[1];
        v[2] = 0;   //fin >> v[2];  //3D
        fin >> radius[i];
        radius[i] = sqrtf(radius[i]);
        //cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    
    //cout << "triangle" <<endl;
    for(int i = 0; i < triN; i++){
        int* t = tri[i];
        fin >> t[0];
        fin >> t[1];
        fin >> t[2];
        //cout << t[0] << " " << t[1] << " " << t[2] << endl;
    }
    
    //cout << "triangle neighbor" << endl;
    for(int i = 0; i < triN; i++){
        int* n = tri_neighbor[i];
        fin >> n[0];
        fin >> n[1];
        fin >> n[2];
        //cout << n[0] << " " << n[1] << " " << n[2] << endl;
    }
    //cout << "Voronoi vertices" <<endl;
    for(int i = 0; i < triN; i++){
        float* v = voro_ver[i];
        fin >> v[0];
        fin >> v[1];
        //fin >> v[2];
        v[2] = 0;
        //cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    fin.close();
    fin.clear();
}

//デストラクタ
Rendering::~Rendering(){
    delete [] ver;
    delete [] tri;
    delete [] tri_neighbor;
    delete [] voro_ver;
    delete [] radius;
    delete [] cross4Points_index;
}






