// ============================================================================
// 
//       Filename:  Model.h
// 
//    Description:  A class that imports model data and loads them in a 
//                  usable format.
// 
//        Version:  1.0
//        Created:  04/11/2011 01:35:54 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  MODEL_H
#define  MODEL_H

#include    <vector>
#include    <algorithm>
#include    <list>
#include    <chrono>
#include    <fstream>
#include    <GL/glew.h>
#include    <glm/glm.hpp>

#include    "Shader.h"

// ============================================================================
//        Class:  Model
//  Description:  
// ============================================================================
class Model
{
    public:
        typedef unsigned char Byte;
        typedef unsigned int Uint;
        Model ( const std::string& fileName );
        void draw ( Shader& shader );
        template<class Func>
        void setAnimation ( Func f, Uint duration, Uint limb, bool loop );
        void startAnimation ( Uint limb );
    private:
        struct Voxel;
        class Limb;
        void init ();
        std::string fileName_;
        std::vector<Limb> limbList_;
}; // -----  end of class Model  -----

// ============================================================================
//        Class:  Model::Limb
//  Description:  
// ============================================================================
class Model::Limb
{
    public:
        Limb ();                             // constructor
        Limb ( std::list<Voxel>& voxels, std::vector<float>& offset,
                      std::vector<Uint>& boxSize, bool moveable=false );

        friend class Model;
    private:
        void createVoxelImage ( std::list<Voxel>& voxels, bool moveable );
        void createVBO ();

        GLuint vbo_, vao_, texID_;
        Uint vertexCount_;
        std::vector<float> offset_;
        std::vector<Uint> boxSize_;
        std::vector<glm::mat4> frames_;
        std::vector<glm::mat4>::iterator frameIt_;

        std::chrono::system_clock::duration animDuration_;
        std::chrono::system_clock::time_point animBegin_;
        bool loop_;
        bool animating_;
}; // -----  end of class Model::Limb  -----

struct Model::Voxel {
    Byte x, y, z;
    glm::vec4 rgba;
};

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  setAnimation
// Description:  
//-----------------------------------------------------------------------------
template<class Func>
    void
Model::setAnimation ( Func f, Uint duration, Uint limb, bool loop )
{
    Limb& l = limbList_.at(limb);
    // Need error checking for f returning the correct type.
    for ( Uint t = 0; t <= duration; t += 50 ) {
        glm::mat4 prev = glm::mat4(1.0f);
        if ( !l.frames_.empty() ) prev = l.frames_.back();
        l.frames_.push_back(f(prev));
    }
    l.animDuration_ += std::chrono::milliseconds(duration);
    l.loop_ = loop;
    l.frameIt_ = l.frames_.begin();
}		// -----  end of method Model::setAnimation  -----

    inline void
Model::startAnimation ( Uint limb )
{
    Limb& l = limbList_.at(limb);
    l.animBegin_ = std::chrono::system_clock::now();
    l.animating_ = true;
}		// -----  end of method Model::startAnimation  -----

#endif   // ----- #ifndef MODEL_H  -----
