//
//  OpneglRendering.cpp
//  porousMetal
//
//  Created by 鈕 臻禹 on 12/11/14.
//  Copyright (c) 2012年 東京大学. All rights reserved.
//

#include "OpenglRendering.h"
void Init();
int indicator = 0;
int nextDraw = 0;
int nextDrawVoronoi = 0;
int data_dimension;
int verN;
int triN;
int segN;

float (*ver)[3];
float (*rad);
int (*tri)[3];
int (*neighbor)[3];
float (*seg)[4];
int (*segIndex)[3];
float (*voro_ver)[3];
float (*intersection)[3][3];
float (*remainingE)[3][3];
float (*remainingF)[6][3];
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire, flat, smooth, cur, circle,voronoi,next;
int width, height;
float aveE;



void drawTri()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    glColor3f(0, 1.0, 0.0);
    
    for(int i=0; i < triN ; i++){
        int *t = tri[i];
        glBegin(GL_TRIANGLES);
        if(t[0] != 0 && t[1] != 0 && t[2] != 0){
            glVertex3fv(ver[t[0]]);
            glVertex3fv(ver[t[1]]);
            glVertex3fv(ver[t[2]]);
        }
        glEnd();
    }
}

void drawVoronoi()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 0, 0);
    glPointSize(7);
    glColor3f(1, 0.3, 0.0);
    for (int i = 0; i < segN; i++) {
        glBegin(GL_LINES);
        glVertex3f(seg[i][0],seg[i][1],0);
        glVertex3f(seg[i][2],seg[i][3],0);
        glEnd();
        glBegin(GL_POINTS);
        glVertex3f(seg[i][0],seg[i][1],0);
        glEnd();
    }
}

void drawNextTri()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex3fv(ver[tri[nextDraw][0]]);
    glVertex3fv(ver[tri[nextDraw][1]]);
    glVertex3fv(ver[tri[nextDraw][2]]);
    glEnd();
    cout << "triNum = " << nextDraw <<endl;
}
void drawNextVoronoi()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex3f(seg[segIndex[nextDraw][0]][0], seg[segIndex[nextDraw][0]][1], 0);
    glEnd();
}

void drawNextNei()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_LIGHTING);
    glLineWidth(3);
    glColor3f(1, 1, 1);
    /*
     glBegin(GL_TRIANGLES);
     
     glVertex3fv(ver[tri[neighbor[nextDraw][0]][0]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][0]][1]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][0]][2]]);
     
     
     glVertex3fv(ver[tri[neighbor[nextDraw][1]][0]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][1]][1]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][1]][2]]);
     
     glVertex3fv(ver[tri[neighbor[nextDraw][2]][0]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][2]][1]]);
     glVertex3fv(ver[tri[neighbor[nextDraw][2]][2]]);
     
     glEnd();
     */
    for (int k = 0; k < 2; k++) {
        if(k == 1){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(0.1, 0.2, 0.8);
        }
        for (int i = 0; i < triN; i++) {
            int *t = tri[i];
            if(t[0] != 0 && t[1] != 0 && t[2] != 0){
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][0][0], intersection[i][0][1], intersection[i][0][2]);
                glVertex3f(remainingF[i][0][0], remainingF[i][0][1], remainingF[i][0][2]);
                glVertex3f(remainingE[i][0][0], remainingE[i][0][1], remainingE[i][0][2]);
                glEnd();
                
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][0][0], intersection[i][0][1], intersection[i][0][2]);
                glVertex3f(remainingF[i][1][0], remainingF[i][1][1], remainingF[i][1][2]);
                glVertex3f(remainingE[i][2][0], remainingE[i][2][1], remainingE[i][2][2]);
                glEnd();
                
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][1][0], intersection[i][1][1], intersection[i][1][2]);
                glVertex3f(remainingF[i][2][0], remainingF[i][2][1], remainingF[i][2][2]);
                glVertex3f(remainingE[i][1][0], remainingE[i][1][1], remainingE[i][1][2]);
                glEnd();
                
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][1][0], intersection[i][1][1], intersection[i][1][2]);
                glVertex3f(remainingF[i][3][0], remainingF[i][3][1], remainingF[i][3][2]);
                glVertex3f(remainingE[i][0][0], remainingE[i][0][1], remainingE[i][0][2]);
                glEnd();
                
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][2][0], intersection[i][2][1], intersection[i][2][2]);
                glVertex3f(remainingF[i][4][0], remainingF[i][4][1], remainingF[i][4][2]);
                glVertex3f(remainingE[i][2][0], remainingE[i][2][1], remainingE[i][2][2]);
                glEnd();
                
                glBegin(GL_POLYGON);
                glVertex3f(voro_ver[i][0], voro_ver[i][1], voro_ver[i][2]);
                glVertex3f(intersection[i][2][0], intersection[i][2][1], intersection[i][2][2]);
                glVertex3f(remainingF[i][5][0], remainingF[i][5][1], remainingF[i][5][2]);
                glVertex3f(remainingE[i][1][0], remainingE[i][1][1], remainingE[i][1][2]);
                glEnd();
            }
        }
        
    }
    // cout << "neiTri = " << tempNeighbor[nextDraw][0] <<" "<< tempNeighbor[nextDraw][1] <<" " <<tempNeighbor[nextDraw][2] <<endl;
}

void drawVertex()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING); //ÉâÉCÉgÇ OFF
    glPointSize(5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for(int i=1; i<verN; i++){
        //glColor3f((float)i/verN, (float)i/verN, 1.0-(float)i/verN);
        glVertex3fv(ver[i]);
    }
    
    for (int i = 0; i < triN; i++) {
        int *t = tri[i];
        if(t[0] != 0 && t[1] != 0 && t[2] != 0){
            for (int j = 0; j < 3; j++) {
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(intersection[i][j][0],intersection[i][j][1],intersection[i][j][2]);
                glColor3f(1.0, 1.0, 0);
                glVertex3f(remainingE[i][j][0], remainingE[i][j][1], remainingE[i][j][2]);
                
            }
            glColor3f(1.0, 0.0, 1);
            for (int j = 0; j < 6; j++) {
                glVertex3f(remainingF[i][j][0], remainingF[i][j][1], remainingF[i][j][2]);
            }
        }
    }
    glEnd();
    
}

void drawCircle()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    float red;
    glColor3f(0.3, 0.1, 0.8);
    for(int i = 0; i<verN; i++ ){
        //
        //cout << rad[i] <<endl;
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
            float x1 = rad[i] * cos(th1_rad);
            float y1 = rad[i] * sin(th1_rad);
            float x2 = rad[i] * cos(th2_rad);
            float y2 = rad[i] * sin(th2_rad);
            glBegin(GL_TRIANGLES);
            glVertex3f( ver[i][0], ver[i][1], .0 );
            glVertex3f( x1+ver[i][0], y1+ver[i][1], .0 );
            glVertex3f( x2+ver[i][0], y2+ver[i][1], .0);
            glEnd();
        }
    }
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(shiftX, shiftY, 0);
    float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
    if(s != 0)
        glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
    
    if(wire == true){
        drawTri();
    }
    if(next == true){
        drawNextTri();
        drawNextVoronoi();
        drawNextNei();
    }
    if (circle) {
        drawCircle();
        drawVertex();
    }
    if(voronoi){
        drawVoronoi();
    }
    glFlush();
    glutSwapBuffers();
}

void allocateMem(){
    ver =  new float[verN][3];
    tri = new int[triN][3];
    rad = new float[verN];
    neighbor = new int[triN][3];
    segIndex = new int[triN][3];
    intersection = new float[triN][3][3];
    remainingE = new float[triN][3][3];
    voro_ver = new float[triN][3];
}

void deleteMesh(){
    delete[] ver;
    delete[] tri;
    delete[] rad;
    delete[] neighbor;
    delete[] seg;
    delete[] segIndex;
    delete[] intersection;
    delete[] remainingE;
    delete[] voro_ver;
}

void readMesh(char* file_name)
{
    /* Read the mesh file */
    FILE* in = fopen(file_name, "r");
    //ConstValueManager cvm;
    fscanf(in, "%d", &verN);
    fscanf(in, "%d", &triN);
    fscanf(in, "%d", &data_dimension);
    cout << verN << endl;
    cout << triN << endl;
    cout << data_dimension << endl;
    cout << "vertex" <<endl;
    allocateMem();
    
    for(int i=1; i<verN; i++){
        float* v = ver[i];
        fscanf(in, "%f %f %f", &v[0], &v[1], &rad[i]);
        v[2] = 0; /** ２次元ようにz成分をゼロに **/
        rad[i] = sqrtf(rad[i]);
        cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    cout << "triangle" <<endl;
    
    for(int i=0; i<triN; i++){
        //int* t = tri[i];
        //int* t = tempTri[i];
        fscanf(in, "%d %d %d", &tri[i][0], &tri[i][1], &tri[i][2]);
        //cout << t[0] << " " << t[1] << " " << t[2] << endl;
    }
    cout << "triangle neighbor" <<endl;
    for(int i=0; i<triN; i++){
        int* n = neighbor[i];
        
        fscanf(in, "%d %d %d", &n[0], &n[1], &n[2]);
        //cout << "fake neighbor\n\n\n"<< t[0] << " " << t[1] << " " << t[2] << endl;
    }
    
    cout << "segment" <<endl;
    
    fscanf(in, "%d", &segN);
    
    seg = new float[segN][4];
    
    remainingF = new float[triN][6][3];
    
    for (int i = 0; i < segN; i++) {
        float* s = seg[i];
        fscanf(in, "%f %f %f %f", &s[0], &s[1], &s[2], &s[3]);
        cout <<  s[0] << " " << s[1] << " " << s[2] <<" "<< s[3]<< endl;
    }
    cout << "triangle2" <<endl;
    fclose(in);
}


void myinit()
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

void myReshape(int w, int h)
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
                -10.0, 10.0);
    else
        glOrtho(-scale * (GLfloat) w / (GLfloat) h / zoom,
                scale * (GLfloat) w / (GLfloat) h /zoom,
                -scale/zoom, scale/zoom, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int btn, int state, int x, int y)
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
        display();
    }
    else if(btn==GLUT_RIGHT_BUTTON){
        zoom -= 0.0025f*(y - mouseY);
        if(zoom > 20.0f) zoom = 20.0f;
        else if(zoom < 0.05f) zoom = 0.05f;
        myReshape(width, height);
        display();
    }
    else if(btn==GLUT_MIDDLE_BUTTON){
        shiftX += 0.25f*(x - mouseX)/zoom;
        shiftY += 0.25f*(y - mouseY)/zoom;
        display();
    }
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'z':
            next = !next;
            display();
            if(next == true) {
                nextDraw++;
                nextDrawVoronoi++;
            }
            break;
        case 'w':
            wire = !wire;
            display();
            break;
        case 'r':
            circle = !circle;
            display();
            break;
        case 'v':
            voronoi = !voronoi;
            display();
            break;
        case 'f':
            
            flat = !flat;
            if(flat){
                smooth = false;
                cur = false;
            }
            display();
            break;
        case 's':
            smooth = !smooth;
            if(smooth){
                flat = false;
                cur = false;
            }
            display();
            break;
        case 'c':
            cur = !cur;
            if(cur){
                flat = false;
                smooth = false;
            }
            display();
            break;
    }
}

void init()
{
    qw = 1;
    qx = qy =  qz = 0;
    zoom = 1;
    shiftX = shiftY = 0;
    wire = true;
    next = false;
    voronoi = false;
    circle = false;
    flat = false;
    cur = false;
    smooth = false;
}