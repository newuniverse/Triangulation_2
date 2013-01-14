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
	//指定回数により，データを抽出
	for(int i=0;i < x; i++){
        for (int j = 0; j < y; j++) {
            for (int k = 0; k < z; k++) {
                int temp;
                fread(&temp, sizeof(short), 1, in);
                voxel[i][j][k] = (float)(temp*1.0);
                //cout << "voxel = "<< voxel[i][j][k] <<endl;
            }
        }
    }
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
                float temp = voxel[i][j][k];
                fwrite(&temp, sizeof(float), 1, out);
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
    fout.open(filename.c_str()); //CGALに渡すデータ
    if(!fout.is_open())
    {
        cout << "ファイルをオープンできません" << endl;
    }
    else
    {
        for (int i = 0; i < (int)csvContainer.size(); i++)
            fout << csvContainer[ i ] << "\n";
        fout.close();
        fout.clear();
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
        //重心点
        for (int i = 1; i < radius.size(); i++)//index = 0は周りの非材料部分, 1からがセル
        {
            if(radius[ i ] < 0) break;
            if(radius[ i ] != 0 && x_center[i] != 0 )
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
        //borderをつける
        /*
        if( cv.GetDim() == 2 )
        {
            int division = ( int )sqrt( radius.size());
            division = 3;
            for( int i = 0; i < division; i++ )
            {
                fout << 0 << " " << i * (cv.GetY() / division )  << " " << 1 << "\n";
                fout << cv.GetX() << " " << i * (cv.GetY() / division) << " " << 1 << "\n";
                fout << i * ( cv.GetX() / division ) << " " << 0 << " " << 1 << "\n";
                fout << i * ( cv.GetX() / division ) << " " << cv.GetY() << " " << 1 << "\n";
            }
        }*/
    }
    fout.close();
    fout.clear();
}