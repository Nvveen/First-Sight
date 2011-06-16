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
    glm::vec4 rgba;
    unsigned char r, g, b, a;
    volData_ = new Octree<Voxel>(log(size_)/log(2));
    for ( int i = 0; i < size_; i += 1 ) {
        for ( int j = 0; j < size_; j += 1 ) {
            for ( int k = 0; k < size_; k += 1 ) {
                if ( file.good() ) {
                    unsigned int x, y, z;
                    unsigned char r, g, b, a;
                    file >> x; file >> y; file >> z;
                    file >> r; file >> g; file >> b; file >> a;
                    rgba[0] = (float)r/255.0f;
                    rgba[1] = (float)g/255.0f;
                    rgba[2] = (float)b/255.0f;
                    rgba[3] = (float)a/255.0f;
                    if ( rgba[3] != 0.0f ) {
                        Voxel *voxel = new Voxel(rgba, x, y, z);
                        volData_->insert(x, y, z, voxel);
                        voxelList_.push_back(voxel);
                    }
                }
            }
        }
    }
    file.close();
    genVoxelField();
}		// -----  end of method Model::init  -----

    void
Model::genVoxelField ()
{
    for ( std::list<Voxel *>::iterator it = voxelList_.begin(); 
          it != voxelList_.end(); it++  ) {
        bool hidden = true;
        for ( unsigned short p = 0; p < 6; p += 1 )
            if ( !volData_->hasNeighbor((*it)->x_, (*it)->y_, (*it)->z_, 
                                        (CubeSides::Sides)p) ) {
                hidden = false;
                break;
            }
        if ( hidden ) {
            voxelList_.erase(it);
            --it;
        }
    }
    int nrPixels = voxelList_.size()*2;
    int width, height;
    nrPixels -= 1;
    nrPixels |= nrPixels >> 1;
    nrPixels |= nrPixels >> 2;
    nrPixels |= nrPixels >> 4;
    nrPixels |= nrPixels >> 8;
    nrPixels |= nrPixels >> 16;
    nrPixels += 1;
    width = pow(2, ((log(nrPixels)/log(2))/2));
    height = width;
    std::vector<unsigned char> texMap;
    texMap.reserve(nrPixels);
    for ( std::list<Voxel *>::iterator it = voxelList_.begin();
          it != voxelList_.end(); it++ ) {
        texMap.push_back((unsigned char)((*it)->rgba_[0]*255.0f));
        texMap.push_back((unsigned char)((*it)->rgba_[1]*255.0f));
        texMap.push_back((unsigned char)((*it)->rgba_[2]*255.0f));
        texMap.push_back((unsigned char)((*it)->rgba_[3]*255.0f));
        texMap.push_back((unsigned char)((*it)->x_));
        texMap.push_back((unsigned char)((*it)->y_));
        texMap.push_back((unsigned char)((*it)->z_));
        texMap.push_back(0);
    }
    for ( unsigned int i = texMap.size(); i < nrPixels; i += 1 )
        texMap.push_back(0);

    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_2D, texID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &texMap[0]);
}		// -----  end of method Model::genVoxelField  -----
    void
Model::createVertexData ( std::vector<GLfloat>& data, 
                          std::vector<unsigned int>& indices )
{
    data.reserve(24);
    data.push_back(0.0f); data.push_back(0.0f); data.push_back(0.0f); // 0
    data.push_back(0.0f); data.push_back(1.0f); data.push_back(0.0f); // 1
    data.push_back(1.0f); data.push_back(1.0f); data.push_back(0.0f); // 2
    data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // 3
    data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f); // 4
    data.push_back(0.0f); data.push_back(1.0f); data.push_back(1.0f); // 5
    data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f); // 6
    data.push_back(1.0f); data.push_back(0.0f); data.push_back(1.0f); // 7

    indices.reserve(20);
    indices.push_back(0); indices.push_back(1);
    indices.push_back(3); indices.push_back(2);
    indices.push_back(7); indices.push_back(6);
    indices.push_back(6); indices.push_back(2);
    indices.push_back(5); indices.push_back(1);
    indices.push_back(4); indices.push_back(0);
    indices.push_back(7); indices.push_back(3);
    indices.push_back(3); indices.push_back(7);
    indices.push_back(7); indices.push_back(6);
    indices.push_back(4); indices.push_back(5);
//
//    data.push_back(0.0f); data.push_back(0.0f); data.push_back(0.0f); // 0
//    data.push_back(0.0f); data.push_back(1.0f); data.push_back(0.0f); // 1
//
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // 3
//    data.push_back(1.0f); data.push_back(1.0f); data.push_back(0.0f); // 2
//    
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(1.0f); // 7
//    data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f); // 6
//
//    data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f); // 6
//    data.push_back(1.0f); data.push_back(1.0f); data.push_back(0.0f); // 2
//
//    data.push_back(0.0f); data.push_back(1.0f); data.push_back(1.0f); // 5
//    data.push_back(0.0f); data.push_back(1.0f); data.push_back(0.0f); // 1
//
//    data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f); // 4
//    data.push_back(0.0f); data.push_back(0.0f); data.push_back(0.0f); // 0
//
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(1.0f); // 7
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // 3
//
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(0.0f); // 3
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(1.0f); // 7
//
//    data.push_back(1.0f); data.push_back(0.0f); data.push_back(1.0f); // 7
//    data.push_back(1.0f); data.push_back(1.0f); data.push_back(1.0f); // 6
//
//    data.push_back(0.0f); data.push_back(0.0f); data.push_back(1.0f); // 4
//    data.push_back(0.0f); data.push_back(1.0f); data.push_back(1.0f); // 5
//    return data;
}		// -----  end of method Model::createVertexData  -----

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

