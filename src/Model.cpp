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
#include    <algorithm>
#include    <glm/gtc/matrix_transform.hpp>
#include    "Model.h"

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ( const std::string& fileName, GLfloat x, GLfloat y, GLfloat z,
               Context& context, Shader& shader ) :
    fileName_(fileName), location_(glm::vec3(x, y, z)), animID_(-1),
    animLoop_(false), shader_(&shader), perspective_(&context.getPerspective()),
    camera_(&context.getCamera())
{
    translation_ = glm::mat4(1.0f);
    rotation_ = glm::mat4(1.0f);
    scaling_ = glm::mat4(1.0f);
    init();
    move(x*32.0f, y*32.0f, z*32.0f);
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  init
// Description:  Initializes the Model.
//-----------------------------------------------------------------------------
    void
Model::init ()
{
    // Read file.
    std::ifstream file(fileName_.c_str(), std::ios::binary);
    if ( !file.is_open() ) {
        std::cerr << "Error reading file " << fileName_ << "\n";
        exit(1);
    }
    // Number of limbs.
    Byte numLimbs;
    file.read(reinterpret_cast<char *>(&numLimbs), sizeof(Byte));
    limbList_.reserve((Uint)numLimbs);
    glm::vec3 minSide(0, 0, 0), maxSide(0, 0, 0);
    for ( Uint i = 0; i < numLimbs; i += 1 ) {
        // Voxels in a limb.
        Uint numVoxels;
        Byte temp;
        // Offset the limb has.
        file.read(reinterpret_cast<char *>(&numVoxels), sizeof(Uint));
        std::array<float, 3> offset;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[0] = (float)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[1] = (float)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[2] = (float)temp;
        // Bounding box of the limb.
        std::array<Uint, 3> boxSize;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[0] = (Uint)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[1] = (Uint)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[2] = (Uint)temp;
        std::list<Voxel> limb;
        // Read every voxels in the limb.
        for ( Uint j = 0; j < numVoxels; j += 1 ) {
            Voxel v;
            file.read(reinterpret_cast<char *>(&v.x), sizeof(Byte));
            file.read(reinterpret_cast<char *>(&v.y), sizeof(Byte));
            file.read(reinterpret_cast<char *>(&v.z), sizeof(Byte));
            Byte info;
            file.read(reinterpret_cast<char *>(&info), sizeof(Byte));
            v.rgba[0] = (GLfloat)info / 255.0f;
            file.read(reinterpret_cast<char *>(&info), sizeof(Byte));
            v.rgba[1] = (GLfloat)info / 255.0f;
            file.read(reinterpret_cast<char *>(&info), sizeof(Byte));
            v.rgba[2] = (GLfloat)info / 255.0f;
            file.read(reinterpret_cast<char *>(&info), sizeof(Byte));
            v.rgba[3] = (GLfloat)info / 255.0f;
            limb.push_back(v);
        }
        // If the limb is the first limb read, then we don't have to
        // increase the size of the bounding box for possible animations.
        bool moveable = false;
        if ( i > 0 ) moveable = true;
        // We also need to offset the offset to put the center of the model
        // at location_.
        minSide[0] = std::min(offset[0], minSide[0]);
        minSide[1] = std::min(offset[1], minSide[1]);
        minSide[2] = std::min(offset[2], minSide[2]);
        maxSide[0] = std::max(offset[0]+boxSize[0], maxSide[0]);
        maxSide[1] = std::max(offset[1]+boxSize[1], maxSide[1]);
        maxSide[2] = std::max(offset[2]+boxSize[2], maxSide[2]);
        offset[0] -= (maxSide[0]-minSide[0])/2;
        offset[1] -= (maxSide[1]-minSide[1])/2;
        offset[2] -= (maxSide[2]-minSide[2])/2;
        limbList_.emplace(limbList_.end(), limb, offset, boxSize, 
                                                moveable);
    }
    file.close();
    // Force the MVP matrix to update.
    setMVP(true);
}		// -----  end of method Model::init  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  draw
// Description:  Draw the model by looking up its transformation animation
//               matrix, binding the buffers and calling OpenGL.
//-----------------------------------------------------------------------------
    void
Model::draw ()
{
    // Bind the shader.
    shader_->bind();
    // If need be, update the MVP (meaning, no force).
    setMVP();
    // Set the MVP in the shader.
    shader_->setUniform("vMVP", mvp_);
    Uint i = 0;
    for ( Limb& l : limbList_ ) {
        glm::mat4 transMat = glm::mat4(1.0f);
        if ( animID_ >= 0 ) {
            // Get the correct animation
            auto animIt = l.anims_.begin()+animID_;
            // Calculate elapsed time and the corresponding offset to a frame
            auto elapsed = std::chrono::system_clock::now() - animBegin_;
            auto offset = elapsed / std::chrono::milliseconds(50);
            if ( elapsed >= duration_ ) {
                if ( animLoop_ ) {
                    // Reset if looping
                    animBegin_ = std::chrono::system_clock::now();
                }
                // But always reduce offset to 0
                offset = 0;
            }
            // Finally, if the offset exceeds the number of frames, we need
            // to wrap around.
            if ( offset >= (*animIt).size()-1 )
                offset %= (*animIt).size();

            // Found the correct matrix, now set it.
            transMat = *((*animIt).begin() + offset);
            if ( (*animIt).size() > 1 ) {
                // Because of z-fighting occuring with animating, I've
                // found an arbitrary polgon offset, that seems to cure most
                // visible artifacts.
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(0, 500);
            }
            else {
                // We don't want to offset the polygons for stationary limbs.
                glDisable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(0, 0);
            }
        }
        // Bind buffers.
        glBindVertexArray(l.vao_);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // Set the animation transformation in the shader.
        shader_->setUniform("texTransform", transMat);
        // Bind its model.
        glBindTexture(GL_TEXTURE_3D, l.texID_);
        // Draw.
        glDrawArrays(GL_TRIANGLES, 0, l.vertexCount_);
        i += 1;
    }
    // Clean up.
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader_->unbind();
}		// -----  end of method Model::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  setMVP
// Description:  Calculate the MVP matrix if force or when the camera changes.
//-----------------------------------------------------------------------------
    void
Model::setMVP ( bool force )
{
    if ( camera_->changed() || force ) {
        mvp_ = perspective_->getMatrix() * camera_->getCamera() * translation_ *
               rotation_ * scaling_;
    }
}		// -----  end of method Model::setMVP  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  Model::Limb
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Limb::Limb ()
{
}  // -----  end of method Model::Limb::Model::Limb  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  Model::Limb
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Limb::Limb ( std::list<Voxel>& voxels, std::array<float, 3>& offset,
                           std::array<Uint, 3>& boxSize, bool moveable ) :
    offset_(offset), boxSize_(boxSize)
{
    createVoxelImage(voxels, moveable);
    createVBO();
}  // -----  end of method Model::Limb::Model::Limb  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  ~Model::Limb
// Description:  destructor
//-----------------------------------------------------------------------------
Model::Limb::~Limb ()
{
    glDeleteBuffers(1, &vbo_);
    glDeleteVertexArrays(1, &vao_);
    glDeleteTextures(1, &texID_);
}  // -----  end of method Model::Limb::~Model::Limb  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  createVoxelImage
// Description:  Generate a texture from the voxel data we have read.
//-----------------------------------------------------------------------------
    void
Model::Limb::createVoxelImage ( std::list<Voxel>& voxels, bool moveable )
{
    // We need to take into account that animating might exceed the normal
    // bounding box of a limb.
    if ( moveable ) {
        // So get the size of the largest axis.
        auto it = std::max_element(boxSize_.begin(), boxSize_.end());
        Uint maxSize = *it;
        // And update all other box axis.
        for ( Uint& d : boxSize_ ) {
            if ( d != maxSize ) d += maxSize;
            d += maxSize;
        }
        // Then offset the voxels and the box itself by the right amount
        for ( Voxel& v : voxels ) {
            v.x += maxSize; v.y += maxSize; v.z += maxSize;
            if ( it == boxSize_.begin() ) v.x += maxSize;
            else if ( it == boxSize_.begin()+1 ) v.y += maxSize;
            else if ( it == boxSize_.begin()+2 ) v.z += maxSize;
        }
        if ( it != boxSize_.begin() ) offset_[0] -= maxSize;
        if ( it != boxSize_.begin()+1 ) offset_[1] -= maxSize;
        if ( it != boxSize_.begin()+2 ) offset_[2] -= maxSize;
    }
    // And voila, we have a limb with the freedom to move if its not
    // stationary.
    std::vector<glm::vec4> texMap(boxSize_[0]*boxSize_[1]*boxSize_[2],
                                  glm::vec4(0));
    // Next, we create an OpenGL texture.
    for ( Voxel& v : voxels ) {
        Uint index = v.z*boxSize_[0]*boxSize_[1] + v.y*boxSize_[0] + v.x;
        texMap[index] = v.rgba;
    }
    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_3D, texID_);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, boxSize_[0], boxSize_[1],
                 boxSize_[2], 0, GL_RGBA, GL_FLOAT, &texMap[0]);
    glBindTexture(GL_TEXTURE_3D, 0);
}		// -----  end of method Model::createVoxelImage  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  createVBO
// Description:  Generate 6 stacks of quads, 2 for each axis in an amount equal
//               to the size of the texture/model on that axis.
//-----------------------------------------------------------------------------
    void
Model::Limb::createVBO ()
{
    std::vector<GLfloat> data;
    // Helper variables so the typed size isn't so big.
    std::array<GLfloat, 3> b = { (GLfloat)boxSize_[0], (GLfloat)boxSize_[1],
                               (GLfloat)boxSize_[2] };
    std::array<GLfloat, 3> o = offset_;
    // Add all slices in the x-axis, ...
    for ( GLfloat x = o[0]; x < b[0]+o[0]; x += 1 ) {
        GLfloat r = (x-o[0]+0.5f)/b[0];
        std::vector<GLfloat> slice = {
            x, o[1],      o[2]+b[2], r, 0.0f, 1.0f,
            x, o[1]+b[1], o[2]+b[2], r, 1.0f, 1.0f,
            x, o[1]+b[1], o[2],      r, 1.0f, 0.0f,
            x, o[1],      o[2]+b[2], r, 0.0f, 1.0f,
            x, o[1]+b[1], o[2],      r, 1.0f, 0.0f,
            x, o[1],      o[2],      r, 0.0f, 0.0f,

            x+1, o[1],      o[2],      r, 0.0f, 0.0f,
            x+1, o[1]+b[1], o[2],      r, 1.0f, 0.0f,
            x+1, o[1]+b[1], o[2]+b[2], r, 1.0f, 1.0f,
            x+1, o[1],      o[2],      r, 0.0f, 0.0f,
            x+1, o[1]+b[1], o[2]+b[2], r, 1.0f, 1.0f,
            x+1, o[1],      o[2]+b[2], r, 0.0f, 1.0f,
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    // ...y-axis, ...
    for ( GLfloat y = o[1]; y < b[1]+o[1]; y += 1 ) {
        GLfloat s = (y-o[1]+0.5f)/b[1];
        std::vector<GLfloat> slice = {
            o[0],      y, o[2]+b[2], 0.0f, s, 1.0f,
            o[0],      y, o[2],      0.0f, s, 0.0f,
            o[0]+b[0], y, o[2],      1.0f, s, 0.0f,
            o[0],      y, o[2]+b[2], 0.0f, s, 1.0f,
            o[0]+b[0], y, o[2],      1.0f, s, 0.0f,
            o[0]+b[0], y, o[2]+b[2], 1.0f, s, 1.0f,

            o[0],      y+1, o[2],      0.0f, s, 0.0f,
            o[0],      y+1, o[2]+b[2], 0.0f, s, 1.0f,
            o[0]+b[0], y+1, o[2]+b[2], 1.0f, s, 1.0f,
            o[0],      y+1, o[2],      0.0f, s, 0.0f,
            o[0]+b[0], y+1, o[2]+b[2], 1.0f, s, 1.0f,
            o[0]+b[0], y+1, o[2],      1.0f, s, 0.0f,
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    // ...and z-axis.
    for ( GLfloat z = o[2]; z < b[2]+o[2]; z += 1 ) {
        GLfloat t = (z-o[2]+0.5)/b[2];
        std::vector<GLfloat> slice = {
            o[0],      o[1],        z, 0.0f, 0.0f, t,
            o[0],      o[1]+b[1],   z, 0.0f, 1.0f, t,
            o[0]+b[0], o[1]+b[1],   z, 1.0f, 1.0f, t,
            o[0],      o[1],        z, 0.0f, 0.0f, t,
            o[0]+b[0], o[1]+b[1],   z, 1.0f, 1.0f, t,
            o[0]+b[0], o[1],        z, 1.0f, 0.0f, t,

            o[0]+b[0], o[1],      z+1, 1.0f, 0.0f, t,
            o[0]+b[0], o[1]+b[1], z+1, 1.0f, 1.0f, t,
            o[0],      o[1]+b[1], z+1, 0.0f, 1.0f, t,
            o[0]+b[0], o[1],      z+1, 1.0f, 0.0f, t,
            o[0],      o[1]+b[1], z+1, 0.0f, 1.0f, t,
            o[0],      o[1],      z+1, 0.0f, 0.0f, t,
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    // Create the buffer for the entire stack.
    vertexCount_ = data.size()/6;
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*data.size(), &data[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6,
                          (const GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*6,
                          (const GLvoid *)12);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}		// -----  end of method Model::createVBO  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  move
// Description:  Move the model around.
//-----------------------------------------------------------------------------
    void
Model::move ( GLfloat x, GLfloat y, GLfloat z )
{
    translation_ *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    setMVP(true);
}		// -----  end of method Model::move  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  rotate
// Description:  Rotate the model around an axis.
//-----------------------------------------------------------------------------
    void
Model::rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    rotation_ *= glm::translate(glm::mat4(1.0f), glm::vec3(-location_[0],
                                                           -location_[1],
                                                           -location_[2]));
    rotation_ *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z));
    rotation_ *= glm::translate(glm::mat4(1.0f), glm::vec3(location_[0],
                                                           location_[1],
                                                           location_[2]));
    setMVP(true);
}		// -----  end of method Model::rotate  -----

