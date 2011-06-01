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
#include    <sstream>
#include    <GL/gl.h>
#include    <glm/glm.hpp>
#include    "Octree.h"

// ============================================================================
//        Class:  Model
//  Description:  
// ============================================================================
class Model
{
    public:
        // ====================  LIFECYCLE     ================================
        Model ( const std::string& fileName );

        // ====================  ACCESSORS     ================================
        std::vector<GLfloat> getVertexData ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        class Voxel;
        // ====================  LIFECYCLE     ================================
        void init ();
        std::vector<GLfloat> read ();
        std::vector<Voxel> constructVoxels 
            ( const std::vector<GLfloat>& voxelData );
        void fillOctree ( const std::vector<Voxel>& voxels );
        // ====================  DATA MEMBERS  ================================
        int size_;
        std::string fileName_;
        Octree<Voxel> *volData_;

}; // -----  end of class Model  -----

// ============================================================================
//        Class:  Voxel
//  Description:  
// ============================================================================
class Model::Voxel
{
    public:
        typedef unsigned char Uint8;
        // ====================  LIFECYCLE     ================================
        Voxel ( glm::vec4 rgba=glm::vec4(1.0f), unsigned int x=0, 
                unsigned int y=0, unsigned int z=0 );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

        friend class Model;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  LIFECYCLE     ================================
        void init ();
        // ====================  DATA MEMBERS  ================================
        unsigned int x_;
        unsigned int y_;
        unsigned int z_;
        glm::vec4 rgba_;
        std::vector<GLfloat> vertices_;

}; // -----  end of class Voxel  -----

#endif   // ----- #ifndef MODEL_H  -----
