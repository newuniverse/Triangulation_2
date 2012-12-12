#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_euclidean_traits_2.h>
#include <CGAL/Regular_triangulation_filtered_traits_2.h>
#include <CGAL/Regular_triangulation_2.h>
#include <iterator>
#include <fstream>

//typedef Triangulation_data_structure_2<> Tds;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;
typedef CGAL::Regular_triangulation_filtered_traits_2<K>  Traits;
typedef CGAL::Regular_triangulation_2<Traits> Regular_triangulation;

//typedef Tds::Face_iterator All_faces_iterator;
//typedef Tds::Face_handle Face_handle;
//typedef CGAL::Regular_triangulation_euclidean_traits_2<K, Weighted> Traits_euclidean;


struct Cropped_voronoi_from_delaunay{
    std::list<Segment_2> m_cropped_vd;
    Iso_rectangle_2 m_bbox;
    
    Cropped_voronoi_from_delaunay(const Iso_rectangle_2& bbox):m_bbox(bbox){}
    
    template <class RSL>
    void crop_and_extract_segment(const RSL& rsl){
        CGAL::Object obj = CGAL::intersection(rsl,m_bbox);
        const Segment_2* s=CGAL::object_cast<Segment_2>(&obj);
        if (s) m_cropped_vd.push_back(*s);
    }
    
    void operator<<(const Ray_2& ray)    { crop_and_extract_segment(ray); }
    void operator<<(const Line_2& line)  { crop_and_extract_segment(line); }
    void operator<<(const Segment_2& seg){ crop_and_extract_segment(seg); }
};

int main()
{
   Regular_triangulation rt;
   std::ifstream in("regular_input.txt");
   Regular_triangulation::Weighted_point wp;
   int count = 0;
    int count2 = 0;
   while(in >> wp){
     count++;
     rt.insert(wp);
   }
   rt.is_valid();
    //OstreamでTriangleの吐き出し
    std::ofstream fout_T;
    fout_T.open("triangulation.txt");
    fout_T << rt;
    fout_T << "\n";
    //Voronoi vertexの吐き出し
    //All_faces_iterator af_it = rt.All_faces_iterator.begin();
    Regular_triangulation::All_faces_iterator fit;
    for(fit = rt.all_faces_begin(); fit != rt.all_faces_end(); fit ++){
        fout_T << rt.dual(fit) << "\n";
        std::cout << rt.dual(fit) << std::endl;
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
    std::cout << rt.number_of_vertices() << std::endl;
    std::cout << "number of hidden vertices :  " ;
    std::cout << rt.number_of_hidden_vertices() << std::endl;
   return 0;
}
