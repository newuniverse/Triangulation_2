#define  CGAL_CFG_ARRAY_MEMBER_INITIALIZATION_BUG
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>
#include <iterator>
#include <fstream>
#include <vector>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Regular_triangulation_euclidean_traits_3<K>  Traits;

//typedef CGAL::RegularTriangulationTraits_3<K>  Traits;;

//typedef Traits::RT                                          Weight;

//typedef Traits::Bare_point                                  Point;

//typedef Traits::Weighted_point                              Weighted_point;

//typedef Traits::Weighted_point_3      Weighted_point;

typedef Traits::RT                                          Weight;
typedef Traits::Bare_point                                  Point;
typedef Traits::Weighted_point                              Weighted_point;

typedef CGAL::Regular_triangulation_3<Traits>               Rt;

typedef Rt::Vertex_iterator  Vertex_iterator;

typedef Rt::All_cells_iterator   All_cells_iterator;

typedef K::Point_3 Point_3;
typedef K::Segment_3 Segment_3;
typedef K::Ray_3 Ray_3;
typedef K::Line_3 Line_3;


int main()
{
    Rt reguTri;
    std::ifstream in("regular_input.txt");
    Weighted_point wp;
   
    std::vector<Weighted_point> P;
    int count = 0;
    
    
    while(in >> wp){
      count++;
      reguTri.insert(wp);
      cout << "weighted point" << count <<": "<< wp <<endl;
    }
    assert( reguTri.is_valid() );
    assert( reguTri.dimension() == 3 );
    //OstreamでTriangleの吐き出し
    std::ofstream fout_T;
    fout_T.open("triangulation3D.txt");
    fout_T << reguTri;
    //cout << reguTri;
    //fout_T << "\n";
    //Voronoi vertexの吐き出し
    //All_faces_iterator af_it = rt.All_faces_iterator.begin();
    All_cells_iterator cit;
    for(cit = reguTri.all_cells_begin(); cit != reguTri.all_cells_end(); cit ++){
        fout_T << reguTri.dual(cit) << "\n";
        std::cout << reguTri.dual(cit) << std::endl;
    }
   /*
    for(fit = rt.all_faces_begin(); fit != rt.all_faces_end(); fit ++){
        for (int i= 0; i<3; i++) {
            fout_T << rt.mirror_index(fit, i) << " ";
            std::cout << rt.mirror_index(fit, i) << " ";
        }
        fout_T << "\n";
        std::cout << "\n";
    }*/
       /*
    //////
   Iso_rectangle_2 bbox(0,0,498,475);
   Cropped_voronoi_from_delaunay vor(bbox);
   rt.draw_dual(vor);
   
    std::list<Segment_2>::iterator it = vor.m_cropped_vd.begin();
    while (it != vor.m_cropped_vd.end()) {
        count2++;
        ++it;
    }
    fout_T << count2 << "\n";
    copy(vor.m_cropped_vd.begin(),vor.m_cropped_vd.end(),std::ostream_iterator<Segment_2>(fout_T,"\n"));
    /////
     */
    fout_T.close();
    fout_T.clear();
    std::cout << "number of inserted points : " << count << std::endl;
    std::cout << "number of vertices :  " ;
    std::cout << reguTri.number_of_vertices() << std::endl;
    std::cout << "number of cells :  " ;
    std::cout << reguTri.number_of_cells() << std::endl;
    //std::cout << "number of hidden vertices :  " ;
    //std::cout << reguTri.number_of_hidden_vertices() << std::endl;
   return 0;
}
