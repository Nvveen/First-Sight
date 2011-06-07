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
    std::ifstream file(fileName_.c_str());
    if ( !file.is_open() ) {
        std::cerr << "Error reading file " << fileName_ << "\n";
        exit(1);
    }
    file >> size_;
    int depth = log(size_)/log(2);
    volData_ = new Octree<Voxel>(depth);
    int i(0), j(0), k(0);
    while ( file.good() ) {
        glm::vec4 rgba;
        unsigned char r, g, b, a;
        file >> r;
        file >> g;
        file >> b;
        file >> a;
        rgba[0] = (float)r/255.0f;
        rgba[1] = (float)g/255.0f;
        rgba[2] = (float)b/255.0f;
        rgba[3] = (float)a/255.0f;
        if ( rgba[3] != 0.0f ) {
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
    file.close();
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
                        vertexData.push_back(vert[q*7+offset] + 
                                    (GLfloat)voxelList_[i]->x_);
                        vertexData.push_back(vert[q*7+offset+1] + 
                                    (GLfloat)voxelList_[i]->y_);
                        vertexData.push_back(vert[q*7+offset+2] + 
                                    (GLfloat)voxelList_[i]->z_);
                        vertexData.push_back(vert[q*7+offset+3] + 
                                    voxelList_[i]->rgba_[0]);
                        vertexData.push_back(vert[q*7+offset+4] + 
                                    voxelList_[i]->rgba_[1]);
                        vertexData.push_back(vert[q*7+offset+5] + 
                                    voxelList_[i]->rgba_[2]);
                        vertexData.push_back(vert[q*7+offset+6] + 
                                    voxelList_[i]->rgba_[3]);
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

