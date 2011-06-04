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
    const std::vector<GLfloat> voxelData = read();
    std::vector<Voxel> voxels = constructVoxels(voxelData);
    fillOctree(voxels);
}		// -----  end of method Model::init  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  read
// Description:  Reads and parses a stream to obtain volumetric data from it.
//-----------------------------------------------------------------------------
    std::vector<GLfloat>
Model::read ()
{
    // Create needed variables.
    std::vector<GLfloat> voxels;
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
            voxels.push_back((GLfloat)tempr/255.0f);
            voxels.push_back((GLfloat)tempg/255.0f);
            voxels.push_back((GLfloat)tempb/255.0f);
            voxels.push_back((GLfloat)tempa/255.0f);
        }
    }
    return voxels;
}		// -----  end of method Model::read  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  constructVoxels
// Description:  Reads in an array of unformatted voxel data and turns it into
//               an array of voxels.
//-----------------------------------------------------------------------------
    std::vector<Model::Voxel>
Model::constructVoxels ( const std::vector<GLfloat>& voxelData )
{
    std::vector<Voxel> voxels;
    for ( unsigned int i = 0; i < voxelData.size(); i += 4 ) {
        // Read every 4 values into a vec4, make a voxel out of it and add it
        // to the list.
        glm::vec4 vec;
        vec.r = voxelData[i]; vec.g = voxelData[i+1];
        vec.b = voxelData[i+2]; vec.a = voxelData[i+3];
        Voxel voxel(vec);
        voxels.push_back(voxel);
    }
    return voxels;
}		// -----  end of method Model::constructVoxels  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  fillOctree
// Description:  Fill an Octree with voxels.
//-----------------------------------------------------------------------------
    void
Model::fillOctree ( const std::vector<Voxel>& voxels )
{
    // Using an array of voxels, we can fill the Octree with it by inserting it
    // value by value.
    int q = 0;
    for ( int i = 0; i < size_; i += 1 ) {
        for ( int j = 0; j < size_; j += 1 ) {
            for ( int k = 0; k < size_; k += 1 ) {
                Voxel voxel(voxels[q].rgba_, i, j, k);
                // Don't add an invisible voxel.
                if ( voxel.rgba_ != glm::vec4(0.0f, 0.0f, 0.0f, 0.0f) )
                    volData_->insert(i, j, k, voxel);
                q += 1;
            }
        }
    }
}		// -----  end of method Model::fillOctree  -----

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
    int edge(0);
    for ( int i = 0; i < size_; i += 1 ) {
        for ( int j = 0; j < size_; j += 1 ) {
            for ( int k = 0; k < size_; k += 1 ) {
                // For every voxel in the octree, check its neighbors.
                Voxel *voxel = (*volData_)(i, j, k);
                if ( voxel != NULL ) {
                    // If no neighbors on any of the six faces, we can expose
                    // that surface.
                    for ( int p = 0; p < 6; p += 1 ) {
                        if ( !volData_->hasNeighbor(i, j, k, 
                                                    (CubeSides::Sides)p) ) {
                            // We select the surface from the array of vertices
                            // that are ordered per face.
                            // We then add that face's vertices to the array,
                            // after which we skip the iterator past the new
                            // face.
                            vertexData.insert(vertexData.begin()+edge, 
                                              voxel->vertices_.begin()+p*42,
                                              voxel->vertices_.begin()+p*42+42);
                            edge += 42;
                        }
                    }
                }
            }
        }
    }
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
    init ();
}  // -----  end of method Voxel::Voxel  (constructor)  -----

    void
Model::Voxel::init ()
{
    GLfloat vertices[252] = {
        // Front
         0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Front
         0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Left
        -0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Left
        -0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Bottom
         0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Bottom
         0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Back
         0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Back
         0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Right
         0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_, -0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Right
         0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_, -0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Top
         0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        // Top
         0.5f+x_,  0.5f+y_, -0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
        -0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
         0.5f+x_,  0.5f+y_,  0.5f+z_, rgba_.r, rgba_.g, rgba_.b, rgba_.a,
    };
    vertices_ = std::vector<GLfloat>(vertices, vertices+252);
}		// -----  end of method Voxel::init  -----

