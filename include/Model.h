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
        Model ( const std::string& obj, const std::string& mtl );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

        friend class Object;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        struct Material {
            std::string m_Name; // Material name
            std::string textureName; // File name
            GLfloat ns;
            std::vector<GLfloat> ambient;
            std::vector<GLfloat> diffuse;
            std::vector<GLfloat> specular;
            GLfloat trans;
            GLfloat illum;
            GLfloat refrac;
        };

        struct Face {
            std::vector<GLuint> vertexIndices;
            std::vector<GLuint> textureIndices;
            std::vector<GLuint> normalIndices;
            Material* mtl;
        };

        struct {
            std::vector<std::vector<GLfloat> > vertices;
            std::vector<std::vector<GLfloat> > textureCoords;
            std::vector<std::vector<GLfloat> > normals;
            std::vector<Face> faces;
            std::map<std::string, Material> materials;
        } m_Data;

        // ====================  LIFECYCLE     ================================
        void parse_obj ( const std::string& obj );
        void parse_mtl ( const std::string& mtl );
        void parse_face ( Face* face, std::string line );
        void split_identifier ( std::string& id, std::string& line );
        template<class T>
        std::vector<T> tokenize_line ( std::string line );
        void construct_vertices ();

        // ====================  DATA MEMBERS  ================================
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> textureCoords;
        std::vector<GLfloat> normals;

        std::string objectName;
        Material* current_mtl;
        Material* textureMtl; // Only material with a texture


}; // -----  end of class Model  -----

#endif   // ----- #ifndef MODEL_H  -----
