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
//       Filename:  Shader.h
// 
//    Description:  A class that handles shader creation.
// 
//        Version:  1.0
//        Created:  04/07/2011 02:21:02 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  SHADER_H
#define  SHADER_H

#include    <string>
#include    <GL/gl.h>
#include    <glm/glm.hpp>

// ============================================================================
//        Class:  Shader
//  Description:  Shader class, for compilation, linking and using.
// ============================================================================
class Shader
{
    public:

        // ====================  LIFECYCLE     ================================
        Shader ();
        Shader ( std::string vertexShaderFile, std::string fragmentShaderFile );

        void bind ();
        void unbind ();

        // ====================  ACCESSORS     ================================
        GLuint getShaderProgram();

        // ====================  MUTATORS      ================================
        bool setUniform ( std::string name, glm::mat4 matrix );
        bool setUniform ( std::string name, GLfloat val );

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        std::string vertexShaderCode_;
        std::string fragmentShaderCode_;
        GLuint vertexShaderObject_;
        GLuint fragmentShaderObject_;
        GLuint shaderProgram_;

        std::string addCode ( std::string fileName );
        GLuint compileShader ( std::string code, GLenum type );
        void createProgram ();
}; // -----  end of class Shader  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  getShaderProgram
// Description:  Returns the shader program object.
//-----------------------------------------------------------------------------
    inline GLuint
Shader::getShaderProgram ()
{
    return shaderProgram_;
}		// -----  end of method Shader::getShaderProgram  -----

#endif   // ----- #ifndef SHADER_H  -----
