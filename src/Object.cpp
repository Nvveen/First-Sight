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

#include    <glm/gtc/matrix_transform.hpp>
#include    <glm/gtc/type_ptr.hpp>
#include    <GL/glew.h>
#include    "Undat.h"
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
    shader_(&shader)
{
    translation = glm::mat4(1.0f);
    rotation = glm::mat4(1.0f);
    scaling = glm::mat4(1.0f);
    texture_ = NULL;
    this->readDat(datName);
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
Object::readDat ( std::string datName )
{
    ModelData mod(datName);
    model_ = new Model(mod.getData(MD_OBJ), mod.getData(MD_MTL));
    texture_ = new Texture(GL_TEXTURE_2D, mod.getImage());
}		// -----  end of method Object::readDat  -----

//-----------------------------------------------------------------------------
//       Class:  Objectmodel->vertices.size()
//      Method:  init
// Description:  Initializes all the buffers and binds them to a vao.
//-----------------------------------------------------------------------------
    void
Object::init ()
{
    // Generate VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Load modeldata into an array so it can be loaded into a buffer
    std::vector<GLfloat> modelData;
    for ( unsigned int i = 0; i < model_->vertices.size()/3; i += 1 ) {
        modelData.push_back(model_->vertices[i*3]);
        modelData.push_back(model_->vertices[i*3+1]);
        modelData.push_back(model_->vertices[i*3+2]);
        modelData.push_back(model_->textureCoords[i*2]);
        modelData.push_back(model_->textureCoords[i*2+1]);
        modelData.push_back(model_->normals[i*3]);
        modelData.push_back(model_->normals[i*3+1]);
        modelData.push_back(model_->normals[i*3+2]);
    }
    // We need to know how many triangles are contained in the object
    triangleCount_ = modelData.size()/8;

    // Generate a buffer, and fill it with the data.
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*modelData.size(), 
            &modelData[0], GL_STATIC_DRAW);
    // The next 3 calls describe the layout of the data in the array.
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
    shader_->bind();
    // Bind the object's vertex array object
    glBindVertexArray(vao_);
    // Enable the arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Activate the texture
    texture_->bind();

    // Set the uniform variables in the shader
    shader_->setUniform("vProjection", proj_->getPerspective());
    shader_->setUniform("vCamera", proj_->getCamera());
    shader_->setUniform("vTranslate", translation);
    shader_->setUniform("vRotate", rotation);
    shader_->setUniform("vScale", scaling);
    shader_->setUniform("gSampler", 0);

    // Use the vao to draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, triangleCount_);

    // Unbind the texture
    texture_->unbind();
    // Disable the arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    // Unbind the vao
    glBindVertexArray(0);
    // Unbind the shader
    shader_->unbind();
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

