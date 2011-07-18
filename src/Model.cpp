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
Model::Model ( const std::string& fileName ) :
    fileName_(fileName)
{
    init();
    animID_ = -1;
    animLoop_ = false;
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
    Byte numLimbs;
    file.read(reinterpret_cast<char *>(&numLimbs), sizeof(Byte));
    for ( Uint i = 0; i < numLimbs; i += 1 ) {
        Uint numVoxels;
        Byte temp;
        file.read(reinterpret_cast<char *>(&numVoxels), sizeof(Uint));
        std::vector<float> offset(3);
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[0] = (float)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[1] = (float)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        offset[2] = (float)temp;
        std::vector<Uint> boxSize(3);
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[0] = (Uint)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[1] = (Uint)temp;
        file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
        boxSize[2] = (Uint)temp;
        std::list<Voxel> limb;
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
        bool moveable = false;
        if ( i > 0 ) moveable = true;
        limbList_.push_back(Limb(limb, offset, boxSize, moveable));
    }
    file.close();
}		// -----  end of method Model::init  -----

    void
Model::draw ( Shader& shader )
{
    Uint i = 0;
    for ( Limb& l : limbList_ ) {
        glm::mat4 transMat = glm::mat4(1.0f);
        if ( animID_ >= 0 ) {
            auto animIt = l.anims_.begin()+animID_;
            auto elapsed = std::chrono::system_clock::now() - animBegin_;
            if ( elapsed >= animDurations_[animID_] ) {
                if ( animLoop_ ) {
                    elapsed %= (*animIt).size();
                    animBegin_ = std::chrono::system_clock::now();
                }
                else { 
                    elapsed = std::chrono::system_clock::duration(0);
                }
            }
            auto offset = elapsed / std::chrono::milliseconds(50);
            if ( offset >= (*animIt).size() ) offset = 0;
            transMat = *((*animIt).begin() + offset);
            if ( (*animIt).size() > 1 ) {
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(0, 500);
            }
            else {
                glDisable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(0, 0);
            }
        }
        glBindVertexArray(l.vao_);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        shader.setUniform("texTransform", transMat);
        glBindTexture(GL_TEXTURE_3D, l.texID_);
        glDrawArrays(GL_TRIANGLES, 0, l.vertexCount_);
        i += 1;
    }
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}		// -----  end of method Model::draw  -----

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
Model::Limb::Limb ( std::list<Voxel>& voxels, std::vector<float>& offset,
                           std::vector<Uint>& boxSize, bool moveable ) :
    offset_(offset), boxSize_(boxSize)
{
    createVoxelImage(voxels, moveable);
    createVBO();
}  // -----  end of method Model::Limb::Model::Limb  (constructor)  -----

    void
Model::Limb::createVoxelImage ( std::list<Voxel>& voxels, bool moveable )
{
    if ( moveable ) {
        auto it = std::max_element(boxSize_.begin(), boxSize_.end());
        Uint maxSize = *it;
        for ( Uint& d : boxSize_ ) {
            if ( d != maxSize ) d += maxSize;
            d += maxSize;
        }
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
    std::vector<glm::vec4> texMap(boxSize_[0]*boxSize_[1]*boxSize_[2],
                                  glm::vec4(0));
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

    void
Model::Limb::createVBO ()
{
    std::vector<GLfloat> data;
    std::vector<GLfloat> b = { (GLfloat)boxSize_[0], (GLfloat)boxSize_[1],
                               (GLfloat)boxSize_[2] };
    std::vector<GLfloat> o = offset_;
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
