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
//       Filename:  Text.h
// 
//    Description:  Class that generates an object for text displaying.
// 
//        Version:  1.0
//        Created:  05/07/2011 09:28:19 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  TEXT_H
#define  TEXT_H

#include    <string>
#include    "Object.h"
#include    "Font.h"

// ============================================================================
//        Class:  Text
//  Description:  Class that is derived from an object and is displayed as 
//                a 2D surface for projection the text texture on it.
// ============================================================================
class Text : public Object
{
    public:

        // ====================  LIFECYCLE     ================================
        Text ( std::string textString, int x, int y, Font& font );
        virtual void draw ();
        virtual void setUniforms ();

        // ====================  ACCESSORS     ================================
        void setText ( std::string textString );

        // ====================  MUTATORS      ================================
        void setBGColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        std::string textString_;
        int x_;
        int y_;

        Font *font_;
        glm::vec4 bgColor_;

        void initVertexBuffer ();
        void createLetterQuad ( char c, float x, float y );
}; // -----  end of class Text  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  setText
// Description:  Sets the interal text string.
//-----------------------------------------------------------------------------
    inline void
Text::setText ( std::string textString )
{
    textString_ = textString;
    modelData_.clear();
    initVertexBuffer();
}		// -----  end of method Text::setText  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  setBGColor
// Description:  Sets the object background color.
//-----------------------------------------------------------------------------
    inline void
Text::setBGColor ( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    bgColor_ = glm::vec4(r, g, b, a);
}		// -----  end of method Text::setBGColor  -----

#endif   // ----- #ifndef TEXT_H  -----
