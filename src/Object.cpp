// This file is part of First Sight.
// 
// First Sight is free software: you can redistribute it and/or modify it under 
// the terms of the GNU General Public License as published by the 
// Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// First Sight is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
// more details.
// 
// You should have received a copy of the GNU General Public License along with 
// First Sight. If not, see <http://www.gnu.org/licenses/>.
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
#include    <GL/glew.h>
#include    <glm/gtc/matrix_transform.hpp>
#include    "Object.h"

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ( std::string fileName, float x, float y, float z,
                 Context *context, Shader *shader ) :
    fileName_(fileName), model_(NULL), x_(x), y_(y), z_(z), shader_(shader)
{
    translation_ = glm::mat4(1.0f);
    rotation_ = glm::mat4(1.0f);
    scaling_ = glm::mat4(1.0f);
    bind(context->getCamera());
    bind(context->getPerspective());
    init();
    initVertexBuffers();
    initUniformBuffers();
    translate(x, y, z);
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
//      Method:  init
// Description:  Initializes the Object.
//-----------------------------------------------------------------------------
    void
Object::init ()
{
    model_ = new Model(fileName_);
    if ( shader_ == NULL ) shader_ = &Context::shaders["default"];
}		// -----  end of method Object::init  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  initVertexBuffers
// Description:  Initializes the vertex buffers.
//-----------------------------------------------------------------------------
    void
Object::initVertexBuffers ()
{
    std::vector<GLfloat> vertexData;
    model_->createVertexData(vertexData, indices_);
    itemCount_ = vertexData.size()/3;

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexData.size(),
                 &vertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_.size(),
                 &indices_[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);
    glBindAttribLocation(shader_->getShaderProgram(), 0, "vVertex");
    glBindFragDataLocation(shader_->getShaderProgram(), 0, "fragColor");

    // Clean up
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}		// -----  end of method Object::initVertexBuffers  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  initUniformBuffers
// Description:  Initializes the uniform buffers.
//-----------------------------------------------------------------------------
    void
Object::initUniformBuffers ()
{
    // Create two uniform buffers and fill it with data.
    projectionUBO_ = shader_->createUBO("Projection");
    shader_->fillUniformBuffer(projectionUBO_, "vProjection", 
                               projection_->getMatrix());
    shader_->fillUniformBuffer(projectionUBO_, "vCamera", camera_->getCamera());
    shader_->bindUBO(projectionUBO_, "Projection");

    modelUBO_ = shader_->createUBO("Model");
    shader_->fillUniformBuffer(modelUBO_, "vTranslate", translation_);
    shader_->fillUniformBuffer(modelUBO_, "vRotate", rotation_);
    shader_->fillUniformBuffer(modelUBO_, "vScale", scaling_);
    shader_->bindUBO(modelUBO_, "Model");
}		// -----  end of method Object::initUniformBuffers  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  draw
// Description:  Draw the object in OpenGL.
//-----------------------------------------------------------------------------
    void
Object::draw ()
{
    // Bind array.
    shader_->bind();
    // Bind vertex array and its attributes.
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);

    glBindTexture(GL_TEXTURE_2D, model_->getTextureID());

    setUniforms();
    // Draw the vertices.
//    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, itemCount_, model_->getVoxelSize());
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, indices_.size(), GL_UNSIGNED_INT, 
                            0, model_->getVoxelSize());

    // Clean up
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader_->unbind();
}		// -----  end of method Object::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  setUniforms
// Description:  Sets the object's uniforms in the shader.
//-----------------------------------------------------------------------------
    void
Object::setUniforms ()
{
    // Fill all the uniforms in the buffer.
    shader_->bindUBO(modelUBO_, "Model");
    shader_->fillUniformBuffer(modelUBO_, "vTranslate", translation_);
    shader_->fillUniformBuffer(modelUBO_, "vRotate", rotation_);
    shader_->fillUniformBuffer(modelUBO_, "vScale", scaling_);
    shader_->bindUBO(projectionUBO_, "Projection");
    shader_->fillUniformBuffer(projectionUBO_, "vProjection", 
                               projection_->getMatrix());
    shader_->fillUniformBuffer(projectionUBO_, "vCamera", camera_->getCamera());
    shader_->setUniform("gSampler", 0);
}		// -----  end of method Object::setUniforms  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  translate
// Description:  Move the object by (x,y,z).
//-----------------------------------------------------------------------------
    void
Object::translate ( GLfloat x, GLfloat y, GLfloat z )
{
    translation_ = glm::translate(glm::mat4(1.0f), glm::vec3(x*32, y*32, z*32));
    shader_->bindUBO(modelUBO_, "Model");
    shader_->fillUniformBuffer(modelUBO_, "vTranslate", translation_);
}		// -----  end of method Object::translate  -----

