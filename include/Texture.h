// ============================================================================
// 
//       Filename:  Texture.h
// 
//    Description:  A class that makes a texture object and binds it.
// 
//        Version:  1.0
//        Created:  04/12/2011 04:53:58 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  TEXTURE_H
#define  TEXTURE_H

#include    <string>
#include    <vector>
#include    <GL/gl.h>
#include    <png++/png.hpp>

// ============================================================================
//        Class:  Texture
//  Description:  Define a texture, set its data and bind it to a buffer, to be
//                be used by a Model.
// ============================================================================
class Texture
{
    public:

        // ====================  LIFECYCLE     ================================
        Texture ();
        Texture ( GLenum textureTarget, 
                  const png::image<png::rgba_pixel>& png );
        ~Texture ();
        void load ();
        void bind ();
        void unbind ();

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================
        friend class Object;

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        GLenum textureTarget_;
        int width_;
        int height_;
        png::image<png::rgba_pixel> png_;
        GLuint texID;

        bool textureSet;

}; // -----  end of class Texture  -----

#endif   // ----- #ifndef TEXTURE_H  -----
