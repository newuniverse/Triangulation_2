#include "porousMetalAPI.h"
#include "Voxel.h"
#include "ConstValue.h"
#include "Rendering.h"
#include "Mesh.h"
Calculate_Voronoi_vertex cvv;
////
int main (int argc, char ** argv)
{
    ConstValue cv;
    cv.Set();
    Voxel vox;
    switch (cv.Phase) {
        case cv.VOXEL_PROCESSING:
        {
            Voxel vox;
            if(cv.GetDim() == 2) vox.ReadFile(cv.fileTobeRead);
            if(cv.GetDim() == 3) vox.ReadBinaryFile(cv.fileTobeRead);
            vox.Labeling(); //ラベリング
            if(cv.GetDim() == 2)vox.ReadFile(cv.fileTobeRead);
            vox.ComputeDistanceField();
            vox.WriteSphere();  //球近似ごの重心と半径の書き出し
            break;
        }
        case cv.VOXEL_DISTANCEFIELD:
        {
            Voxel vox;
            if(cv.GetDim() == 3) vox.ReadBinaryFile(cv.fileTobeRead); //ボクセル読み込み
            if(cv.GetDim() == 2) vox.ReadFile(cv.fileTobeRead);
            
            vox.Labeling(); //ラベリング
            
            if(cv.GetDim() == 3) vox.ReadBinaryFile(cv.fileTobeRead);
            if(cv.GetDim() == 2) vox.ReadFile(cv.fileTobeRead);
            
            vox.ComputeDistanceField(); //距離場計算
            vox.WriteSphere();  //球近似ごの重心と半径の書き出し
            system("./regular");//CGALにVoronoiとDelaunayを計算してもらう
            cout << "end regular" << endl;
            cv.MovePhase();
            break;
        }
        case cv.RENDERING:
        {
            Rendering::InitValues();
            Rendering::ReadCoordinate("triangulation.txt");//球、Delaunay、Voronoiの読み込み
            Rendering::Meshing();
            
            //cv.MovePhase();
            Rendering::OpenglRendering(argc, argv);
            break;
        }
        case cv.BOTH:
        {
            Voxel vox;
            ////vox.ReadFile(cv.fileTobeRead); //ボクセル読み込み
            vox.ReadBinaryFile(cv.fileTobeRead);
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
    
    return 0;
}
