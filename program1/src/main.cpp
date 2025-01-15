/* Release code for program 1 CPE 471 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

#include "tiny_obj_loader.h"
#include "Image.h"
#include "Primatives.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int g_width, g_height;
float width_scalar, width_offset, height_scalar, height_offset; 


void mesh2vertices(const std::vector<float>& positions, 
                    std::vector<Vertex>& vertices);
void mesh2triangles(const int n_triangles, const std::vector<unsigned int>& indices,
                    const std::vector<Vertex>& vertices, std::vector<Face>& faces);

int world2px(float x);
int world2py(float y);


/*
   Helper function you will want all quarter
   Given a vector of shapes which has already been read from an obj file
   resize all vertices to the range [-1, 1]
 */
void resize_obj(std::vector<tinyobj::shape_t> &shapes){
   float minX, minY, minZ;
   float maxX, maxY, maxZ;
   float scaleX, scaleY, scaleZ;
   float shiftX, shiftY, shiftZ;
   float epsilon = 0.001;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         if(shapes[i].mesh.positions[3*v+0] < minX) minX = shapes[i].mesh.positions[3*v+0];
         if(shapes[i].mesh.positions[3*v+0] > maxX) maxX = shapes[i].mesh.positions[3*v+0];

         if(shapes[i].mesh.positions[3*v+1] < minY) minY = shapes[i].mesh.positions[3*v+1];
         if(shapes[i].mesh.positions[3*v+1] > maxY) maxY = shapes[i].mesh.positions[3*v+1];

         if(shapes[i].mesh.positions[3*v+2] < minZ) minZ = shapes[i].mesh.positions[3*v+2];
         if(shapes[i].mesh.positions[3*v+2] > maxZ) maxZ = shapes[i].mesh.positions[3*v+2];
      }
   }

    //From min and max compute necessary scale and shift for each dimension
   float maxExtent, xExtent, yExtent, zExtent;
   xExtent = maxX-minX;
   yExtent = maxY-minY;
   zExtent = maxZ-minZ;
   if (xExtent >= yExtent && xExtent >= zExtent) {
      maxExtent = xExtent;
   }
   if (yExtent >= xExtent && yExtent >= zExtent) {
      maxExtent = yExtent;
   }
   if (zExtent >= xExtent && zExtent >= yExtent) {
      maxExtent = zExtent;
   }
   scaleX = 2.0 /maxExtent;
   shiftX = minX + (xExtent/ 2.0);
   scaleY = 2.0 / maxExtent;
   shiftY = minY + (yExtent / 2.0);
   scaleZ = 2.0/ maxExtent;
   shiftZ = minZ + (zExtent)/2.0;

   //Go through all verticies shift and scale them
   for (size_t i = 0; i < shapes.size(); i++) {
      for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
         shapes[i].mesh.positions[3*v+0] = (shapes[i].mesh.positions[3*v+0] - shiftX) * scaleX;
         assert(shapes[i].mesh.positions[3*v+0] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+0] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+1] = (shapes[i].mesh.positions[3*v+1] - shiftY) * scaleY;
         assert(shapes[i].mesh.positions[3*v+1] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+1] <= 1.0 + epsilon);
         shapes[i].mesh.positions[3*v+2] = (shapes[i].mesh.positions[3*v+2] - shiftZ) * scaleZ;
         assert(shapes[i].mesh.positions[3*v+2] >= -1.0 - epsilon);
         assert(shapes[i].mesh.positions[3*v+2] <= 1.0 + epsilon);
      }
   }
}


int main(int argc, char **argv)
{
    if(argc < 3) {
      cout << "Usage: raster meshfile imagefile" << endl;
      return 0;
    }
    // OBJ filename
    string meshName(argv[1]);
    string imgName(argv[2]);

    // TODO: take in screen size 
    //set g_width and g_height appropriately!
    g_width = 600;
    g_height = 300;

    //create an image
    // TODO: what is a shared pointer
    auto image = make_shared<Image>(g_width, g_height);

    // triangle buffer
    vector<unsigned int> triBuf;
    // position buffer
    vector<float> posBuf;
    // Some obj files contain material information.
    // We'll ignore them for this assignment.
    vector<tinyobj::shape_t> shapes; // geometry
    vector<tinyobj::material_t> objMaterials; // material
    string errStr;
    
   bool rc = tinyobj::LoadObj(shapes, objMaterials, errStr, meshName.c_str());
    /* error checking on read */
    if(!rc) {
        cerr << errStr << endl;
    } else {
        //keep this code to resize your object to be within -1 -> 1
        resize_obj(shapes); 
        posBuf = shapes[0].mesh.positions;
        triBuf = shapes[0].mesh.indices;
    }
    cout << "Number of vertices: " << posBuf.size()/3 << endl;
    cout << "Number of triangles: " << triBuf.size()/3 << endl;
    
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    mesh2vertices(posBuf, vertices);
    mesh2triangles(triBuf.size() / 3, triBuf, vertices, faces);

    // ! make sure i get the correct number of prims out
    assert(vertices.size() == posBuf.size() / 3);
    assert(faces.size() == triBuf.size() / 3);

    // convert from world space to pixel space
    // calc view volume
    // TODO: make a class
    float left, right, top, bottom; // TODO: far and near
    if (g_width >= g_height) {
        right = static_cast<float>(g_width) / g_height;
        left = static_cast<float>(-g_width) / g_height;
        top = 1;
        bottom = -1;
    } else {
        top = static_cast<float>(g_height) / g_width;
        bottom = static_cast<float>(-g_height) / g_width;
        left = -1;
        right = 1;
    }

    // printf("vvolume(%f, %f, %f, %f)\n", left, right, bottom, top);

    // calc pixel position
    // float width_scalar, width_offset, height_scalar, height_offset; 
    width_scalar = (g_width - 1) / (right - left);
    width_offset = -left * width_scalar;

    height_scalar = (g_height - 1) / (top - bottom);
    height_offset = -bottom * height_scalar;

    

    for (auto f : faces) {

    }

    /**
     * Knowns: triangles already in world space
     * need to convert to pixel space
     * 
     * this should print every triangle (will be overlapping)
     * for each triangle
     *      for each pixel in triangle
     *          compute bounding box
     *          if pixel in triangle 
     *              print pixel
     */

    




    //write out the image
   image->writeToFile(imgName);

    return 0;
}


void mesh2vertices(const std::vector<float>& positions, 
                    std::vector<Vertex>& vertices) {

    
    for (size_t i = 0; i < positions.size(); i += 3) {
        vertices.push_back(Vertex(positions[i], positions[i + 1], 
                            positions[i + 2], Color()));
    }

    return;
}

void mesh2triangles(const int n_triangles, const std::vector<unsigned int>& indices,
                    const std::vector<Vertex>& vertices, std::vector<Face>& faces) {

    int face_ind = 0;
    for (int i = 0; i < n_triangles; i++) {
        unsigned int i0, i1, i2;
        i0 = indices[face_ind];
        i1 = indices[face_ind + 1];
        i2 = indices[face_ind + 2];

        Face f = Face(vertices[i0], vertices[i1], vertices[i2]);
        faces.push_back(f);
        face_ind += 3;
    }

    return;
}

int w2px(float x) {
    return width_scalar * x + width_offset;
}

int w2py(float y) {
    return height_scalar * y + height_offset;
}
