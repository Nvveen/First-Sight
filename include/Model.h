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
        void setAnimation ( Func f, Uint duration, Uint limb, Uint animID );
        void startAnimation ( short animID, bool loop=false );
    private:
        struct Voxel;
        class Limb;
        void init ();
        std::string fileName_;
        std::vector<Limb> limbList_;

        short animID_;
        bool animLoop_;
        std::vector<std::chrono::system_clock::duration> animDurations_;
        std::chrono::system_clock::time_point animBegin_;
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
        std::vector<std::vector<glm::mat4>> anims_;
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
Model::setAnimation ( Func f, Uint duration, Uint limb, Uint animID )
{
    for ( Limb& l : limbList_ ) {
        assert(animID <= l.anims_.size());
        if ( animID == l.anims_.size() ) {
            // Push new animation
            l.anims_.push_back(std::vector<glm::mat4>(1, glm::mat4(1.0f)));
        }
    }
    assert(animID <= animDurations_.size());
    if ( animID == animDurations_.size() )
        animDurations_.push_back(std::chrono::system_clock::duration(0));
    *(animDurations_.begin()+animID) +=
        std::chrono::milliseconds(duration);
    Limb& l = limbList_.at(limb);
    auto animIt = l.anims_.begin()+animID;
    for ( Uint t = 0; t <= duration; t += 50 )
        (*animIt).push_back(f((*animIt).back()));
}		// -----  end of method Model::setAnimation  -----

    inline void
Model::startAnimation ( short animID, bool loop )
{
    animLoop_ = loop;
    animID_ = animID;
    animBegin_ = std::chrono::system_clock::now();
}		// -----  end of method Model::startAnimation  -----

#endif   // ----- #ifndef MODEL_H  -----
