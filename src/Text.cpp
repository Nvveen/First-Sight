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
//       Filename:  Text.cpp
// 
//    Description:  Class that generates an object for text displaying.
// 
//        Version:  1.0
//        Created:  05/07/2011 09:27:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <glm/gtc/matrix_transform.hpp>
#include    "Context.h"
#include    "Text.h"

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  Text
// Description:  constructor
//-----------------------------------------------------------------------------
Text::Text ( std::string textString, int x, int y, Font& font ) :
    Object(), textString_(textString), x_(x), y_(y), font_(&font)
{
    bgColor_ = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    init();
    shader_ = &Context::shaders["text"];
}  // -----  end of method Text::Text  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  init
// Description:  Initializes the Text object.
//-----------------------------------------------------------------------------
    void
Text::init ()
{
    // For every character in a string, we need to loop through its letters,
    // generate a quad, and set that quad next to the previous one.
    float k = x_;
    for ( unsigned int i = 0; i < textString_.size(); i += 1 ) {
        // Get current character from string.
        char c = textString_[i];
        // Create quad.
        createLetterQuad(c, k, y_);
        // Get the letters texture coordinate values.
        SDL_Rect *rect = &font_->charMap_[c];
        // Set the location for the next quad.
        k += (float)rect->w;
    }
    // Set the texture that contains the bitmap font from the Font object.
    texture_ = font_->tex_;
    // Do normal object inits.
    this->Object::init();
}		// -----  end of method Text::init  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  createLetterQuad
// Description:  Creates a quad for a letter at position (x,y).
//-----------------------------------------------------------------------------
    void
Text::createLetterQuad ( char c, float x, float y )
{
    // Get the current letter's texture coordinates.
    SDL_Rect *rect = &font_->charMap_[c];
    float cw = rect->w; // Current char width
    float ch = rect->h; // Current char height
    float charPosX = (float)rect->x/(float)font_->w_; // Char pos in font
    float charPosY = (float)rect->y/(float)font_->h_; // Char pos in font
    float cwNorm = cw/(float)font_->w_; // Char width and height normalized to
    float chNorm = ch/(float)font_->h_; // values in an orthogonal projection.
    // Every quad needs 6 vertex specifications (2 triangles). Texture
    // coordinates are specified by getting the characters position in the
    // bitmap, and depending on the position of the tex coord, adding the
    // normalized values of the char's width or height. We don't need normals,
    // so we're just setting that to default values.
    // This means the layout is as follows:
    //
    // vertexX vertexY vertexZ texX texY normalX normalY normalZ
    //
    GLfloat data[48] = {
        x,    y,    1.0f, charPosX,        charPosY,        1.0f, 0.0f, 0.0f,
        x,    y+ch, 1.0f, charPosX,        charPosY+chNorm, 1.0f, 0.0f, 0.0f,
        x+cw, y+ch, 1.0f, charPosX+cwNorm, charPosY+chNorm, 1.0f, 0.0f, 0.0f,
        x,    y,    1.0f, charPosX,        charPosY,        1.0f, 0.0f, 0.0f,
        x+cw, y+ch, 1.0f, charPosX+cwNorm, charPosY+chNorm, 1.0f, 0.0f, 0.0f,
        x+cw, y,    1.0f, charPosX+cwNorm, charPosY,        1.0f, 0.0f, 0.0f
    };
    // When done, push all the vertex data into the model.
    for ( int i = 0; i < 48; i += 1 ) modelData_.push_back(data[i]);
}		// -----  end of method Text::createLetterQuad  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  draw
// Description:  Wrap the object draw method to disable depth testing, since
//               this is an orthographic rendering.
//-----------------------------------------------------------------------------
    void
Text::draw ()
{
    glDisable(GL_DEPTH_TEST);
    this->Object::draw();
    glEnable(GL_DEPTH_TEST);
}		// -----  end of method Text::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  setUniforms
// Description:  Sets the uniforms in the shader.
//-----------------------------------------------------------------------------
    void
Text::setUniforms ()
{
    shader_->setUniform("vProjection", projection_->getProjection());
    shader_->setUniform("vCamera", camera_->getCamera());
    shader_->setUniform("varyingColor", color_);
    shader_->setUniform("backgroundColor", bgColor_);
    shader_->setUniform("gSampler", 0);
}		// -----  end of method Text::setUniforms  -----

