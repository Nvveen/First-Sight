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
#include    <sstream>
#include    <fstream>
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
        void createVertexData ( std::vector<GLfloat>& data,
                                std::vector<unsigned int>& indices );
        int size ();
        GLuint getTextureID ();
        int getVoxelSize ();

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        class Voxel;
        // ====================  LIFECYCLE     ================================
        void init ();
        void genVoxelField ();
        // ====================  DATA MEMBERS  ================================
        int size_;
        std::string fileName_;
        Octree<Voxel> *volData_;
        std::list<Voxel *> voxelList_;

        GLuint texID_;

}; // -----  end of class Model  -----

// ============================================================================
//        Class:  Voxel
//  Description:  
// ============================================================================
class Model::Voxel
{
    public:
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
        // ====================  DATA MEMBERS  ================================
        unsigned int x_;
        unsigned int y_;
        unsigned int z_;
        glm::vec4 rgba_;

}; // -----  end of class Voxel  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  size
// Description:  Return the size of the model.
//-----------------------------------------------------------------------------
    inline int
Model::size ()
{
    return size_;
}		// -----  end of method Model::size  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  getTextureID
// Description:  Returns the ID of the voxel data map.
//-----------------------------------------------------------------------------
    inline GLuint
Model::getTextureID ()
{
    return texID_;
}		// -----  end of method Model::getTextureID  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  getVoxelSize
// Description:  Return the actual list size of the active voxels.
//-----------------------------------------------------------------------------
    inline int
Model::getVoxelSize ()
{
    return voxelList_.size();
}		// -----  end of method Model::getVoxelSize  -----

#endif   // ----- #ifndef MODEL_H  -----
