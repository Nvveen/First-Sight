// ============================================================================
// 
//       Filename:  Drawable.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  08/25/2011 01:50:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    "Drawable.h"

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  DrawableInterface
// Description:  constructor
//-----------------------------------------------------------------------------
DrawableInterface::DrawableInterface ()
{
}  // -----  end of method DrawableInterface::DrawableInterface -----

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  DrawableInterface
// Description:  constructor
//-----------------------------------------------------------------------------
DrawableInterface::DrawableInterface ( Context &context, 
                                       Shader &shader ) :
    pers_(&context.getPerspective()), cam_(&context.getCamera()),
    shader_(&shader)
{
    mvp_ = glm::mat4(1.0f);
    translation_ = rotation_ = scaling_ = glm::mat4(1.0f);
}  // -----  end of method DrawableInterface::DrawableInterface -----

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  setMVP
// Description:  Set state matrices.
//-----------------------------------------------------------------------------
    void
DrawableInterface::setMVP ( bool force )
{
    if ( force || cam_->changed() ) {
        mvp_ = translation_ * rotation_ * scaling_ * pers_->getMatrix() *
               cam_->getMatrix();
    }
}		// -----  end of method DrawableInterface::setMVP  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  setDrawState
// Description:  Collector function that groups functionality every draw call
//               needs.
//-----------------------------------------------------------------------------
    void
DrawableInterface::setDrawState ()
{
    // Bind the shader, recalculate the Modelview Projection matrix if
    // necessary and pass the MVP matrix to the GPU.
    shader_->bind();
    setMVP();
    shader_->setUniform("vMVP", mvp_);
}		// -----  end of method DrawableInterface::setDrawState  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  move
// Description:  Move the Drawable by a certain amount on every axis.
//-----------------------------------------------------------------------------
    void
DrawableInterface::move ( const GLfloat& x, const GLfloat& y, const GLfloat& z )
{
    translation_ *= glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    setMVP(true);
}		// -----  end of method DrawableInterface::move  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableInterface
//      Method:  rotate
// Description:  Rotate the Drawable by a certain angle on every axis.
//-----------------------------------------------------------------------------
    void
DrawableInterface::rotate ( const GLfloat& angle,
                            const std::array<float, 3>& axis )
{
    rotation_ *= glm::rotate(glm::mat4(1.0f), angle, glm::vec3(axis[0], axis[1],
                                                               axis[2]));
    setMVP(true);
}		// -----  end of method DrawableInterface::rotate  -----

//-----------------------------------------------------------------------------
//       Class:  Drawable
//      Method:  Drawable
// Description:  constructor
//-----------------------------------------------------------------------------
Drawable::Drawable ()
{
}  // -----  end of method Drawable::Drawable  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableObject
//      Method:  DrawableObject
// Description:  constructor
//-----------------------------------------------------------------------------
DrawableObject::DrawableObject ()
{
    texType_ = GL_TEXTURE_3D; // default
}  // -----  end of method DrawableObject::DrawableObject  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableObject
//      Method:  draw
// Description:  Render the OpenGL object.
//-----------------------------------------------------------------------------
    void
DrawableObject::draw ()
{
    // Bind arrays
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    if ( texType_ == GL_TEXTURE_3D ) glEnableVertexAttribArray(1);
    glBindTexture(texType_, texID_);

    glDrawArrays(GL_TRIANGLES, 0, itemCount_);
}		// -----  end of method DrawableObject::draw  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableObject
//      Method:  createVertexBuffer
// Description:  Create an OpenGL VBO from the data passed as a parameter.
//-----------------------------------------------------------------------------
    void
DrawableObject::createVertexBuffer ( GLfloat *buffer )
{
    // Generate vertex array object
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Vertex buffer
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // Determine what type of texture we have, and depending on that, activate
    // one or two attribute pointers.
    Uint typeMod = sizeof(GLfloat) * (texType_ == GL_TEXTURE_3D ? 6 : 3);
    // Buffer data size = sizeof(GLfloat)*buffer size, and itemCount is
    // buffer size / 6 --> sizeof(GLfloat)*(buffersize/6)*6 = 
    // sizeof(GLfloat)*buffersize = typeMod*itemCount
    glBufferData(GL_ARRAY_BUFFER, typeMod*itemCount_, buffer, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, typeMod, (const GLvoid*)0);

    if ( texType_ == GL_TEXTURE_3D )
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, typeMod,
                              (const GLvoid*)12);

    // Unbind everything
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}		// -----  end of method DrawableObject::createVertexBuffer  -----

//-----------------------------------------------------------------------------
//       Class:  DrawableObject
//      Method:  createTexture
// Description:  Create a texture of a certain size defined in the parameters
//               with the data passed as a parameter.
//-----------------------------------------------------------------------------
    void
DrawableObject::createTexture ( GLfloat *texBuffer, Uint w, Uint h, Uint d )
{
    // Create texture object.
    glGenTextures(1, &texID_);
    glBindTexture(texType_, texID_);
    // Define texture properties
    glTexParameteri(texType_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Define texture type.
    if ( texType_ == GL_TEXTURE_3D )
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h, d, 0, GL_RGBA, GL_FLOAT,
                     texBuffer);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT,
                     texBuffer);
    // Clean up
    glBindTexture(texType_, 0);
}		// -----  end of method DrawableObject::createTexture  -----

//-----------------------------------------------------------------------------
//       Class:  Drawable
//      Method:  draw
// Description:  Collect methods into one draw method.
//-----------------------------------------------------------------------------
    void
Drawable::draw ()
{
    setDrawState();
    DrawableObject::draw();
}		// -----  end of method Drawable::draw  -----

