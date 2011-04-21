// ============================================================================
// 
//       Filename:  Object.cpp
// 
//    Description:  Base object class.
// 
//        Version:  1.0
//        Created:  04/07/2011 11:44:29 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <fstream>
#include    <sstream>

#include    <glm/gtc/matrix_transform.hpp>
#include    <glm/gtc/type_ptr.hpp>
#include    <GL/glew.h>
#include    <snappy.h>
#include    "Object.h"

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ()
{
}  // -----  end of method Object::Object  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ( std::string datName, Shader& shader ) :
    shader_(&shader), datName_(datName)
{
    translation = glm::mat4(1.0f);
    rotation = glm::mat4(1.0f);
    scaling = glm::mat4(1.0f);
    texture_ = NULL;
    this->readDat();
    this->init();
}  // -----  end of method Object::Object  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  ~Object
// Description:  destructor
//-----------------------------------------------------------------------------
Object::~Object ()
{
}  // -----  end of method Object::~Object  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  readDat
// Description:  Reads the .dat file and attempts to pass the data contained
//               to the appropiate members.
//-----------------------------------------------------------------------------
    void
Object::readDat ()
{
    // Read the file
    std::ifstream ifs(datName_.c_str());
    // Put the file into a stringstream
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string fileString = oss.str();
    size_t fileSize = fileString.size();
    std::string decompressedString;
    // Decompress the file string
    snappy::Uncompress(fileString.c_str(), fileSize, &decompressedString);
    // Put the file string in a stringstream to be read as input
    std::istringstream iss(decompressedString);
    // The seperate files need to be sent as strings from stringstreams
    std::ostringstream obj, mtl, png;
    std::string line;
    while ( iss.good() ) {
        getline(iss, line);
        if ( line == "EOF" ) break;
        obj << line << "\n";
    }
    while ( iss.good() ) {
        getline(iss, line);
        if ( line == "EOF" ) break;
        mtl << line << "\n";
    }
    model_ = new Model(obj.str(), mtl.str());
    while ( iss.good() ) {
        getline(iss, line);
        png << line << "\n";
    }
    // Now we can create a new texture from the texture data sent as a string
    texture_ = new Texture(GL_TEXTURE_2D, png.str());
}		// -----  end of method Object::readDat  -----

//-----------------------------------------------------------------------------
//       Class:  Objectmodel->vertices.size()
//      Method:  init
// Description:  Initializes all the buffers and binds them to a vao.
//-----------------------------------------------------------------------------
    void
Object::init ()
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for ( unsigned int i = 0; i < model_->vertices.size()/3; i += 1 ) {
        modelData_.push_back(model_->vertices[i*3]);
        modelData_.push_back(model_->vertices[i*3+1]);
        modelData_.push_back(model_->vertices[i*3+2]);
        modelData_.push_back(model_->textureCoords[i*2]);
        modelData_.push_back(model_->textureCoords[i*2+1]);
        modelData_.push_back(model_->normals[i*3]);
        modelData_.push_back(model_->normals[i*3+1]);
        modelData_.push_back(model_->normals[i*3+2]);
    }

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*modelData_.size(), 
            &modelData_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)20);

    // Load the texture
    texture_->load();

    // Clean up
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}		// -----  end of method Object::init  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  draw
// Description:  Draws the element from the buffer using the VAO.
//-----------------------------------------------------------------------------
    void
Object::draw ()
{
    // Use the specified program
    glUseProgram(shader_->getShaderProgram());
    // Bind the object's vertex array object
    glBindVertexArray(vao_);
    // Enable the arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    texture_->bind(shader_->getShaderProgram());

    // Locate the 3 matrices and bind them to the shader
    GLint locTranslate = glGetUniformLocation(shader_->getShaderProgram(),
                                                "vTranslate");
    glUniformMatrix4fv(locTranslate, 1, GL_FALSE, glm::value_ptr(translation));
    GLint locRotate = glGetUniformLocation(shader_->getShaderProgram(), 
                                                "vRotate");
    glUniformMatrix4fv(locRotate, 1, GL_FALSE, glm::value_ptr(rotation));
    GLint locScale = glGetUniformLocation(shader_->getShaderProgram(), 
                                                "vScale");
    glUniformMatrix4fv(locScale, 1, GL_FALSE, glm::value_ptr(scaling));

    // Use the vao to draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, modelData_.size()/8);
    
    // Disable the arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    // Unbind the vao
    glBindVertexArray(0);
}		// -----  end of method Object::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  translate
// Description:  Give a translation matrix with the params and pass it to the
//               shader.
//-----------------------------------------------------------------------------
    void
Object::translate ( GLfloat x, GLfloat y, GLfloat z )
{
    glm::vec3 loc(x, y, z);
    translation = glm::translate(glm::mat4(1.0f), loc);
}		// -----  end of method Object::translate  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  rotate
// Description:  Give a rotation matrix with the params and pass it to the
//               shader.
//-----------------------------------------------------------------------------
    void
Object::rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    glm::vec3 axis(x, y, z);
    rotation = glm::rotate(glm::mat4(1.0f), toRadians(angle), axis);
}		// -----  end of method Object::rotate  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  scale
// Description:  Give a scaling matrix with the params and pass it to the 
//               shader.
//-----------------------------------------------------------------------------
    void
Object::scale ( GLfloat x, GLfloat y, GLfloat z )
{
    glm::vec3 scaleVec(x, y, z);
    scaling = glm::scale(glm::mat4(1.0f), scaleVec);
}		// -----  end of method Object::scale  -----

