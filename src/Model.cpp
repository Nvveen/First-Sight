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
    std::ifstream file(fileName_.c_str(), std::ios::binary);
    if ( !file.is_open() ) {
        std::cerr << "Error reading file " << fileName_ << "\n";
        exit(1);
    }
    // Get the size first.
    unsigned char blockSize;
    unsigned int numAnim;
    file.read(reinterpret_cast<char *>(&blockSize), sizeof(unsigned char));
    size_ = (unsigned int)blockSize;
    file.read(reinterpret_cast<char *>(&numAnim), sizeof(unsigned int));
    // Initialize the octree.
    volData_ = new Octree<Voxel>(log(size_)/log(2));

    std::list<Voxel *>* currentList = &voxelList_;
    int q = 0; // Variable to keep track of which list we're modifying.
    while ( file.good() && q <= numAnim ) {
        unsigned int size;
        file.read(reinterpret_cast<char *>(&size), sizeof(unsigned int));
        for ( unsigned int i = 0; i < size; i += 1 ) {
            if ( !file.good() ) {
                std::cerr << "Error reading model " << fileName_ << "\n";
                exit(1);
            }
            unsigned char x, y, z, r, g, b, a;
            file.read(reinterpret_cast<char *>(&x), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&y), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&z), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&r), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&g), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&b), sizeof(unsigned char));
            file.read(reinterpret_cast<char *>(&a), sizeof(unsigned char));
            glm::vec4 rgba = glm::vec4((float)r/255.0f, (float)g/255.0f,
                                       (float)b/255.0f, (float)a/255.0f);
            Voxel *vox = new Voxel(rgba, (unsigned int)x, (unsigned int)y,
                                         (unsigned int)z);
            currentList->push_back(vox);
            if ( currentList == &voxelList_ ) {
                volData_->insert((unsigned int)x, (unsigned int)y,
                                 (unsigned int)z, vox);
            }
        }
        animationVoxels_.resize(animationVoxels_.size()+1);
        currentList = &animationVoxels_[q];
        q += 1;
    }
    animationVoxels_.pop_back(); // Code adds one too many lists, so remove.
    texID_ = createVoxelImage(voxelList_);
    for ( std::list<Voxel *>& list : animationVoxels_ ) {
        GLuint animID = createVoxelAnimation(list);
        animationTexIDs_.push_back(animID);
    }
    file.close();
}		// -----  end of method Model::init  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  createVoxelImage
// Description:  With the available voxel data, this method creates the texture.
//-----------------------------------------------------------------------------
    GLuint
Model::createVoxelImage ( std::list<Voxel *>& voxelList )
{
    GLuint texID;
    // First we create the data-array representing the voxel model from the
    // initial data we already have.
    texMap_ = std::vector<glm::vec4>(size_*size_*size_, glm::vec4(0.0f));
    // For every voxel that exists, we adjust that value in the empty array.
    for ( std::list<Voxel *>::iterator it = voxelList.begin(); 
          it != voxelList.end(); it++  ) {
        unsigned short i, j, k;
        i = (*it)->x_;
        j = (*it)->y_;
        k = (*it)->z_;
        int index = i*size_*size_ + j*size_ + k;
        texMap_[index] = (*it)->rgba_;
    }
    // Now we have the 3D array, we can make an OpenGL 3D texture.
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_3D, texID);
    // We don't want to interpolate between pixels.
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Fill the texture with data.
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, size_, size_, size_, 0, GL_RGBA,
                 GL_FLOAT, &texMap_[0]);
    return texID;
}		// -----  end of method Model::createVoxelImage  -----

    GLuint
Model::createVoxelAnimation ( std::list<Voxel *>& animList )
{
    GLuint texID;
    std::vector<glm::vec4> animMap = texMap_;
    for ( Voxel* v : animList ) {
        unsigned int index = v->x_*size_*size_ + v->y_*size_ + v->z_;
        animMap[index] = v->rgba_;
    }
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_3D, texID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, size_, size_, size_, 0, GL_RGBA,
                 GL_FLOAT, &animMap[0]);
    return texID;
}		// -----  end of method Model::createVoxelAnimation  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  createSliceField
// Description:  Create for each axis the front and back slices.
//-----------------------------------------------------------------------------
    std::vector<GLfloat>
Model::createSliceField ()
{
    // If we have the 3D texture that represents the model, we visualize it by
    // rendering slices of every pixel plane in an axis.
    std::vector<GLfloat> data(36*size_*6, 0.0f);
    unsigned int p = 0;
    for ( int i = size_-1; i >= 0; i -= 1 ) {
        GLfloat q = ((GLfloat)i-0.5f)/(GLfloat)size_; // steps in the texture.
        GLfloat r = (GLfloat)size_; // End points for the quads.
        // XY slice.
        data[p   ] = 0.0f; data[p+1 ] = 0.0f; data[p+2 ] = i;
        data[p+3 ] = 0.0f; data[p+4 ] = 0.0f; data[p+5 ] = q;
        data[p+6 ] = 0.0f; data[p+7 ] =    r; data[p+8 ] = i;
        data[p+9 ] = 0.0f; data[p+10] = 1.0f; data[p+11] = q;
        data[p+12] =    r; data[p+13] =    r; data[p+14] = i;
        data[p+15] = 1.0f; data[p+16] = 1.0f; data[p+17] = q;
        data[p+18] = 0.0f; data[p+19] = 0.0f; data[p+20] = i;
        data[p+21] = 0.0f; data[p+22] = 0.0f; data[p+23] = q;
        data[p+24] =    r; data[p+25] =    r; data[p+26] = i;
        data[p+27] = 1.0f; data[p+28] = 1.0f; data[p+29] = q;
        data[p+30] =    r; data[p+31] = 0.0f; data[p+32] = i;
        data[p+33] = 1.0f; data[p+34] = 0.0f; data[p+35] = q;
        p += 36;
        // XZ slice.
        data[p   ] =    r; data[p+1 ] =    i; data[p+2 ] = 0.0f;
        data[p+3 ] = 1.0f; data[p+4 ] =    q; data[p+5 ] = 0.0f;
        data[p+6 ] =    r; data[p+7 ] =    i; data[p+8 ] =    r;
        data[p+9 ] = 1.0f; data[p+10] =    q; data[p+11] = 1.0f;
        data[p+12] = 0.0f; data[p+13] =    i; data[p+14] =    r;
        data[p+15] = 0.0f; data[p+16] =    q; data[p+17] = 1.0f;
        data[p+18] =    r; data[p+19] =    i; data[p+20] = 0.0f;
        data[p+21] = 1.0f; data[p+22] =    q; data[p+23] = 0.0f;
        data[p+24] = 0.0f; data[p+25] =    i; data[p+26] =    r;
        data[p+27] = 0.0f; data[p+28] =    q; data[p+29] = 1.0f;
        data[p+30] = 0.0f; data[p+31] =    i; data[p+32] = 0.0f;
        data[p+33] = 0.0f; data[p+34] =    q; data[p+35] = 0.0f;
        p += 36;
        // YZ slice.
        data[p   ] =    i; data[p+1 ] = 0.0f; data[p+2 ] =    r;
        data[p+3 ] =    q; data[p+4 ] = 0.0f; data[p+5 ] = 1.0f;
        data[p+6 ] =    i; data[p+7 ] =    r; data[p+8 ] =    r;
        data[p+9 ] =    q; data[p+10] = 1.0f; data[p+11] = 1.0f;
        data[p+12] =    i; data[p+13] =    r; data[p+14] = 0.0f;
        data[p+15] =    q; data[p+16] = 1.0f; data[p+17] = 0.0f;
        data[p+18] =    i; data[p+19] = 0.0f; data[p+20] =    r;
        data[p+21] =    q; data[p+22] = 0.0f; data[p+23] = 1.0f;
        data[p+24] =    i; data[p+25] =    r; data[p+26] = 0.0f;
        data[p+27] =    q; data[p+28] = 1.0f; data[p+29] = 0.0f;
        data[p+30] =    i; data[p+31] = 0.0f; data[p+32] = 0.0f;
        data[p+33] =    q; data[p+34] = 0.0f; data[p+35] = 0.0f;
        p += 36;
    }
    // Backwards facing slices.
    for ( int i = 0; i < size_; i += 1 ) {
        GLfloat q = ((GLfloat)i+0.5f)/(GLfloat)size_;
        GLfloat r = (GLfloat)size_; // End points for the quads.
        // XY slice.
        data[p   ] =    r; data[p+1 ] = 0.0f; data[p+2 ] = i;
        data[p+3 ] = 1.0f; data[p+4 ] = 0.0f; data[p+5 ] = q;
        data[p+6 ] =    r; data[p+7 ] =    r; data[p+8 ] = i;
        data[p+9 ] = 1.0f; data[p+10] = 1.0f; data[p+11] = q;
        data[p+12] = 0.0f; data[p+13] =    r; data[p+14] = i;
        data[p+15] = 0.0f; data[p+16] = 1.0f; data[p+17] = q;
        data[p+18] =    r; data[p+19] = 0.0f; data[p+20] = i;
        data[p+21] = 1.0f; data[p+22] = 0.0f; data[p+23] = q;
        data[p+24] = 0.0f; data[p+25] =    r; data[p+26] = i;
        data[p+27] = 0.0f; data[p+28] = 1.0f; data[p+29] = q;
        data[p+30] = 0.0f; data[p+31] = 0.0f; data[p+32] = i;
        data[p+33] = 0.0f; data[p+34] = 0.0f; data[p+35] = q;
        p += 36;
        // XZ slice.
        data[p   ] = 0.0f; data[p+1 ] =    i; data[p+2 ] = 0.0f;
        data[p+3 ] = 0.0f; data[p+4 ] =    q; data[p+5 ] = 0.0f;
        data[p+6 ] = 0.0f; data[p+7 ] =    i; data[p+8 ] =    r;
        data[p+9 ] = 0.0f; data[p+10] =    q; data[p+11] = 1.0f;
        data[p+12] =    r; data[p+13] =    i; data[p+14] =    r;
        data[p+15] = 1.0f; data[p+16] =    q; data[p+17] = 1.0f;
        data[p+18] = 0.0f; data[p+19] =    i; data[p+20] = 0.0f;
        data[p+21] = 0.0f; data[p+22] =    q; data[p+23] = 0.0f;
        data[p+24] =    r; data[p+25] =    i; data[p+26] =    r;
        data[p+27] = 1.0f; data[p+28] =    q; data[p+29] = 1.0f;
        data[p+30] =    r; data[p+31] =    i; data[p+32] = 0.0f;
        data[p+33] = 1.0f; data[p+34] =    q; data[p+35] = 0.0f;
        p += 36;
        // YZ slice.
        data[p   ] =    i; data[p+1 ] = 0.0f; data[p+2 ] = 0.0f;
        data[p+3 ] =    q; data[p+4 ] = 0.0f; data[p+5 ] = 0.0f;
        data[p+6 ] =    i; data[p+7 ] =    r; data[p+8 ] = 0.0f;
        data[p+9 ] =    q; data[p+10] = 1.0f; data[p+11] = 0.0f;
        data[p+12] =    i; data[p+13] =    r; data[p+14] =    r;
        data[p+15] =    q; data[p+16] = 1.0f; data[p+17] = 1.0f;
        data[p+18] =    i; data[p+19] = 0.0f; data[p+20] = 0.0f;
        data[p+21] =    q; data[p+22] = 0.0f; data[p+23] = 0.0f;
        data[p+24] =    i; data[p+25] =    r; data[p+26] =    r;
        data[p+27] =    q; data[p+28] = 1.0f; data[p+29] = 1.0f;
        data[p+30] =    i; data[p+31] = 0.0f; data[p+32] =    r;
        data[p+33] =    q; data[p+34] = 0.0f; data[p+35] = 1.0f;
        p += 36;
    }
    return data;
}		// -----  end of method Model::createSliceField  -----

