#include "SvgExporter.h"

SvgExporter::SvgExporter()
{
}

SvgExporter::~SvgExporter()
{
}

void SvgExporter::export_svg_3d(std::vector<SolidSlice> slices)
{
    std::vector<std::vector<Point3D>> P;
    std::vector<bool> orientation;
    int nslices = slices.size();
    for (int k = 0; k < nslices; k++) 
    {
        int ncontours = slices[k].contour_number();
        for (int c = 0; c < ncontours; c++) 
        {
            std::vector<Point3D> points = slices[k].get_contour(c).get_points();
            orientation.push_back(slices[k].get_contour(c).get_orientation());
            P.push_back(points);
        }
    }

    char filename[256];
    sprintf(filename, "out_3d.svg");
    printf("\n\nwriting output file: %s\n", filename);
    fill(filename, P, orientation);
    printf("... done\n\n\n");
}

void SvgExporter::fill(char filename[], const std::vector<std::vector<Point3D>> &polygons, std::vector<bool> orientation)
{
    glm::vec3 fromEuler (0.0f, 0.0f, 60.0f);
    glm::quat quaternion (DEG_TO_RAD(fromEuler));
    glm::eulerAngles(quaternion);
    float angle = glm::angle(quaternion);
    glm::vec3 axis = glm::axis(quaternion);
    glm::mat4 View = glm::rotate(glm::mat4(1.0), angle, axis);

    float zoom = 0.6f; /*01.liver*/
    //float zoom = 0.2f; /*02.femur*/
    //float zoom = 0.1f;  /*04.demon*/
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
    int npolygons = polygons.size();
    for (int i = 0; i < npolygons; i++) 
    {
        for (size_t index = 0; index < polygons[i].size(); index++) 
        {
            Point3D p0 = polygons[i].at(index);
            if (index < (polygons[i].size() - 1)) 
            {
                Point3D p1 = polygons[i].at(index + 1);
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
            else
            {
                Point3D p1 = polygons[i].at(0);
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

void SvgExporter::add_svg_information(FILE * file)
{
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
