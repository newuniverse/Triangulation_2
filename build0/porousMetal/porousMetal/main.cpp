#include "porousMetalAPI.h"
#include "Voxel.h"
#include "ConstValue.h"
#include "Rendering.h"
#include "Mesh.h"
#include <time.h>
Calculate_Voronoi_vertex cvv;
////
int main (int argc, char ** argv)
{
    clock_t start,end;
    start = clock();
    ConstValue cv;
    cv.Set();
    Voxel vox;
    switch (cv.Phase) {
        case cv.VOXEL_PROCESSING:
        {
            Voxel vox;
            if(cv.GetDim() == 2) vox.ReadFile(cv.GetFileName());
            if(cv.GetDim() == 3) vox.ReadBinaryFile(cv.GetFileName());
            vox.Labeling(); //ラベリング
            if(cv.GetDim() == 2)vox.ReadFile(cv.GetFileName());
            vox.ComputeDistanceField();
            vox.WriteSphere();  //球近似ごの重心と半径の書き出し
            break;
        }
        case cv.VOXEL_DISTANCEFIELD:
        {
            Voxel vox;
            if(cv.GetDim() == 3) vox.ReadBinaryFile(cv.GetFileName()); //ボクセル読み込み
            if(cv.GetDim() == 2) vox.ReadFile(cv.GetFileName());
            
            vox.Labeling(); //ラベリング
            end = clock();
            cout << "ラベリング計算時間=" << (end - start)/CLOCKS_PER_SEC<<"sec"<<endl;
            vox.ComputeDistanceField(); //距離場計算
            vox.WriteSphere();  //球近似ごの重心と半径の書き出し
            
            if(cv.GetDim() == 2)system("./regular");//CGALにVoronoiとDelaunayを計算してもらう
            if(cv.GetDim() == 3)system("./regular_3");
            cout << "end regular" << endl;
            cv.MovePhase();
            break;
        }
        case cv.RENDERING:
        {
            Rendering::InitValues();
            if(cv.GetDim() == 2){
                Rendering::ReadCoordinate("triangulation.txt");//球、Delaunay、Voronoiの読み込み
                Rendering::Meshing();
            }
            if(cv.GetDim() == 3) {
                Rendering::ReadCoordinate3D("triangulation3D.txt");
                //Rendering::Meshing3D();
            }
            //cv.MovePhase();
            Rendering::OpenglRendering(argc, argv);
            break;
        }
        case cv.BOTH:
        {
            Voxel vox;
            ////vox.ReadFile(cv.GetFileName()); //ボクセル読み込み
            vox.ReadBinaryFile(cv.GetFileName());
            vox.Labeling(); //ラベリング
            vox.WriteBinaryFile();    //ラベリング処理後のボクセル書き出し
            vox.WriteSphere();  //球近似ごの重心と半径の書き出し
            Rendering::InitValues();
            Rendering::ReadCoordinate("triangulation.txt");
            Rendering::OpenglRendering(argc, argv);
            break;
        }
        default:
            break;
    }
    end = clock();
    cout << "総計算時間=" << (end - start)/CLOCKS_PER_SEC<<"sec."<<endl;
    return 0;
}
