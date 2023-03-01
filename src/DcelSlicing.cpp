#include <iostream>
#include "Mesh_DCEL.h"
#include "MeshParserStrategy.h"
#include "IncrementalSlicer.h"
#include "SolidSlice.h"
#include "MeshBuilder.h"
#include "Translate3D.h"
#include <stdlib.h>
#include <string.h>
#include <string.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <CGAL/Cartesian.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Quotient.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Sweep_line_2_algorithms.h>

#define GLM_FORCE_RADIANS
#define DEG_TO_RAD(x) (x*0.0174532925199f)

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

void add_svg_information (FILE *file) {
  fprintf (file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf (file, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
  fprintf (file, "<svg preserveAspectRatio=\"xMidYMid meet\" width=\"1024\" height=\"768\" viewBox=\"0 0 1024 768\""); 
  fprintf (file, " xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"");
  fprintf (file, " xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""); 
  fprintf (file, " xmlns:cc=\"http://web.resource.org/cc/\">\n");
  fprintf (file, " <defs>\n");
  fprintf (file, "  <marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refx=\"0\" refy=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\" viewBox=\"0 0 20 20\">\n");
  fprintf (file, "    <path d=\"M0,0 L0,6 L9,3 z\" fill=\"#f00\" />\n");
  fprintf (file, "  </marker>\n");
  fprintf (file, "</defs>\n");
}

void export_svg (char filename[], const std::vector<std::vector<Point3D*>> &Polygons, int nslices, int nsegments, bool video, std::vector<bool> orientation) {
  glm::vec3 fromEuler (0.0f, 0.0f, 60.0f);
  glm::quat quaternion (DEG_TO_RAD(fromEuler));
  glm::vec3 toEuler = glm::eulerAngles(quaternion);
  float angle = glm::angle(quaternion);
  glm::vec3 axis = glm::axis(quaternion);
  glm::mat4 View = glm::rotate(glm::mat4(1.0), angle, axis);

  //float zoom = 0.6f; /*01.liver*/
  //float zoom = 0.2f; /*02.femur*/
  float zoom = 0.6f;  /*04.demon*/
  glm::mat4 Projection = glm::perspective (zoom, 4.0f / 3.0f, 0.1f, 100.f);
  int shift_x = +400;
  int shift_y = +400;

  glm::mat4 Model = glm::lookAt (
        glm::vec3(1, 1, 1),    /* Eye point (where am I?) */
        glm::vec3(0, 0, 0),    /* Look at Center point (What am I looking at?) */
        glm::vec3(0, 1, 0));   /* UP vector of the camera (Where is my UP vector?) */
  glm::mat4 MVP = Projection * View * Model;
  FILE *file = fopen (filename, "w");
  add_svg_information (file);

  /*Writing previous slices:*/
  int i;
  for (i = 0; i < nslices; i++) 
  {
    for (size_t index = 0; index < Polygons[i].size(); index++) 
    {
      Point3D p0 = *Polygons[i].at(index);
      if (index < (Polygons[i].size() - 1)) 
      {
        Point3D p1 = *Polygons[i].at(index + 1);
        p0.transform(MVP);
        p1.transform(MVP);
        if (orientation.at(i)) 
        {
          fprintf (file, "   <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"rgb(255,0,0)\"/>\n",
                   p0.get_x() + shift_x, p0.get_y() + shift_y, p1.get_x() + shift_x,p1.get_y() + shift_y);
        }
        else 
        {
          fprintf (file, "   <line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"rgb(0,0,0)\"/>\n",
                   p0.get_x() + shift_x, p0.get_y() + shift_y, p1.get_x() + shift_x,p1.get_y() + shift_y);
        }
      }
    }
  }
  fprintf (file,"</svg>\n");
  fclose(file);
}

void export_svg_3d (std::vector<SolidSlice*> polygons, int nplanes) 
{
  std::vector<std::vector<Point3D*>> P;
  std::vector<bool> orientation;
  for (int k = 0; k < nplanes; k++) {
    const size_t ncontours = polygons[k]->getContourSize();
    for (int c = 0; c < ncontours; c++) {
      std::vector<Point3D*> Pc = polygons[k]->getContour(c)->getPointVector();
      orientation.push_back(polygons[k]->getContour(c)->getOrientation());
      P.push_back(Pc);
    }
  }

  char filename[256];
  sprintf(filename, "out_3d.svg");
  printf("\n\nwriting output file: %s\n", filename);
  export_svg (filename, P, P.size(), 0, false, orientation);
  printf("... done\n\n\n");
}

int main(int argc, char** argv)
{
    char* model = nullptr;

    if (strcmp(argv[1], "-model") == 0)
    {
        model = argv[2];
    }

    float thickness = 0.f;

    if(argv[3] != nullptr && argv[4] != nullptr)
    {
        if (strcmp(argv[3], "-thickness") == 0) 
        {
            thickness = atof(argv[4]);

            if(thickness <= 0.f)
            {
                 std::cout << "Error: specify a positive slicing spacing in mm (thickness)!!!"<< std::endl;
                 return -1;
            }
        } 
    }
    else
    {
        std::cout << "Error: specify a positive slicing spacing in mm (thickness)!!!"<< std::endl;
        return -1;
    }

    std::string path(model);
    std::string lastFileName;
    
    size_t pos = path.find_last_of("/");
    
    if (pos != std::string::npos)
      lastFileName.assign(path.begin() + pos + 1, path.end());
    else
      lastFileName = path;
    
    Mesh_DCEL mesh;

    auto start = std::chrono::high_resolution_clock::now();
    MeshBuilder builder(&mesh);
    builder.build(model);
    auto end= std::chrono::high_resolution_clock::now();
	  std::chrono::duration<double, std::milli> float_ms1 = end - start;

    Point3D p = mesh.getBottomLeftVertex();

    if (p != Point3D(0, 0, 0))
    {
        Translate3D move(-p);
        move.meshTransform(&mesh);
    }

    std::vector<SolidSlice*> slices;

    IncrementalSlicer slicer;

    start = std::chrono::high_resolution_clock::now();
    slicer.sliceMesh(mesh, thickness, slices);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> float_ms2 = end - start;
    std::cout << "Time Building " << lastFileName << ": " << float_ms1.count() / 1000.f << std::endl;
	  std::cout << "Time Slicing: " << float_ms2.count() / 1000.f << std::endl;

    std::cout << "Total time: " << float_ms1.count() / 1000.f + float_ms2.count() / 1000.f << "s" << std::endl;

    std::cout << "Slice number: " << slices.size() << std::endl;

    export_svg_3d(slices, slices.size());

    return 0;
}

