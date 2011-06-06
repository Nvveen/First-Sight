// ============================================================================
// 
//       Filename:  Model.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/11/2011 01:35:21 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <fstream>
#include    "Model.h"

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ( const std::string& fileName ) :
    volData_(NULL), fileName_(fileName), size_(0)
{
    init();
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  init
// Description:  Initializes the Model.
//-----------------------------------------------------------------------------
    void
Model::init ()
{
    // Create needed variables.
    std::ifstream file(fileName_);
    std::string line, item;
    // These masking variables are explained later.
    unsigned long maskr(255 << 24), maskg(255 << 16), maskb(255 << 8), 
                  maska(255);
    unsigned long tempr(0), tempg(0), tempb(0), tempa(0);

    // The first line contains the size in a x,y,z pattern. We need to find the
    // biggest size possible to fit the whole Octree.
    getline(file, line);
    std::istringstream sizestream(line);
    while ( sizestream.good() ) {
        // Split the line at the commas.
        getline(sizestream, item, ',');
        std::istringstream iss(item);
        // Put value into an integer.
        int temp = 0;
        iss >> temp;
        // If the current value is bigger than size, update size.
        if ( temp > size_ ) size_ = temp;
    }

    // We have a size, so we can build the Octree.
    volData_ = new Octree<Voxel>(log(size_)/log(2));
    int i(0), j(0), k(0);
    // Read the rest of the file.
    while ( file.good() ) {
        // For every line, we split it at the commas again to get the color
        // values for every voxel in a hexadecimal format starting with a #.
        getline(file, line);
        std::istringstream iss(line);
        while ( iss.good() && line.size() > 0 ) {
            // Split with identifier.
            getline(iss, item, ',');
            // Remove the #
            item.erase(0,1);
            // Set the plain text hex format to a stream for conversion, and
            // read in the stream in a hexidecimal format.
            std::istringstream itemstream(item);
            itemstream.setf(std::ios::hex, std::ios::basefield);
            // Each converted value is now a decimal value containing the red,
            // green, blue and alpha values of the voxel. To extract the
            // seperate rgba values from this value, we mask it 4 times against
            // its corresponding color, and bitshift the value to get a number
            // from 0-255.
            itemstream >> tempr;
            tempg = tempb = tempa = tempr;
            // Mask
            tempr &= maskr; tempg &= maskg; tempb &= maskb; tempa &= maska;
            // Bitshift
            tempr = tempr >> 24; tempg = tempg >> 16; tempb = tempb >> 8;
            // Shift range from 0..255 to 0.0..1.0 and add it to the voxel data.
            GLfloat r = (GLfloat)tempr/255.0f;
            GLfloat g = (GLfloat)tempg/255.0f;
            GLfloat b = (GLfloat)tempb/255.0f;
            GLfloat a = (GLfloat)tempa/255.0f;
            glm::vec4 rgba(r, g, b, a);
            if ( a > 0.0f ) {
                Voxel *voxel = new Voxel(rgba, i, j, k);
                volData_->insert(i, j, k, voxel);
                voxelList_.push_back(voxel);
            }
            k += 1;
            if ( k >= size_ ) {
                j += 1;
                k = 0;
            }
            if ( j >= size_ ) {
                i += 1;
                j = 0;
            }
        }
    }
}		// -----  end of method Model::init  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  getVertexData
// Description:  Runs through the Octree of Voxels and for every Voxel decides
//               which vertices to show by checking to see if they are exposed.
//-----------------------------------------------------------------------------
    std::vector<GLfloat>
Model::getVertexData ()
{
    std::vector<GLfloat> vertexData;
    vertexData.reserve(252*size_*size_*size_);
    GLfloat vert[252] = {
        // Front
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Front
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Left
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Left
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Bottom
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Bottom
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Back
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Back
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Right
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Right
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Top
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Top
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    };
    int edge = 0;
    for ( unsigned int i = 0; i < voxelList_.size(); i += 1 ) {
        if ( voxelList_[i] != NULL ) {
            for ( int p = 0; p < 6; p += 1 ) {
                if ( !volData_->hasNeighbor(voxelList_[i]->x_,
                                            voxelList_[i]->y_,
                                            voxelList_[i]->z_,
                                            (CubeSides::Sides)p) ) {
                    int offset = p*42;
                    for ( int q = 0; q < 6; q += 1 ) {
                        GLfloat x = vert[q*7+offset] + 
                                    (GLfloat)voxelList_[i]->x_;
                        GLfloat y = vert[q*7+offset+1] + 
                                    (GLfloat)voxelList_[i]->y_;
                        GLfloat z = vert[q*7+offset+2] + 
                                    (GLfloat)voxelList_[i]->z_;
                        GLfloat r = vert[q*7+offset+3] + 
                                    voxelList_[i]->rgba_[0];
                        GLfloat g = vert[q*7+offset+4] + 
                                    voxelList_[i]->rgba_[1];
                        GLfloat b = vert[q*7+offset+5] + 
                                    voxelList_[i]->rgba_[2];
                        GLfloat a = vert[q*7+offset+6] + 
                                    voxelList_[i]->rgba_[3];
                        vertexData.push_back(x);
                        vertexData.push_back(y);
                        vertexData.push_back(z);
                        vertexData.push_back(r);
                        vertexData.push_back(g);
                        vertexData.push_back(b);
                        vertexData.push_back(a);
                    }
                    edge += 42;
                }
            }
        }
    }
    vertexData.resize(edge);
    return vertexData;
}		// -----  end of method Model::getVertexData  -----

//-----------------------------------------------------------------------------
//       Class:  Voxel
//      Method:  Voxel
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Voxel::Voxel ( glm::vec4 rgba, unsigned int x, unsigned int y, 
                      unsigned int z ) :
    x_(x), y_(y), z_(z), rgba_(rgba)
{
}  // -----  end of method Voxel::Voxel  (constructor)  -----

