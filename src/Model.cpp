// ============================================================================
// 
//       Filename:  Model.cpp
// 
//    Description:  A class that imports model data and loads them in a 
//                  usable format.
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

#include    <cstdlib>
#include    <iostream>
#include    <fstream>
#include    <sstream>
#include    "Model.h"

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ()
{
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ( const std::string& obj, const std::string& mtl ) :
    current_mtl(NULL), textureMtl(NULL)
{
    parse_obj(obj);
    parse_mtl(mtl);
    construct_vertices();
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  parse_obj
// Description:  Parses the obj stream.
//-----------------------------------------------------------------------------
    void
Model::parse_obj ( const std::string& obj )
{
    std::istringstream iss(obj);
    std::string line;
    while ( iss.good() ) {
        getline(iss, line);
        std::string identifier;
        split_identifier(identifier, line);
        if ( line.size() == 0 ) {
            return;
        }

        if ( identifier == "o" ) {
            objectName = line;
        }
        else if ( identifier == "v" ) { // specified vertex
            std::vector<GLfloat> vertex = tokenize_line<GLfloat>(line);
            m_Data.vertices.push_back(vertex);
        }
        else if ( identifier == "vt" ) {
            std::vector<GLfloat> textureCoord = tokenize_line<GLfloat>(line);
            m_Data.textureCoords.push_back(textureCoord);
        }
        else if ( identifier == "vn" ) {
            std::vector<GLfloat> normals = tokenize_line<GLfloat>(line);
            m_Data.normals.push_back(normals);
        }
        else if ( identifier == "usemtl" ) { // specified what material to use
            // Since a defined material holds for every face next specified, we
            // use this helper data member.
            current_mtl = &m_Data.materials[line];
        }
        else if ( identifier == "f" ) { // Creating a new face
            Face* newFace = new Face;
            parse_face(newFace, line);
            // Each face can have at most 3 vertices, because we haven't
            // implemented an algorithm that can deduce this from the indexes (yet)
            // Because we don't want to spam the error stream with errors, we use
            // a bool to set the state.
            static bool faceRender = false;
            if ( newFace->vertexIndices.size() > 3 && !faceRender ) {
                std::cerr << "The imported model failed to render properly, ";
                std::cerr << "because one of its faces has more than 3 vertices. ";
                std::cerr << "Undefined rendering will occur.\n";
                faceRender = true;
            }
            newFace->mtl = current_mtl;
            m_Data.faces.push_back(*newFace);
            delete newFace;
        }
    }
}		// -----  end of method Model::parse_obj  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  parse_mtl
// Description:  Parses the mtl stream.
//-----------------------------------------------------------------------------
    void
Model::parse_mtl ( const std::string& mtl )
{
    // The model is now parsing the MTL stream.
    std::istringstream iss(mtl);
    std::string line;
    Material* newMat = NULL;
    while ( iss.good() ) {
        getline(iss, line);
        std::string id;
        split_identifier(id, line);
        if ( id == "newmtl" ) {
            newMat = new Material;
            newMat->m_Name = line;
            while ( line.size() != 0 ) {
                getline(iss, line);
                split_identifier(id, line);
                if ( id == "Ns" ) // Specularity weight
                    newMat->ns = tokenize_line<GLfloat>(line)[0];
                else if ( id == "Ka" ) // Ambient color
                    newMat->ambient = tokenize_line<GLfloat>(line);
                else if ( id == "Kd" ) // Diffuse color
                    newMat->diffuse = tokenize_line<GLfloat>(line);
                else if ( id == "Ks" ) // Specular color
                    newMat->specular = tokenize_line<GLfloat>(line);
                else if ( id == "d" )  // Transparency
                    newMat->trans = (tokenize_line<GLfloat>(line))[0];
                else if ( id == "Ni" )
                    newMat->refrac = (tokenize_line<GLfloat>(line))[0];
                else if ( id == "illum" ) // Illumination mode
                    newMat->illum = (tokenize_line<GLfloat>(line))[0];
                else if ( id == "map_Kd" )
                    newMat->textureName = line;
            }
            m_Data.materials[newMat->m_Name] = *newMat;
            if ( !newMat->textureName.empty() ) {
                if ( textureMtl != NULL ) {
                    std::cerr << "Cannot apply multiple textures to the ";
                    std::cerr << "same object.\n";
                    exit(1);
                }
                textureMtl = &m_Data.materials[newMat->m_Name];
            }
            delete newMat;
        }
    }
}		// -----  end of method Model::parse_mtl  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  parse_face
// Description:  Parses the line for the face identifier.
//-----------------------------------------------------------------------------
    void
Model::parse_face ( Face* face, std::string line )
{
    std::vector<GLuint> v,t,n;
    std::vector<std::string> words;

    std::stringstream linestream(line);
    std::string word;
    while ( getline(linestream, word, ' ') ) {
        std::stringstream wordStream(word);
        std::string numberString;
        int i = 0;
        while ( getline(wordStream, numberString, '/') ) {
            std::stringstream numberStream(numberString);
            GLuint number;
            numberStream >> number;
            if ( i == 0 ) v.push_back(number);
            else if ( i == 1 ) t.push_back(number);
            else if ( i == 2 ) n.push_back(number);
            i += 1;
        }
    }
    face->vertexIndices = v;
    face->textureIndices = t;
    face->normalIndices = n;
}		// -----  end of method Model::parse_face  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  split_identifier
// Description:  Splits any line from its identifier.
//-----------------------------------------------------------------------------
    void
Model::split_identifier ( std::string& id, std::string& line )
{
    // Splits a line into an identifier (the first word or letter), and the
    // remaining string
    std::string::iterator it;
    std::string newId;
    std::string restLine;
    if ( line.size() != 0 ) {
        for ( it = line.begin(); *it != ' '; it += 1 ) newId.push_back(*it);
        restLine = line.substr(newId.size()+1, line.size()-newId.size()+1);
    }
    id = newId;
    line = restLine;
}		// -----  end of method Model::split_identifier  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  tokenize_line
// Description:  Splits a line into seperate floating point components.
//-----------------------------------------------------------------------------
template<class T>
    std::vector<T>
Model::tokenize_line ( std::string line )
{
    // Generate a list to hold the numbers.
    std::vector<T> list;

    std::stringstream wordStream(line);
    std::string numberString;
    while ( getline(wordStream, numberString, ' ') ) {
        std::stringstream numberStream(numberString);
        T number;
        numberStream >> number;
        list.push_back(number);
    }
    return list;
}		// -----  end of method Model::tokenize_line  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  construct_vertices
// Description:  Takes all the info gathered from the files, and construct the
//               4 arrays, vertices, colors, texture coordinates and normals,
//               without using an index buffer (for ease of use).
//-----------------------------------------------------------------------------
    void
Model::construct_vertices ()
{
    for ( unsigned int i = 0; i < m_Data.faces.size(); i += 1 ) {
        for ( unsigned int j = 0; j < 3; j += 1 ) {
            GLfloat index;
            if ( !m_Data.vertices.empty() ) {
                index = m_Data.faces[i].vertexIndices[j]-1;
                for ( unsigned int k = 0; k < m_Data.vertices[index].size(); 
                        k += 1 )
                    vertices.push_back(m_Data.vertices[index][k]);
            }
            if ( !m_Data.textureCoords.empty() ) {
                index = m_Data.faces[i].textureIndices[j]-1;
                for ( unsigned int k = 0; 
                        k < m_Data.textureCoords[index].size(); k += 1 )
                    textureCoords.push_back(m_Data.textureCoords[index][k]);
            }
            if ( !m_Data.normals.empty() ) {
                index = m_Data.faces[i].normalIndices[j]-1;
                for ( unsigned int k = 0; k < m_Data.normals[index].size(); 
                        k += 1 )
                    normals.push_back(m_Data.normals[index][k]);
            }
            else {
                index = m_Data.faces[i].vertexIndices[j]-1;
                for ( unsigned int k = 0; k < m_Data.vertices[index].size(); 
                        k += 1 )
                    normals.push_back(1.0f);
            }
        }
    }
}		// -----  end of method Model::construct_vertices  -----

