#pragma once

#include "SolidSlice.h"
#include "Point3D.h"

#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/vector_angle.hpp"

#define GLM_FORCE_RADIANS
#define DEG_TO_RAD(x) (x*0.0174532925199f)
#define GLM_ENABLE_EXPERIMENTAL

class SvgExporter
{
public:

	SvgExporter();
    ~SvgExporter();

    void export_svg_3d(std::vector<SolidSlice> slices);

private:

    void fill(char filename[], const std::vector<std::vector<Point3D>> &polygons, std::vector<bool> orientation);
    void add_svg_information(FILE* file);
};