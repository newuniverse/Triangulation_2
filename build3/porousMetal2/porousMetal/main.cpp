//
//  main.cpp
//  porousMetal
//
//  Created by 臻禹 鈕 on 12/09/12.
//  Copyright 2012年 東京大学. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "porousMetalAPI.h"
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

string filename = "input.txt";


Pixel p;
Pixel visitTable;
Labeling label;

void Init();

float reso = 0.012;

int verN;
float (*ver)[3];

//éOäpå`êîÇ∆éOäpå`ÇÃí∏ì_î‘çÜÇÃîzóÒ (ÉÅÉÇÉäÇÕÉtÉ@ÉCÉãÇì«ÇÒÇæéûÇ…ämï€çœÇ›)
int triN;
int (*tri)[3];

float (*rad);
int dimension;

//éOäpå`ÇÃñ@ê¸ (ÉTÉCÉYÇÕ triN Å~ 3)
//â€ëË2-2Ç≈åvéZÇ∑ÇÈ (ÉÅÉÇÉäÇÕÉtÉ@ÉCÉãÇì«ÇÒÇæéûÇ…ämï€çœÇ›)
float (*norT)[3];

//í∏ì_ÇÃñ@ê¸ (ÉTÉCÉYÇÕ verN Å~ 3)
//â€ëË2-3Ç≈åvéZÇ∑ÇÈ (ÉÅÉÇÉäÇÕÉtÉ@ÉCÉãÇì«ÇÒÇæéûÇ…ämï€çœÇ›)
float (*norV)[3];

//ÉâÉvÉâÉVÉAÉì (ÉTÉCÉYÇÕ verN Å~ 3)
//â€ëË3 Ç≈åvéZÇ∑ÇÈ (ÉÅÉÇÉäÇÕÉtÉ@ÉCÉãÇì«ÇÒÇæéûÇ…ämï€çœÇ›)
float (*lap)[3];
//ó◊ê⁄í∏ì_ÇêîÇ¶ÇÈÇΩÇﬂÇÃîzóÒ
int *neiN;


//â€ëË2-2 : éOäpå`ÇÃñ@ê¸ÇåvéZÇ∑ÇÈä÷êî
void computeTriangleNormals(){
    for(int i=0; i<triN; i++){
        //îzóÒÇÃìYÇ¶éöÇ™í∑Ç≠Ç»ÇÈÇÃÇ≈ïœêîÇ…ë„ì¸ÇµÇƒÇ®Ç≠
        int* t = tri[i];
        float* A = ver[ t[0] ]; // A[0] ÇÕ xç¿ïWÅAA[1] ÇÕ yç¿ïWÅAA[2] ÇÕ zç¿ïWÇ…Ç»ÇÈ
        float* B = ver[ t[1] ];
        float* C = ver[ t[2] ];
        
        //â€ëËÇÕà»â∫ÇÇ§ÇﬂÇÈ
        //ÉxÉNÉgÉã AB Ç∆ ÉxÉNÉgÉã AC ÇÃäOêœ
        float cx = (B[1]-A[1])*(C[2]-A[2]) - (B[2]-A[2])*(C[1]-A[1]);
        float cy = (B[2]-A[2])*(C[0]-A[0]) - (B[0]-A[0])*(C[2]-A[2]);
        float cz = (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]);
        
        //í∑Ç≥ÇåvéZÇµÇƒê≥ãKâªÇ∑ÇÈ
        float l = sqrt(cx*cx + cy*cy + cz*cz);
        if(l != 0){ //äÑÇËéZÇµÇƒñ≥å¿ëÂÇ…Ç»ÇÁÇ»Ç¢ÇÊÇ§Ç…Ç∑ÇÈ
            norT[i][0] = cx/l;
            norT[i][1] = cy/l;
            norT[i][2] = cz/l;
        }
    }
}

//â€ëË2-4 : í∏ì_ñ@ê¸ÇåvéZÇ∑ÇÈä÷êî
void computeVertexNormals(){
    //Ç∑Ç◊ÇƒÇÃí∏ì_ñ@ê¸ÇÉ[ÉçÉxÉNÉgÉãÇ…Ç∑ÇÈ
    for(int i=0; i<verN; i++){
        norV[i][0] = 0;
        norV[i][1] = 0;
        norV[i][2] = 0;
    }
    
    //äeéOäpå`Ç…Ç®Ç¢Çƒ
    for(int i=0; i<triN; i++){
        int* t = tri[i];
        float* A = ver[t[0]];
        float* B = ver[t[1]];
        float* C = ver[t[2]];
        
        //éOäpå`ÇÃ2ï”ÇÃäOêœÇåvéZÇµÇƒ
        float cx = (B[1]-A[1])*(C[2]-A[2]) - (B[2]-A[2])*(C[1]-A[1]);
        float cy = (B[2]-A[2])*(C[0]-A[0]) - (B[0]-A[0])*(C[2]-A[2]);
        float cz = (B[0]-A[0])*(C[1]-A[1]) - (B[1]-A[1])*(C[0]-A[0]);
        
        //ìæÇÁÇÍÇΩäOêœÉxÉNÉgÉãÇÇªÇÃéOäpå`ÇÇ»Ç∑3í∏ì_ÇÃñ@ê¸Ç÷ë´ÇµÇ±Çﬁ
        for(int j=0; j<3; j++){
            norV[t[j]][0] += cx;
            norV[t[j]][1] += cy;
            norV[t[j]][2] += cz;
        }
    }
    
    //Ç∑Ç◊ÇƒÇÃí∏ì_ñ@ê¸Çê≥ãKâªÇ∑ÇÈ
    for(int i=0; i<verN; i++){
        float* n = norV[i];
        float l = (float)sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
        if(l != 0){
            n[0] /= l;
            n[1] /= l;
            n[2] /= l;
        }
    }
}

//â€ëË 3-1 : ÉâÉvÉâÉVÉAÉìÇÃåvéZ
void computeLaplacians(){
    //ëSÇƒÇÃí∏ì_Ç…Ç®Ç¢Çƒ
    //ÉâÉvÉâÉVÉAÉì lap[i][0], lap[i][1], lap[i][2] ÇÉ[ÉçÇ…Ç∑ÇÈ
    //ó◊ê⁄í∏ì_êî neiN[i] ÇÉ[ÉçÇ…Ç∑ÇÈ
    for(int i=0; i<verN; i++){
        lap[i][0] = 0;
        lap[i][1] = 0;
        lap[i][2] = 0;
        neiN[i] = 0;
    }
    
    //äeéOäpå`Ç…Ç®Ç¢Çƒ
    for(int i=0; i<triN; i++){
        int* t = tri[i];
        float* A = ver[t[0]];
        float* B = ver[t[1]];
        float* C = ver[t[2]];
        
        //ÉxÉNÉgÉã AB Ç A ÇÃÉâÉvÉâÉVÉAÉìÇ…ë´Çµ
        lap[t[0]][0] += B[0] - A[0];
        lap[t[0]][1] += B[1] - A[1];
        lap[t[0]][2] += B[2] - A[2];
        //A ÇÃó◊ê⁄í∏ì_êîÇ 1 ëùÇ‚Ç∑
        neiN[t[0]] = neiN[t[0]] + 1;
        
        //ÉxÉNÉgÉã BC Ç B ÇÃÉâÉvÉâÉVÉAÉìÇ…ë´Çµ
        lap[t[1]][0] += C[0] - B[0];
        lap[t[1]][1] += C[1] - B[1];
        lap[t[1]][2] += C[2] - B[2];
        //B ÇÃó◊ê⁄í∏ì_êîÇ 1 ëùÇ‚Ç∑
        neiN[t[1]] = neiN[t[1]] + 1;
        
        //ÉxÉNÉgÉã CA Ç C ÇÃÉâÉvÉâÉVÉAÉìÇ…ë´Çµ
        lap[t[2]][0] += A[0] - C[0];
        lap[t[2]][1] += A[1] - C[1];
        lap[t[2]][2] += A[2] - C[2];
        //C ÇÃó◊ê⁄í∏ì_êîÇ 1 ëùÇ‚Ç∑
        neiN[t[2]] = neiN[t[2]] + 1;
    }
    
    //ëSÇƒÇÃí∏ì_Ç…Ç®Ç¢ÇƒÉâÉvÉâÉVÉAÉìÇó◊ê⁄í∏ì_êîÇ≈äÑÇÈ
    for(int i=0; i<verN; i++){
        lap[i][0] = lap[i][0] / neiN[i];
        lap[i][1] = lap[i][1] / neiN[i];
        lap[i][2] = lap[i][2] / neiN[i];
    }
}

//â€ëË 3-2 : ÉâÉvÉâÉVÉAÉìïΩääâª
void laplacianSmoothing(){
    //ÉâÉvÉâÉVÉAÉìÇåvéZÇ∑ÇÈ
    computeLaplacians();
    
    //â€ëË 3-3 Ç≈ÇÕÇ±ÇÃílÇïâÇ…ÇµÇƒÇ›ÇÈ
    float dt = 0.5; //0Å`1ÇÃílÇì¸ÇÍÇÈ
    
    //äeí∏ì_Ç÷ dt Å~ ÉâÉvÉâÉVÉAÉìÇë´Ç∑ (Ç±Ç±ÇñÑÇﬂÇÈÇÃÇ™â€ëË 3-2)
    for(int i=0; i<verN; i++){
        ver[i][0] = ver[i][0] + dt*lap[i][0];
        ver[i][1] = ver[i][1] + dt*lap[i][1];
        ver[i][2] = ver[i][2] + dt*lap[i][2];
    }
}


/************ Ç±ÇÃâ∫ÇÕâ€ëËÇ…ÇÕä÷åWÇÃÇ»Ç¢ä÷êî (Ç›Ç»Ç≠ÇƒÇÊÇ¢) *****************/

//ïœêîÇ¢ÇÎÇ¢ÇÎ
int mouseX, mouseY;
float qw, qx, qy, qz;
float zoom;
float shiftX, shiftY;
bool wire, flat, smooth, cur, circle;
int width, height;
float aveE; //ï”ÇÃïΩãœÇÃí∑Ç≥ (âöì ÇÃêFÇ√ÇØÇ…égÇ§)

//ÉèÉCÉÑÅ[Ç≈ï\é¶Ç∑ÇÈ
void drawWire(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING); //ÉâÉCÉgÇ OFF
    
    glColor3f(0.2, 0.2, 1.0);
    for(int i=0; i<triN; i++){
        int *t = tri[i];
        if(t[0] != 0 && t[1] != 0 && t[2] != 0){
            glBegin(GL_POLYGON);
            glVertex3fv(ver[t[0]]);
            glVertex3fv(ver[t[1]]);
            glVertex3fv(ver[t[2]]);
            glEnd();
        }
    }
}

void drawPoint(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING); //ÉâÉCÉgÇ OFF
                 
    
    glPointSize(5);
    for(int i=0; i<verN; i++){
        glColor3f((float)i/verN, (float)i/verN, 1.0-(float)i/verN);
        glBegin(GL_POINTS);
        glVertex3fv(ver[i]);
        glEnd();
    }
    
}
void drawCircle(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING); //ÉâÉCÉgÇ OFF
    
    //glColor3f(0.2, 0.7, 0.2);
    float * r = new float[verN];
    for(int i = 0; i<verN; i++ ){
        //cout << "radius = "<< rad[i] << endl;
        for (float th1 = 0.0;  th1 <= 360.0;  th1 = th1 + 1.0)
        {
           
            glColor3f((float)i/verN, 1 - (float)i/verN, (float)i/verN);
            float th2 = th1 + 15.0;
            float th1_rad = th1 / 180.0 * M_PI;
            float th2_rad = th2 / 180.0 * M_PI;
            float x1 = sqrtf(rad[i]) * cos(th1_rad);
            float y1 = sqrtf(rad[i]) * sin(th1_rad);
            float x2 = sqrtf(rad[i]) * cos(th2_rad);
            float y2 = sqrtf(rad[i]) * sin(th2_rad);
    
            glBegin(GL_TRIANGLES);
            glVertex3f( ver[i][0], ver[i][1], .0 );
            glVertex3f( x1+ver[i][0], y1+ver[i][1], .0 );
            glVertex3f( x2+ver[i][0], y2+ver[i][1], .0);
            glEnd();
            //cout << "x1="<<x1<<endl;
        }
    
    }
    
}

//ÉtÉâÉbÉgÉVÉFÅ[ÉfÉBÉìÉO
void flatShading(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    
    for(int i=0; i<triN; i++){
        int *t = tri[i];
        glBegin(GL_POLYGON);
        glNormal3fv(norT[i]);
        glVertex3fv(ver[t[0]]);
        glVertex3fv(ver[t[1]]);
        glVertex3fv(ver[t[2]]);
        glEnd();
    }
}

//ÉXÉÄÅ[ÉXÉVÉFÅ[ÉfÉBÉìÉO
void smoothShading(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    
    for(int i=0; i<triN; i++){
        int *t = tri[i];
        glBegin(GL_POLYGON);
        glNormal3fv(norV[t[0]]);
        glVertex3fv(ver[t[0]]);
        glNormal3fv(norV[t[1]]);
        glVertex3fv(ver[t[1]]);
        glNormal3fv(norV[t[2]]);
        glVertex3fv(ver[t[2]]);
        glEnd();
    }
}

void curvature(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING); //ÉâÉCÉgÇOFF
    
    for(int i=0; i<triN; i++){
        int *t = tri[i];
        glBegin(GL_POLYGON);
        for(int j=0; j<3; j++){
            float dot = lap[t[j]][0]*norV[t[j]][0] + lap[t[j]][1]*norV[t[j]][1] + lap[t[j]][2]*norV[t[j]][2];
            float c = -5.0*dot/aveE + 0.5; //êFÇ™ÇÊÇ≠ïtÇ≠ÇÊÇ§Ç…ílÇà¯Ç´êLÇŒÇ∑
            glColor3f(c, c, c);
            glVertex3fv(ver[t[j]]);
        }
        glEnd();
    }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glTranslatef(shiftX, shiftY, 0);
    float s = (float)sqrt(qx*qx + qy*qy + qz*qz);
    if(s != 0)
        glRotatef(2.0f*(float)acos(qw)*180/3.1412f, qx/s, qy/s,qz/s);
    
    //ÉÅÉbÉVÉÖÇÃÉåÉìÉ_ÉäÉìÉO
    if(wire){
        drawWire();
    }
    if (circle) {
        drawCircle();
        drawPoint();
    }
    if(flat)
        flatShading();
    else if(smooth)
        smoothShading();
    else if(cur)
        curvature();
    
    glFlush();
    glutSwapBuffers();
}

void allocateMem(){
    ver =  new float[verN][3];
    tri = new int[triN][3];
    norV = new float[verN][3];
    norT = new float[triN][3];
    lap = new float[verN][3];
    neiN = new int[verN];
    rad = new float[verN];
}

void deleteMesh(){
    delete[] ver;
    delete[] tri;
    delete[] rad;
    delete[] norV;
    delete[] norT;
    delete[] lap;
    delete[] neiN;
}

void readMesh(char* file_name){
    /* Read the mesh file */
    FILE* in = fopen(file_name, "r");
    
    fscanf(in, "%d", &verN);
    fscanf(in, "%d", &triN);
    fscanf(in, "%d", &dimension);
    cout << verN << endl;
    cout << triN << endl;
    cout << dimension << endl;
    
    allocateMem();
    //verN--;
    cout << "vertex" <<endl;
    for(int i=1; i<verN; i++){
        float* v = ver[i];
        fscanf(in, "%f %f %f", &v[0], &v[1], &rad[i]);
        //fscanf(in, "%f %f", &v[0], &v[1]);
        //rad[i] = 0.1;
        cout << v[0] << " " << v[1] << " " << rad[i] << endl;
        v[2] = 0; /** ２次元ようにz成分をゼロに **/
        rad[i] = rad[i]*reso;
    }
    cout << "triangle" <<endl;
    
    for(int i=0; i<triN; i++){
        int* t = tri[i];
        fscanf(in, "%d %d %d", &t[0], &t[1], &t[2]);
        cout << t[0] << " " << t[1] << " " << t[2] << endl;
    }
    fclose(in);
}

void normalizeSize(float length){
    // Compute the bounding box
    float max[3], min[3];
    float size[3], mid[3];
    for(int i=0; i<3; i++){
        max[i] = ver[0][i];
        min[i] = ver[0][i];
        for(int j=1; j<verN; j++){
            float v = ver[j][i];
            if(v > max[i])
                max[i] = v;
            else if(v < min[i])
                min[i] = v;
        }
        size[i] = max[i] - min[i];
        mid[i] = 0.5f*(max[i] + min[i]);
    }
    
    float s = size[0];
    if(size[1] > s)
        s = size[1];
    if(size[2] > s)
        s = size[2];
    float scale = length/s;
    
    for(int i=0; i<3; i++)
        for(int j=0; j<verN; j++)
            ver[j][i] = scale*(ver[j][i] - mid[i]);
    
   
    double total = 0;
    for(int i=0; i<triN; i++){
        int* t = tri[i];
        for(int j=0; j<3; j++){
            float* p = ver[t[j]];
            float* q = ver[t[(j+1)%3]];
            total += sqrt((q[0]-p[0])*(q[0]-p[0]) + (q[1]-p[1])*(q[1]-p[1]) + (q[2]-p[2])*(q[2]-p[2]));
        }
    }
    aveE = total/(3*triN);
}

void myinit(){
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
    
    glClearColor (1.0, 1.0, 1.0, 1.0);
}

void myReshape(int w, int h){
    width = w;
    height = h;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0/zoom, 2.0/zoom,
                -2.0 * (GLfloat) h / (GLfloat) w /zoom,
                2.0 * (GLfloat) h / (GLfloat) w /zoom,
                -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h / zoom,
                2.0 * (GLfloat) w / (GLfloat) h /zoom,
                -2.0/zoom, 2.0/zoom, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

//É}ÉEÉXëÄçÏ
void mouse(int btn, int state, int x, int y){
    if(state == GLUT_DOWN){
        mouseX = x;
        mouseY = y;
    }
    else if(btn==GLUT_LEFT_BUTTON){
        float mx = -0.0025f*(x - mouseX)/zoom;
        float my = 0.0025f*(y - mouseY)/zoom;
        
        //âÒì]
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
    else if(btn==GLUT_RIGHT_BUTTON){ //ägëÂèkè¨
        zoom -= 0.0025f*(y - mouseY);
        if(zoom > 20.0f) zoom = 20.0f;
        else if(zoom < 0.05f) zoom = 0.05f;
        myReshape(width, height);
        display();
    }
    else if(btn==GLUT_MIDDLE_BUTTON){ //ïΩçsà⁄ìÆ
        shiftX += 0.0025f*(x - mouseX)/zoom;
        shiftY += 0.0025f*(y - mouseY)/zoom;
        display();
    }
}


void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'w':
          
            wire = !wire;
            display();
            break;
        case 'r':
            circle = !circle;
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
        case 'l':
          
            laplacianSmoothing();
            if(flat)
                computeTriangleNormals();
            else if(smooth)
                computeVertexNormals();
            else if(cur){
                computeVertexNormals();
                computeLaplacians();
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

int main (int argc, char ** argv)
{
    FileStream fs;
    p.pxl = fs.readFile(filename);
    label.QueueFunc(p);
    fs.writeFile(label.gCenterI, label.gCenterJ, label.r, "regular.txt");
    fs.writeFile(p);
    delete label.gCenterI;
    delete label.gCenterJ;
    delete label.r;
        
    /**ここからOpenGL関係**/
    
    readMesh("data/Tri.txt");
    
    normalizeSize(4); 
    computeTriangleNormals(); 
    computeVertexNormals(); 
    computeLaplacians();
    
    qw = 1;
    qx = qy =  qz = 0;
    zoom = 1;
    shiftX = shiftY = 0;
    wire = true;
    circle = false;
    flat = false;
    cur = false;
    smooth = false;
    
    //ÉEÉBÉìÉhÉEÇÃèâä˙âª
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Triangulation");
    
    myinit(); 
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);  
    glutMouseFunc(mouse); 
    glutKeyboardFunc(keyboard);
    
    glutMainLoop();
    
    deleteMesh(); 
    
    return 0;
    
    return 0;
}


