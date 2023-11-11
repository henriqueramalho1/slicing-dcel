#include <iostream>
#include "Mesh_DCEL.h"
#include "IncrementalSlicer.h"
#include "SolidSlice.h"
#include "MeshBuilder.h"
#include "SvgExporter.h"
#include <stdlib.h>
#include <string.h>
#include <string.h>

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
    if(!builder.build(model))
      return -1;
    auto end= std::chrono::high_resolution_clock::now();
	  std::chrono::duration<double, std::milli> float_ms1 = end - start;

    std::cout << "Faces: " << mesh.get_faces().size() << std::endl;

    std::vector<SolidSlice> slices;

    IncrementalSlicer slicer;

    start = std::chrono::high_resolution_clock::now();
    slicer.slice_mesh(mesh, thickness, slices);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> float_ms2 = end - start;

    std::cout << "Time Building " << lastFileName << ": " << float_ms1.count() / 1000.f << std::endl;
	  std::cout << "Time Slicing: " << float_ms2.count() / 1000.f << std::endl;

    std::cout << "Total time: " << float_ms1.count() / 1000.f + float_ms2.count() / 1000.f << "s" << std::endl;

    std::cout << "Slice number: " << slices.size() << std::endl;
    std::cout << "Intersections: " << slicer.intersections << std::endl;

    SvgExporter exporter;
    exporter.export_svg_3d(slices);

    return 0;
}

