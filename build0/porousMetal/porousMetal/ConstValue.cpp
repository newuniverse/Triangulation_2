#include "ConstValue.h"
int ConstValue::X_MAX = 500;
int ConstValue::Y_MAX = 500;
int ConstValue::Z_MAX = 500;
int ConstValue::DIMENSION;
float ConstValue::SD_threshold;
int ConstValue::convergence_radius;
string ConstValue::fileTobeRead;

void ConstValue::Set(){
    string filename;
    filename = "initialize.txt";
    ifstream fin;
    fin.open(filename.c_str());
    fin >> X_MAX;
    fin >> Y_MAX;
    fin >> Z_MAX;
    fin >> DIMENSION;
    fin >> SD_threshold;
    fin >> convergence_radius;
    fin >> Phase;
    fin >> fileTobeRead;
    cout << "filetoberead is "<<fileTobeRead<<endl;
    fin.close();
    fin.clear();
}

void ConstValue::MovePhase(){
    string filename;
    filename = "initialize.txt";
    ofstream fout;
    fout.open(filename.c_str());
    fout << X_MAX << "\n";
    fout << Y_MAX << "\n";
    fout << Z_MAX << "\n";
    fout << DIMENSION << "\n";
    fout << SD_threshold << "\n";
    fout << convergence_radius << "\n";
    if(Phase == 1)fout << 2 << "\n";
    if(Phase == 2)fout << 1 << "\n";
    fout << fileTobeRead;
    fout.close();
    fout.clear();
}

string ConstValue::GetFileName(){
    return fileTobeRead;
}

int ConstValue::GetX(){
    return X_MAX;
}
int ConstValue::GetY(){
    return Y_MAX;
}
int ConstValue::GetZ(){
    return Z_MAX;
}
int ConstValue::GetDim(){
    return DIMENSION;
}
float ConstValue::GetSD(){
    return SD_threshold;
}
int ConstValue::GetRad(){
    return convergence_radius;
}
