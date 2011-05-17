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
#include    <glm/gtc/type_ptr.hpp>
#include    <GL/glew.h>
#include    "Undat.h"
#include    "Object.h"

std::map<std::string, Object> Object::objectCache;

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  Object
// Description:  constructor
//-----------------------------------------------------------------------------
Object::Object ( std::string datName, Context& context, 
                 Uint8 x, Uint8 y, Uint8 z, Shader *shader ) :
    shader_(shader)
{
    if ( shader_ == NULL ) shader_ = &Context::shaders["default"];

    projection_ = NULL;
    camera_ = NULL;
    model_ = NULL;
    texture_ = NULL;
    color_ = glm::vec4(1.0f);

    if ( readDat(datName) ) {
        initVertexBuffer();
        objectCache[datName] = *this;
    }
    bind(context, false);
    initUniformBuffer();

    translateGrid(x, y, z);
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
//               to the appropiate members. Returns true if the object hasn't
//               been cached yet.
//-----------------------------------------------------------------------------
    bool
Object::readDat ( std::string datName )
{
    std::map<std::string, Object>::iterator it;
    it = objectCache.find(datName);
    if ( it != objectCache.end() ) {
        *this = it->second;
        return false;
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
        return true;
    }
}		// -----  end of method Object::readDat  -----

//-----------------------------------------------------------------------------
//       Class:  Objectmodel->vertices.size()
//      Method:  initVertexBuffer
// Description:  Initializes the vertex buffer.
//-----------------------------------------------------------------------------
    void
Object::initVertexBuffer ()
{
    // Get the object dimensions
    width_ = getSize(0);
    height_ = getSize(1);
    depth_ = getSize(2);
    // We need to know how many triangles are contained in the object
    triangleCount_ = modelData_.size()/8;

    // Generate a buffer, and fill it with the data.
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*modelData_.size(), 
            &modelData_[0], GL_STATIC_DRAW);
    // Point the attribute pointers to the data in the array.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, 
            (const GLvoid*)20);
    glBindAttribLocation(shader_->getShaderProgram(), 0, "vVertex");
    glBindAttribLocation(shader_->getShaderProgram(), 1, "TexCoord");

    // Clean up
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
}		// -----  end of method Object::initVertexBuffer  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  initUniformBuffer
// Description:  Initializes the uniform buffer.
//-----------------------------------------------------------------------------
    void
Object::initUniformBuffer ()
{
    GLuint prog = shader_->getShaderProgram();
    glGenBuffers(1, &ubo_[0]);
    GLuint uniformBlockIndex = glGetUniformBlockIndex(prog, "Projection");
    GLint uniformBlockSize(0);
    glGetActiveUniformBlockiv(prog, uniformBlockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_[0]);
    glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_[0]);
    glUniformBlockBinding(prog, uniformBlockIndex, 0);

    const GLchar *names[2] = { "vProjection", "vCamera" };
    GLuint indices[2];
    GLint offsets[2];
    glGetUniformIndices(prog, 2, names, indices);
    glGetActiveUniformsiv(prog, 2, indices, GL_UNIFORM_OFFSET, offsets);

    glBufferSubData(GL_UNIFORM_BUFFER, offsets[0], sizeof(glm::mat4),
                    glm::value_ptr(projection_->getProjection()));
    glBufferSubData(GL_UNIFORM_BUFFER, offsets[1], sizeof(glm::mat4),
                    glm::value_ptr(camera_->getCamera()));

    glGenBuffers(1, &ubo_[1]);
    uniformBlockIndex = glGetUniformBlockIndex(prog, "Model");
    glGetActiveUniformBlockiv(prog, uniformBlockIndex,
                              GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_[1]);
    glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_[1]);
    glUniformBlockBinding(prog, uniformBlockIndex, 1);

    const GLchar *names2[3] = { "vTranslate", "vRotate", "vScale" };
    GLuint indices2[3];
    GLint offsets2[3];
    glGetUniformIndices(prog, 3, names2, indices2);
    glGetActiveUniformsiv(prog, 3, indices2, GL_UNIFORM_OFFSET, offsets2);

    glBufferSubData(GL_UNIFORM_BUFFER, offsets2[0], sizeof(glm::mat4),
                    glm::value_ptr(translation_));
    glBufferSubData(GL_UNIFORM_BUFFER, offsets2[1], sizeof(glm::mat4),
                    glm::value_ptr(rotation_));
    glBufferSubData(GL_UNIFORM_BUFFER, offsets2[2], sizeof(glm::mat4),
                    glm::value_ptr(scaling_));
}		// -----  end of method Object::initUniformBuffer  -----

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
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    // Activate the texture
    texture_->bind();

    setUniforms();
    // Use the vao to draw the vertices
    glDrawArrays(GL_TRIANGLES, 0, triangleCount_);
    // Unbind the texture
    texture_->unbind();
    // Disable the arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindVertexArray(0);
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
//    shader_->setUniform("vTranslate", translation_);
//    shader_->setUniform("vRotate", rotation_);
//    shader_->setUniform("vScale", scaling_);
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

