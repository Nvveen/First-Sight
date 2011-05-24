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
Model::Model ( std::string fileName ) :
    fileName_(fileName)
{
    init();
    constructVertices();
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  init
// Description:  Initializes the model by reading in the data file.
//-----------------------------------------------------------------------------
    void
Model::init ()
{
    // Open file
    std::ifstream file(fileName_);
    std::string line;
    // If file is opened, iterate through every line.
    if ( file.is_open() ) {
        std::string line, identifier;
        while ( file.good() ) {
            getline(file, line);
            // We need to split each line from its identifier.
            splitIdentifier(identifier, line);

            if ( line.size() == 0 ) return;

            // For the vertex data, we need to tokenize the lines.
            std::vector<GLfloat> tokens;
            tokens = tokenizeLine<GLfloat>(line);
            if ( identifier == "v" ) data_.vertices.push_back(tokens);
            if ( identifier == "vt" ) data_.textureCoords.push_back(tokens);
            if ( identifier == "vn" ) data_.normals.push_back(tokens);
            // The next identifier specifies the location of the texture.
            if ( identifier == "usemtl" ) textureFileName = line;
            // Faces are composed of the vertices. Using faces, we can
            // determine the triangles.
            if ( identifier == "f" ) {
                Face *newFace = new Face;
                parseFace(newFace, line);
                if ( newFace->vertexIndices.size() > 3 ||
                     newFace->textureIndices.size() > 3 ||
                     newFace->normalIndices.size() > 3 ) {
                    std::cerr << "The imported model failed to render "
                                 "properly, because one of its faces has more "
                                 "than 3 vertices/texture coordinates/normals."
                                 "\n";
                    exit(1);
                }
                data_.faces.push_back(*newFace);
                delete newFace;
            }
        }
    }
}		// -----  end of method Model::parse_obj  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  parseFace
// Description:  Parses the line for the face identifier.
//-----------------------------------------------------------------------------
    void
Model::parseFace ( Face* face, std::string line )
{
    // Each face should have three numbers, for vertex, texture and normal
    // data.
    std::vector<GLuint> v,t,n;
    std::vector<std::string> words;

    // We need to iterate through a line, so specify it as a stream, so we can
    // easily use delimiters to tokenize the line.
    std::stringstream linestream(line);
    std::string word;
    // Each group of VTN numbers is seperated by a space, so get these groups.
    while ( getline(linestream, word, ' ') ) {
        // Next, we do the same for each group of VTN numbers, called a word.
        std::stringstream wordStream(word);
        std::string numberString;
        int i = 0;
        // Each number in a word is delimited by a slash, so tokenize from that.
        while ( getline(wordStream, numberString, '/') ) {
            std::stringstream numberStream(numberString);
            // Convert each numberstring to a number.
            GLuint number;
            numberStream >> number;
            // Insert new numbers into the face data.
            if ( i == 0 ) v.push_back(number);
            else if ( i == 1 ) t.push_back(number);
            else if ( i == 2 ) n.push_back(number);
            i += 1;
        }
    }
    face->vertexIndices = v;
    face->textureIndices = t;
    face->normalIndices = n;
}		// -----  end of method Model::parseFace  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  splitIdentifier
// Description:  Splits any line from its identifier. We do this by finding the
//               location of the first space. Anything before that is the
//               identifier. After that location is found, the delimiter and
//               restlines are set.
//-----------------------------------------------------------------------------
    void
Model::splitIdentifier ( std::string& id, std::string& line )
{
    std::stringstream lineStream(line);
    std::string temp;
    size_t found;
    found = line.find_first_of(' ');
    id = line.substr(0, found);
    temp = line.substr(found+1, line.size()-found);
    line = temp;
}		// -----  end of method Model::splitIdentifier  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  tokenizeLine
// Description:  Splits a line into seperate floating point components.
//----------------------------------------------------------------------------- 
template<class T> std::vector<T>
Model::tokenizeLine ( std::string line )
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
}		// -----  end of method Model::tokenizeLine  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  constructVertices
// Description:  Takes all the info gathered from the files, and construct the
//               4 arrays, vertices, colors, texture coordinates and normals,
//               without using an index buffer (for ease of use).
//-----------------------------------------------------------------------------
    void
Model::constructVertices ()
{
    for ( unsigned int i = 0; i < data_.faces.size(); i += 1 ) {
        for ( unsigned int j = 0; j < 3; j += 1 ) {
            GLfloat index;
            if ( !data_.vertices.empty() ) {
                index = data_.faces[i].vertexIndices[j]-1;
                for ( unsigned int k = 0; k < data_.vertices[index].size(); 
                        k += 1 )
                    vertices.push_back(data_.vertices[index][k]);
            }
            if ( !data_.textureCoords.empty() ) {
                index = data_.faces[i].textureIndices[j]-1;
                for ( unsigned int k = 0; 
                        k < data_.textureCoords[index].size(); k += 1 )
                    textureCoords.push_back(data_.textureCoords[index][k]);
            }
            if ( !data_.normals.empty() ) {
                index = data_.faces[i].normalIndices[j]-1;
                for ( unsigned int k = 0; k < data_.normals[index].size(); 
                        k += 1 )
                    normals.push_back(data_.normals[index][k]);
            }
            else {
                index = data_.faces[i].vertexIndices[j]-1;
                for ( unsigned int k = 0; k < data_.vertices[index].size(); 
                        k += 1 )
                    normals.push_back(1.0f);
            }
        }
    }
}		// -----  end of method Model::constructVertices  -----

