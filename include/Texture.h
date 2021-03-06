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
#include    <GL/gl.h>
#include    <SDL/SDL.h>

// ============================================================================
//        Class:  Texture
//  Description:  Define a texture, set its data and bind it to a buffer, to be
//                be used by a Model.
// ============================================================================
class Texture
{
    public:
        Texture ();
        Texture ( GLenum textureTarget, std::string fileName );
        Texture ( GLenum textureTarget, SDL_Surface *surface );
        ~Texture ();
        void bind ();
        void unbind ();

        friend class Object;

    protected:

    private:
        void load ();
        void setFromPNG ( std::string fileName );
        void setFromSDL_Surface ( SDL_Surface *surface );

        // ====================  DATA MEMBERS  ================================
        GLenum textureTarget_;
        int width_;
        int height_;
        GLuint texID;

}; // -----  end of class Texture  -----

#endif   // ----- #ifndef TEXTURE_H  -----
