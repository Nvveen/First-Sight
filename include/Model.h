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

#include    <string>
#include    <vector>
#include    <map>
#include    <GL/gl.h>

// ============================================================================
//        Class:  Model
//  Description:  Model class for importing model information like vertices,
//                normals, textures and the like.
// ============================================================================
class Model
{
    public:

        // ====================  LIFECYCLE     ================================
        Model ();
        Model ( std::string fileName );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

        friend class Object;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        struct Face {
            std::vector<GLuint> vertexIndices;
            std::vector<GLuint> textureIndices;
            std::vector<GLuint> normalIndices;
        };

        struct {
            std::vector<std::vector<GLfloat> > vertices;
            std::vector<std::vector<GLfloat> > textureCoords;
            std::vector<std::vector<GLfloat> > normals;
            std::vector<Face> faces;
        } data_;

        // ====================  LIFECYCLE     ================================
        void init ();
        void parseFace ( Face* face, std::string line );
        void splitIdentifier ( std::string& id, std::string& line );
        template<class T>
        std::vector<T> tokenizeLine ( std::string line );
        void constructVertices ();

        // ====================  DATA MEMBERS  ================================
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> textureCoords;
        std::vector<GLfloat> normals;

        std::string fileName_;
        std::string textureFileName;


}; // -----  end of class Model  -----

#endif   // ----- #ifndef MODEL_H  -----
