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
#include    <array>
#include    <list>
#include    <chrono>
#include    <fstream>
#include    <GL/glew.h>
#include    <glm/glm.hpp>

#include    "Shader.h"
#include    "Context.h"

// ============================================================================
//        Class:  Model
//  Description:  Base class that provides the necessary function to read
//                animate and render a voxel model from file.
// ============================================================================
class Model
{
    public:
        typedef unsigned char Byte;
        typedef unsigned int Uint;

        Model ();
        Model ( const std::string& fileName, GLfloat x, GLfloat y, GLfloat z,
                Context& context,
                Shader& shader=Context::shaders["default"] );
        void draw ();
        void move ( GLfloat x, GLfloat y, GLfloat z );
        void rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );

        template<class Func>
            void setAnimation ( Func f, Uint nrFrames, Uint limb, Uint animID );
        void startAnimation ( short animID, bool loop=false );
    private:
        void init ();
        void setMVP ( bool force=false );

        // Resident classes that don't need to be exposed to anything other
        // than a Model.
        struct Voxel;
        class Limb;

        // Standard model data.
        std::string fileName_;
        std::vector<Limb> limbList_;
        glm::vec3 location_;
        glm::mat4 translation_, rotation_, scaling_;
        glm::mat4 mvp_;

        // Bound variables needed for rendering.
        Shader *shader_;
        Projection *perspective_;
        Camera *camera_;

        // Variables needed for animation.
        short animID_;
        bool animLoop_;
        std::chrono::system_clock::time_point animBegin_;
        std::chrono::system_clock::duration duration_;
}; // -----  end of class Model  -----

// ============================================================================
//        Class:  Model::Limb
//  Description:  Each Model is divided into Limbs, which generate their own
//                buffers in OpenGL and their transformation matrices used
//                for animating.
// ============================================================================
class Model::Limb
{
    public:
        Limb ();                             // constructor
        Limb ( std::list<Voxel>& voxels, std::array<float, 3>& offset,
                      std::array<Uint, 3>& boxSize, bool moveable=false );
        ~Limb ();

        friend class Model;
    private:
        void createVoxelImage ( std::list<Voxel>& voxels, bool moveable );
        void createVBO ();

        GLuint vbo_, vao_, texID_;
        Uint vertexCount_;
        std::array<float, 3> offset_;
        std::array<Uint, 3> boxSize_;
        std::vector<std::vector<glm::mat4>> anims_;
}; // -----  end of class Model::Limb  -----

// Simple voxel struct that holds the data a voxel needs.
struct Model::Voxel {
    Byte x, y, z;
    glm::vec4 rgba;
};				// ----------  end of struct Model::Voxel  ----------

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  setAnimation
// Description:  Animating works by progressively generating the transformation
//               matrices in a specific timeframe an animation needs. To do
//               this, the limb + its animation ID are looked up.
//-----------------------------------------------------------------------------
template<class Func>
    void
Model::setAnimation ( Func f, Uint nrFrames, Uint limb, Uint animID )
{
    // We first check for every limb if it already has an animation with that
    // ID yet.
    for ( Limb& l : limbList_ ) {
        // The ID can never exceed the number of IDs already stored.
        assert(animID <= l.anims_.size());
        if ( animID == l.anims_.size() )
            // Push new animation
            l.anims_.push_back(std::vector<glm::mat4>(1, glm::mat4(1.0f)));
    }
    Limb& l = limbList_.at(limb);
    auto animIt = l.anims_.begin()+animID;
    for ( Uint t = 0; t < nrFrames; t += 1 )
        (*animIt).push_back(f((*animIt).back()));
}		// -----  end of method Model::setAnimation  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  startAnimation
// Description:  Start the animation.
//-----------------------------------------------------------------------------
    inline void
Model::startAnimation ( short animID, bool loop )
{
    animLoop_ = loop;
    // Set the current animation the model is in to the new ID.
    animID_ = animID;
    // Keep track of when the animation started.
    animBegin_ = std::chrono::system_clock::now();
    // Calculate duration
    Uint maxFrames = 0;
    for ( Limb& l : limbList_ ) {
        auto animIt = l.anims_.begin()+animID;
        maxFrames = std::max(maxFrames, (Uint)(*animIt).size()-1);
    }
    duration_ = maxFrames * std::chrono::milliseconds(50);
}		// -----  end of method Model::startAnimation  -----

#endif   // ----- #ifndef MODEL_H  -----
