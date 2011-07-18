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
    float angle = -11.5f;
    glm::vec3 limbPos = glm::vec3(13, 9, 18);
    auto rotate = [&](glm::mat4 mat)->glm::mat4 {
        glm::mat4 m = glm::translate(glm::mat4(1.0f), limbPos);
        m *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1, 0, 0));
        m *= glm::translate(glm::mat4(1.0f), -limbPos);
        m *= mat;
        return m;
    };
    model_->setAnimation(rotate, 200, 1, 0);
    angle = 11.5f;
    model_->setAnimation(rotate, 200, 1, 0);

    limbPos = glm::vec3(13, 9, 11);
    angle = 11.5f;
    model_->setAnimation(rotate, 200, 2, 0);
    angle = -11.5f;
    model_->setAnimation(rotate, 200, 2, 0);
    if ( shader_ == NULL ) shader_ = Context::shaders["default"];
}		// -----  end of method Object::init  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  draw
// Description:  Draw the object in OpenGL.
//-----------------------------------------------------------------------------
    void
Object::draw ()
{
    static int i = 0;
    if ( i == 75 ) {
        model_->startAnimation(0, true);
    }
    i += 1;
    shader_->bind();
    setMVP();
    shader_->setUniform("vMVP", mvp_);
    model_->draw(*shader_);
    shader_->unbind();
}		// -----  end of method Object::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Object
//      Method:  setUniforms
// Description:  Sets the object's uniforms in the shader.
//-----------------------------------------------------------------------------
    void
Object::setMVP ( bool force )
{
    if ( camera_->changed() || force ) {
        mvp_ = projection_->getMatrix() * camera_->getCamera() * translation_ *
               rotation_ * scaling_;
        camera_->clearUpdate();
    }
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
    setMVP(true);
}		// -----  end of method Object::translate  -----

