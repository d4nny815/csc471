/* Release code for program 1 CPE 471 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <tgmath.h>

#include "tiny_obj_loader.h"
#include "Image.h"
#include "Primatives.h"
#include "WorldSpace.h"
#include "PixelSpace.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

int g_width, g_height;

void mesh2vertices(const std::vector<float>& positions, 
                    std::vector<Vertex>& vertices);
void mesh2triangles(const int n_triangles, const std::vector<unsigned int>& indices,
                    const std::vector<Vertex>& vertices, std::vector<Face>& faces);
Color calc_color(BaryCoord bary, std::array<Pixel, 3>& vertices, int z);
int calc_z(BaryCoord bary, std::array<Pixel, 3>& pixels);


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
    g_width = 720;
    // g_width = 1720;
    g_height = 480;

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
    
    // get faces in world space
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    mesh2vertices(posBuf, vertices);
    mesh2triangles(triBuf.size() / 3, triBuf, vertices, faces);
    
    // get viewvolume of the world
    ViewVolume vvolume;
    vvolume.calc_vvolume(g_width, g_height);

    // calc the transformation matrix to project to pixel space
    PixelTransform pmatrix(g_width, g_height, vvolume);

    // alloc z buf
    std::vector<uint8_t> z_buffer(g_width * g_height);

    for (Face f : faces) {
        PixelFace pf = PixelFace(f, pmatrix); 

        BoundingBox bbox;
        bbox.calc_box(pf.pixels);

        for (int y = bbox.y_min; y <= bbox.y_max; ++y) {
		    for (int x = bbox.x_min; x <= bbox.x_max; ++x) {
			    Pixel p = Pixel(x, y, 0, Color());
                BaryCoord bary = p.calc_bary_coords(pf.pixels);
			    
                if (!bary.in_triangle()) {
                    continue;    
                } 

                int z = calc_z(bary, pf.pixels);
                if (z_buffer[y * g_width + x] < z) {
                    z_buffer[y * g_width + x] = z;
                    p.color = calc_color(bary, pf.pixels, z);
                    image->setPixel(x, y, p.color.r, p.color.g, p.color.b);
                }
            }
        }

    }

    //write out the image
    image->writeToFile(imgName);

    return 0;
}

/*
                if (z_buffer[y * g_width + x] < z) {
                    
                    if (is_center(bary)) {
                        int radius = 2;
                        for (int row = y - radius; row <= y + radius; row++) {
                            for (int col = x - radius; col <= x + radius; col++) {
                                image->setPixel(col, row, 255, 255, 255);
                                z_buffer[row * g_width + col] = z;
                            }
                        }

                    } else {
                        z_buffer[y * g_width + x] = z;
                        p.color = calc_color(bary, pf.pixels, z);
                        image->setPixel(x, y, p.color.r, p.color.g, p.color.b);
                    }
                }
*/


void mesh2vertices(const std::vector<float>& positions, 
                    std::vector<Vertex>& vertices) {
    std::array<Color, 3> colors = {Color(255, 0, 0), Color(0, 255, 0), 
									Color(0, 0, 255)};
    for (size_t i = 0; i < positions.size(); i += 3) {
        Color c = colors[(rand() * rand()) % 3];
        // Color c = Color(0xff, 0x33, 0x77);
        // Color c = Color(rand(), rand(), rand());
        vertices.push_back(Vertex(positions[i], positions[i + 1], 
                            positions[i + 2], c));
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

Color calc_color(BaryCoord bary, std::array<Pixel, 3>& pixels, int z) {
    uint16_t r, g, b;
    r = 0;
    g = 0;
    b = 0;

    // triangle is average of 3 verts
    for (int i = 0; i < 3; i++) {
        r += pixels[i].color.r;
        g += pixels[i].color.g;
        b += pixels[i].color.b;
    }
    r /= 3.0f;
    g /= 3.0f;
    b /= 3.0f;

    // color blending
    // r = bary.alpha * pixels[0].color.r + bary.beta * pixels[1].color.r +
                // bary.gamma * pixels[2].color.r;
    // g = bary.alpha * pixels[0].color.g + bary.beta * pixels[1].color.g +
                // bary.gamma * pixels[2].color.g;
    // b = bary.alpha * pixels[0].color.b + bary.beta * pixels[1].color.b +
                // bary.gamma * pixels[2].color.b;
    
    // depth coloring
    // r *= static_cast<float>(z) / 255;
    // b *= static_cast<float>(z) / 255;
    // g *= static_cast<float>(z) / 255;
    
    return Color(r, g, b);
}

int calc_z(BaryCoord bary, std::array<Pixel, 3>& pixels) {
    float zw = bary.alpha * pixels[0].z + bary.beta * pixels[1].z + 
                bary.gamma * pixels[2].z;
    return static_cast<int>((zw + 1) * 127.5);
}