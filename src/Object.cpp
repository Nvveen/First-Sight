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

#include    <glm/gtc/matrix_transform.hpp>
#include    <GL/glew.h>
#include    "Undat.h"
#include    "Object.h"

std::map<std::string, Object> Object::objectCache;

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ( std::string datName, Uint8 x, Uint8 y, Uint8 z, 
                 Shader *shader ) :
    shader_(shader)
{
    if ( shader_ == NULL ) shader_ = &Context::shaders["default"];

    projection_ = NULL;
    camera_ = NULL;
    model_ = NULL;
    texture_ = NULL;
    color_ = glm::vec4(1.0f);

    this->readDat(datName);
    // Determine object max dimensions.
    x_ = x;
    y_ = y;
    z_ = z;
    translateGrid(x_, y_, z_);
}  // -----  end of method Object::Object  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ( Uint8 x, Uint8 y, Uint8 z, Shader *shader ) :
    x_(x), y_(y), z_(z), shader_(shader)
{
    if ( shader_ == NULL ) shader_ = &Context::shaders["default"];

    projection_ = NULL;
    camera_ = NULL;
    model_ = NULL;
    texture_ = NULL;
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
    std::map<std::string, Object>::iterator it;
    it = objectCache.find(datName);
    if ( it != objectCache.end() ) {
        *this = it->second;
    }
    else {
        ModelData mod(datName);
        model_ = new Model(mod.getData(MD_OBJ), mod.getData(MD_MTL));
        texture_ = new Texture(GL_TEXTURE_2D, mod.getImage());
        
        // Load modeldata into an array so it can be loaded into a buffer
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
        init ();
        objectCache[datName] = *this;
    }
}		// -----  end of method Object::readDat  -----

//-----------------------------------------------------------------------------
//       Class:  Objectmodel->vertices.size()
//      Method:  init
// Description:  Initializes all the buffers and binds them.
//-----------------------------------------------------------------------------
    void
Object::init ()
{
    // Get the object dimensions
    width_ = getSize(0);
    height_ = getSize(1);
    depth_ = getSize(2);
    // We need to know how many triangles are contained in the object
    triangleCount_ = modelData_.size()/8;

    // Generate a buffer, and fill it with the data.
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*modelData_.size(), 
            &modelData_[0], GL_STATIC_DRAW);
    
    // Clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    // Enable the arrays
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Point the attribute pointers to the data in the array.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)20);
    glBindAttribLocation(shader_->getShaderProgram(), 0, "vVertex");
    glBindAttribLocation(shader_->getShaderProgram(), 1, "TexCoord");

    // Activate the texture
    texture_->bind();
    // Set the program's uniforms.
    setUniforms();

    // Use the vao to draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, triangleCount_);

    // Unbind the texture
    texture_->unbind();
    // Disable the arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Unbind the shader
    shader_->unbind();
}		// -----  end of method Object::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  setUniforms
// Description:  Sets the uniform variables in the shader.
//-----------------------------------------------------------------------------
    void
Object::setUniforms ()
{
    shader_->setUniform("vProjection", projection_->getProjection());
    shader_->setUniform("vCamera", camera_->getCamera());
    shader_->setUniform("vTranslate", translation_);
    shader_->setUniform("vRotate", rotation_);
    shader_->setUniform("vScale", scaling_);
    shader_->setUniform("varyingColor", color_);
    shader_->setUniform("gSampler", 0);
}		// -----  end of method Object::setUniforms  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  getSize
// Description:  Calculates an object's size on the axis specified by k.
//-----------------------------------------------------------------------------
    GLfloat
Object::getSize ( int axis )
{
    if ( axis != 0 && axis != 1 && axis != 2 ) {
        std::cerr << "An invalid axis is specified for the size calculation\n";
        exit(1);
    }
    GLfloat maxWidth = 0;
    GLfloat minWidth = 0;
    for ( unsigned int i = axis; i < modelData_.size(); i += 8 ) {
        if ( modelData_[i] > maxWidth ) maxWidth = modelData_[i];
        if ( modelData_[i] < minWidth ) minWidth = modelData_[i];
    }
    return maxWidth - minWidth;
}		// -----  end of method Object::getSize  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  translate
// Description:  Give a translation matrix with the params and pass it to the
//               shader.
//-----------------------------------------------------------------------------
    void
Object::translate ( GLfloat x, GLfloat y, GLfloat z )
{
    translation_ = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    shader_->setUniform("vTranslate", translation_);
}		// -----  end of method Object::translate  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  translateGrid
// Description:  Give a translation matrix with the params set to the proper
//               ranges, and pass it to the shader.
//-----------------------------------------------------------------------------
    void
Object::translateGrid ( unsigned int x, unsigned int y, unsigned int z )
{
    x_ = x; y_ = y; z_ = z;
    translation_ = glm::translate(glm::mat4(1.0f), 
                                  glm::vec3(2.9 - width_ * x_, 
                                            height_ * y_, 
                                            -2.5 + depth_ * z_ ));
    shader_->setUniform("vTranslate", translation_);
}		// -----  end of method Object::translateGrid  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  rotate
// Description:  Give a rotation matrix with the params and pass it to the
//               shader.
//-----------------------------------------------------------------------------
    void
Object::rotate ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    rotation_ = glm::rotate(glm::mat4(1.0f), toRadians(angle), 
                            glm::vec3(x, y, z));
    shader_->setUniform("vRotate", rotation_);
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
    scaling_ = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
    shader_->setUniform("vScale", scaling_);
}		// -----  end of method Object::scale  -----

