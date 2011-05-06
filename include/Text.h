// ============================================================================
// 
//       Filename:  Text.h
// 
//    Description:  Class for displaying text in the window.
// 
//        Version:  1.0
//        Created:  05/06/2011 11:30:23 AM
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
#include    <GL/glew.h>
#include    <SDL/SDL.h>
#include    <SDL/SDL_ttf.h>

#include    "Shader.h"
#include    "Camera.h"

// ============================================================================
//        Class:  Text
//  Description:  This class implements the rendering of text on the screen.
// ============================================================================
class Text
{
    public:

        // ====================  LIFECYCLE     ================================
        Text ( std::string string, std::string fontName, unsigned char red=255, 
               unsigned char green=255, unsigned char blue=255, Shader shader=
               Shader("shaders/default.vs", "shaders/default.fs"));
        ~Text ();
        void init ();
        bool loadFont ( std::string font );
        void render ();
        void bind ( Camera* cam );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        std::string string_;
        TTF_Font *font_;
        SDL_Color color_;
        SDL_Surface* text_;

        Camera* cam_;
        Shader shader_;

        GLfloat w_;
        GLfloat h_;
        GLfloat x_;
        GLfloat y_;

        GLuint vbo_;
        GLuint texID_;

}; // -----  end of class Text  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  bind
// Description:  Binds the camera matrix to the text.
//-----------------------------------------------------------------------------
    inline void
Text::bind ( Camera* cam )
{
    cam_ = cam;
}		// -----  end of method Text::bind  -----

#endif   // ----- #ifndef TEXT_H  -----
