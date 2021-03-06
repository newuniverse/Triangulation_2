#include "ConstValue.h"
#include "Voxel.h"
#include <sstream>
#include <string>
Voxel::Voxel()
{
    //voxelのメモリ確保
    ConstValue cv;
    int x,y,z;
    x = cv.GetX();
    y = cv.GetY();
    z = cv.GetZ();
    voxel = new float**[x];
    for (int i = 0;  i < x; i++)
    {
		voxel[i] = new float*[y];
    }
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++) {
            voxel[i][j] = new float[z];
        }
    }
}

int Voxel::ReadBinaryFile(string filename)
{
    ConstValue cv;
    int x,y,z;
    x = cv.GetX(); y = cv.GetY(); z = cv.GetZ();
    if(filename == "") filename = "input.raw";
    cout << filename.c_str() << endl;
    FILE* in = fopen(filename.c_str(),"rb");
	if(in == NULL){
		printf("%sファイルが開けません¥n",filename.c_str());
		return -1;
	}
  
    int count = 0;
    int zcount = 0;
    
    //指定回数により，データを抽出
	for(int i=0;i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                short temp;
                fread(&temp, sizeof(short), 1, in) ;
                voxel[i][j][k] = (float)(temp*1.0);
                if( voxel[i][j][k] > 0 ) count++;
                else zcount++;
                // cout << "voxel = "<< temp <<endl;
            }
        }
    }
    cerr<<zcount<<"voxels are zero."<<std::endl;
    cerr<<count<<"voxels are non-zero."<<std::endl;
    cout << "read!"<<endl;
	fclose(in);
    return 0;
}

void Voxel::ReadFile(string filename)
{
    ConstValue cv;
    int x,y,z;
    x = cv.GetX(); y = cv.GetY(); z = cv.GetZ();
    float num;
    for (int k = 0; k < z; k++)
    {
        //filename;
        ostringstream s ;
        s << k ;
        //if(filename.size() < 5) filename = "input" + s.str() + ".txt";
        ifstream fin;
        fin.open(filename.c_str());
        for (int j = 0; j < y; j++)
        {
            for (int i = 0; i < x; i++)
            {
                fin >> num;
                //cout << "num = "<< num << endl;
                voxel[i][j][k] = /*(int)*/(float)num;
            }
        }
        fin.close();
        fin.clear();
    }
}

void Voxel::WriteTextFile(float ***table, string filename){
    ConstValue cv;
    int x,y,z;
    x = cv.GetX(); y = cv.GetY(); z = cv.GetZ();
    for (int k = 0; k < z; k++){
        ofstream fout;
        ostringstream s ;
        s << k ;
        filename = filename + s.str() + ".txt";
        fout.open(filename.c_str());
        for (int j = 0; j < y; j++)
        {
            for (int i = 0; i < x; i++)
            {
                //cout << voxel[i][j][k];
                //cout << " ";
                fout << table[i][j][k];
                fout << " ";
            }
            //cout << "\n";
            fout << "\n";
        }
        fout.close();
        fout.clear();
    }
}

void Voxel::WriteBinaryFile(){
    ConstValue cv;
    int x,y,z;
    x = cv.GetX(); y = cv.GetY(); z = cv.GetZ();
    /*
    for (int k = 0; k < z; k++){
        ofstream fout;
        string filename;
        ostringstream s ;
        s << k ;
        filename = "output" + s.str() + ".txt";
        fout.open(filename.c_str());
        for (int j = 0; j < y; j++)
        {
            for (int i = 0; i < x; i++)
            {
                cout << voxel[i][j][k];
                cout << " ";
                //fout << voxel[i][j][k];
                //fout << " ";
            }
            cout << "\n";
            //fout << "\n";
        }
       // fout.close();
       // fout.clear();
    }*/
    FILE* out = fopen("labeled.raw", "w+b");
    for(int i=0;i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                unsigned short temp = (int) voxel[i][j][k];
                //cout << voxel[i][j][k] <<endl;
                fwrite(&temp, sizeof(unsigned short), 1, out);
            }
        }
    }
    fclose(out);
}
//voxel以外でfloat型書き出し用
void Voxel::WriteBinaryFile(float ***table, string filename){
    ConstValue cv;
    int x,y,z;
    x = cv.GetX(); y = cv.GetY(); z = cv.GetZ();
    filename = filename + ".raw";
    FILE* out = fopen(filename.c_str(), "w+b");
    for(int i=0;i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                float temp = voxel[i][j][k];
                fwrite(&temp, sizeof(float), 1, out);
            }
        }
    }
    fclose(out);
}

void Voxel::WriteCsvData(string filename)
{
    ofstream fout;
    fout.open(filename.c_str()); 
    if(!fout.is_open())
    {
        cout << "ファイルをオープンできません" << endl;
    }
    else
    {
        for (int i = 0; i < (int)csvContainer.size(); i++)
            fout << csvContainer2[ i ] << ","<< csvContainer[ i ] <<"\n";
        fout.close();
        fout.clear();
    }
    
    ofstream fout2;
    fout2.open("volumes.csv");
    if(!fout2.is_open())
    {
        cout << "ファイルをオープンできません" << endl;
    }
    else
    {
        fout2 << labelIndex - 3 << "\n";
        for (int i = 0; i < (int)volume.size(); i++)
            fout2 << volume[ i ] <<"\n";
        fout2.close();
        fout2.clear();
    }
    
}

void Voxel::WriteSphere()
{
    ofstream fout;
    fout.open("regular_input.txt"); //CGALに渡すデータ
    ConstValue cv;
    if(!fout.is_open())
    {
        cout << "ファイルをオープンできません" << endl;
    }
    else
    {
        //borderをつける
        if( cv.GetDim() == 2 )
        {
            int division = ( int )sqrt( radius.size());
            //division = 3;
            float x, y ,z, r;
            for( int i = 0; i < division; i++ )
            {
                x = 0.0 + 0.01 ; y = 1.0*(i + 1) * (cv.GetY() / division ) + 0.01 ; r = 1.0+0.01;
                fout << x << " " << y  << " " << r << "\n";
                x = 1.0*cv.GetX() + 0.01 ; y = 1.0*(i + 1) * (cv.GetY() / division ) + 0.01; r = 1.0+0.01;
                fout << x << " " << y  << " " << r << "\n";
                x = 1.0 * (i + 1) * ( cv.GetX() / division )+0.01; y = 0.0+0.01; r = 1.0+0.01;
                fout << x << " " << y  << " " << r << "\n";
                x = 1.0 * (i + 1) * ( cv.GetX() / division ) + 0.01; y = 1.0*cv.GetY() + 0.01; r = 1.0+0.01;
                fout << x << " " << y  << " " << r << "\n";
            }
        }
        
        //重心点
        for (int i = 1; i < radius.size(); i++)//index = 0は周りの非材料部分, 1からがセル
        {
            if(radius[ i ] < 0) break;
            if((int)radius[ i ] != 0 && (int)x_center[i] != 0 )
            {
                if( cv.GetDim() == 2 )
                {
                    fout << x_center[i] << " " << y_center[i]<< " " << radius[i]*radius[i]<< "\n";
                }
                
                if(cv.GetDim() == 3)
                {
                    if( (float)cv.GetX() - radius[ i ] > 0 ){
                        fout << x_center[i] << " " << y_center[i]<< " " << z_center[i] << " " << radius[i]*radius[i]<< "\n";
                    }
                }
            }
        }
    }
    fout.close();
    fout.clear();
}