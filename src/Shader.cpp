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
//       Filename:  Shader.cpp
// 
//    Description:  A class that handles shader creation.
// 
//        Version:  1.0
//        Created:  04/07/2011 02:20:45 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <fstream>
#include    <sstream>
#include    <cstdlib>
#include    <glm/gtc/type_ptr.hpp>
#include    "Shader.h"

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  Shader
// Description:  constructor
//-----------------------------------------------------------------------------
Shader::Shader () : bound_(false)
{
    // Create the shader program
    shaderProgram_ = 0;
}  // -----  end of method Shader::Shader  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  ~Shader
// Description:  destructor
//-----------------------------------------------------------------------------
Shader::~Shader ()
{
    if ( !shaderCodes_.empty() ) {
        for ( auto it : shaderObjects_ )
            glDeleteShader(it.second);
        glDeleteProgram(shaderProgram_);
    }
}  // -----  end of method Shader::~Shader  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  add
// Description:  Add the shader object by compiling the source.
//-----------------------------------------------------------------------------
    void
Shader::add ( std::string fileName, GLenum type )
{
    if ( shaderProgram_ == 0 ) shaderProgram_ = glCreateProgram();
    if ( shaderCodes_.find(type) != shaderCodes_.end() ) {
        std::cerr << "The shader program already has an object of this type.\n";
        exit(1);
    }
    std::string code = addCode(fileName);
    shaderCodes_.insert(std::pair<GLenum, std::string>(type, code));
    GLuint object = compileShader(code, type);
    shaderObjects_.insert(std::pair<GLenum, GLuint>(type, object));
    // Bind the shader to the program.
    glAttachShader(shaderProgram_, object);
}		// -----  end of method Shader::add  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  addCode
// Description:  Find the file, reads it in and returns the code as a string.
//-----------------------------------------------------------------------------
    std::string
Shader::addCode ( std::string fileName )
{
    std::stringstream shaderCode(std::ios::out);
    std::ifstream file;
    file.open(fileName.c_str(), std::ios::in);
    if ( !file.good() ) {
        std::cerr << "Error opening " << fileName << "\n";
        exit(1);
    }
    shaderCode << file.rdbuf();
    return shaderCode.str();
}		// -----  end of method Shader::addCode  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  link
// Description:  Link the shader program to all specified objects.
//-----------------------------------------------------------------------------
    void
Shader::link ()
{
    GLint success;
    // Link program.
    glLinkProgram(shaderProgram_);
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    // Error check for linking
    if ( success == GL_FALSE ) {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderProgram_, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error linking shader program: " << errorLog << "\n";
        exit(1);
    }

    glValidateProgram(shaderProgram_);
    glGetProgramiv(shaderProgram_, GL_VALIDATE_STATUS, &success);
    // Error check for validation
    if ( success == GL_FALSE ) {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderProgram_, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Invalid shader program: " << errorLog << "\n";
        exit(1);
    }
    bound_ = true;
    unbind();
}		// -----  end of method Shader::link  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  compileShader
// Description:  Compiles the shader by the type specified as a parameter.
//-----------------------------------------------------------------------------
    GLuint
Shader::compileShader ( std::string code, GLenum type )
{
    GLuint shader = glCreateShader(type);
    // Error checking
    if ( shader == 0 ) {
        std::cerr << "Error creating shader type " << type << "\n";
        exit(0);
    }

    // Convert string to const char.
    const GLchar *sStringPtr[1];
    sStringPtr[0] = code.c_str();
    // Pass code to shader
    glShaderSource(shader, 1, (const GLchar **)sStringPtr, NULL);
    GLenum error = glGetError();

    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    // Error checking for compilation.
    if ( success == GL_FALSE ) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "Error compiling shader " << type << ": ";
        std::cerr << infoLog << " in shader " << shaderProgram_ << "\n";
        exit(1);
    }
    return shader;
}		// -----  end of method Shader::compileShader  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, GLfloat val )
{
    if ( shaderProgram_ && bound_ ) {
        glUniform1i(uniformLocs_[name], val);
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::vec3 vec )
{
    if ( shaderProgram_ && bound_ ) {
        glUniform3f(uniformLocs_[name], vec.x, vec.y, vec.z);
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::vec4 vec )
{
    if ( shaderProgram_ && bound_ ) {
        glUniform4f(uniformLocs_[name], vec.x, vec.y, vec.z, vec.w);
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::mat4 matrix )
{
    if ( shaderProgram_ && bound_ ) {
        glUniformMatrix4fv(uniformLocs_[name], 1, GL_FALSE, 
                           glm::value_ptr(matrix));
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

