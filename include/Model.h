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

#include    <array>
#include    <vector>
#include    <list>
#include    <chrono>
#include    "Drawable.h"
#include    "Context.h"
#include    "Shader.h"

// ============================================================================
//        Class:  Model
//  Description:  Base class that provides the necessary function to read
//                animate and render a voxel model from file.
// ============================================================================
class Model : public DrawableInterface
{
    private:
        // Easy access typedefs.
        typedef unsigned int Uint;
        typedef unsigned char Byte;
        typedef std::array<float, 3> Vec3;

        class Limb;
        // A limb is constructed from voxels.
        struct Voxel {
            Byte x, y, z;
            glm::vec4 color;
        };

        struct Animation {
            std::map<Limb *, std::vector<glm::mat4>> matrices;
            bool loop;
        };
    public:
        Model ();
        Model ( const std::string& fileName, Context& context,
                const GLfloat& x, const GLfloat& y, const GLfloat& z,
                Shader& shader=Context::shaders["default"] );
        ~Model ();
        virtual void draw ();

        Uint addAnimation ( const bool& loop=false );
        template < typename Function >
            void animateLimb ( const Uint& limbIndex, const Uint& animIndex, 
                               const Uint& length, Function func );
        bool loopAnimation ( const Uint& animIndex );
        void setAnimationLoop ( const Uint& animIndex, const bool& loop );
        void startAnimation ( const Uint& animIndex );
    protected:
    private:
        std::string fileName_;
        std::vector<Limb> limbs_;
        std::vector<Animation> animations_;
        std::chrono::system_clock::time_point animBegin_;
        int animIndex_;

        void readData ();
}; // -----  end of class Model  -----

// ============================================================================
//        Class:  Model::Limb
//  Description:  A limb is an OpenGL rendering object, constructed from a list
//                of voxels.
// ============================================================================
class Model::Limb : public DrawableObject
{
    public:
        Limb () = default;
        Limb ( const std::vector<Voxel>& voxels, const Vec3& offset,
               const Vec3& boxSize );

        virtual void draw ();
        virtual void construct ();
        virtual void adjustBoxSize ( const std::vector<glm::mat4>& matrices );
    protected:
    private:
        Vec3 offset_, boxSize_;
        std::vector<Voxel> voxels_;
        bool constructed_;

        virtual void createVertexBuffer ();
        virtual void createTexture ();
}; // -----  end of class Model::Limb  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  animateLimb
// Description:  Inputs a function to generate a row of matrices to
//               transform a limb.
//-----------------------------------------------------------------------------
    template < typename Function >
void Model::animateLimb ( const Uint& limbIndex, const Uint& animIndex, 
                          const Uint& length, Function func )
{
    try {
        if ( limbIndex >= limbs_.size() ) throw 1;
        if ( animIndex >= animations_.size() ) throw 2;

        // Find the correct vector of matrices.
        std::vector<glm::mat4> *p = 
            &(animations_[animIndex].matrices[&limbs_[limbIndex]]);
        // Number of frames * 50 ms = length
        Uint numAnimations = length/50;
        // Execute function so many times
        for ( Uint i = 0; i < numAnimations; i += 1 ) {
            glm::mat4 newMat = func(*(p->end()-1));
            p->push_back(newMat);
        }
        // Adjust limbbox to account for animation.
        limbs_[limbIndex].adjustBoxSize(*p);
    }
    catch ( int i ) {
        std::cerr << "Invalid index throw for ";
        if ( i == 1 ) std::cerr << "limbIndex.";
        else if ( i == 2 ) std::cerr << "animIndex.";
        std::cerr << std::endl;
        std::cerr << "Doing nothing." << std::endl;
    }
}		// -----  end of method Model<T>::animateLimb  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  loopAnimation
// Description:  Return whether or not the animation is looping.
//-----------------------------------------------------------------------------
    inline bool
Model::loopAnimation ( const Uint& animIndex )
{
    // We need a special try-catch block so users don't input the wrong
    // indices.
    try {
        if ( animIndex >= animations_.size() ) throw animIndex;
        return animations_[animIndex].loop;
    }
    catch ( const Uint& i ) {
        std::cerr << "Invalid index " << i << " for animations in ";
        std::cerr << "loopAnimation." << std::endl << "Returning false.";
        std::cerr << std::endl;
        return false;
    }
}		// -----  end of method Model::loopAnimation  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  setAnimationLoop
// Description:  Set the animation to loop.
//-----------------------------------------------------------------------------
    inline void
Model::setAnimationLoop ( const Uint& animIndex, const bool& loop )
{
    try {
        if ( animIndex >= animations_.size() ) throw animIndex;
        animations_[animIndex].loop = loop;
    }
    catch ( Uint i ) {
        std::cerr << "Invalid index " << i << " for animations in ";
        std::cerr << "setAnimationLoop." << std::endl << "Doing nothing.";
        std::cerr << std::endl;
    }
}		// -----  end of method Model::setAnimationLoop  -----

#endif   // ----- #ifndef MODEL_H  -----
