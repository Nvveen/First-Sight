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
#include    <iostream>
#include    <cstdlib>
#include    <glm/gtc/type_ptr.hpp>
#include    "Shader.h"

std::map<std::string, GLuint> Shader::codeNames_;

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  Shader
// Description:  constructor
//-----------------------------------------------------------------------------
Shader::Shader ()
{
}  // -----  end of method Shader::Shader  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  Shader
// Description:  constructor
//-----------------------------------------------------------------------------
Shader::Shader ( std::string vertexShaderFile, std::string fragmentShaderFile,
                 std::string geoShaderFile )
{
    // Check if the object has already been compiled for the shaders.
    std::map<std::string, GLuint>::iterator it;
    it = codeNames_.find(vertexShaderFile);
    // Vertex shader
    if ( it != codeNames_.end() ) {
        vertexShaderObject_ = (*it).second;
    }
    else {
        vertexShaderCode_ = addCode(vertexShaderFile);
        vertexShaderObject_ = compileShader(vertexShaderCode_, 
                                            GL_VERTEX_SHADER);
        codeNames_[vertexShaderFile] = vertexShaderObject_;
    }
    it = codeNames_.find(fragmentShaderFile);
    // Fragment shader
    if ( it != codeNames_.end() ) {
        fragmentShaderObject_ = (*it).second;
    }
    else {
        fragmentShaderCode_ = addCode(fragmentShaderFile);
        fragmentShaderObject_ = compileShader(fragmentShaderCode_, 
                                             GL_FRAGMENT_SHADER);
        codeNames_[fragmentShaderFile] = fragmentShaderObject_;
    }
    // Optional geometry shader
    if ( geoShaderFile.size() > 0 ) {
        it = codeNames_.find(geoShaderFile);
        if ( it != codeNames_.end() ) {
            geoShaderObject_ = (*it).second;
        }
        else {
            geoShaderCode_ = addCode(geoShaderFile);
            geoShaderObject_ = compileShader(geoShaderCode_, 
                                             GL_GEOMETRY_SHADER);
            codeNames_[geoShaderFile] = geoShaderObject_;
        }
    }
    // Create shader program
    createProgram();
    if ( geoShaderFile.size() > 0 ) {
        // We also need to define the types of input and output the geometry
        // shader has to handle.
        glProgramParameteriARB(shaderProgram_, GL_GEOMETRY_INPUT_TYPE_ARB, 
                               GL_POINTS);
        glProgramParameteriARB(shaderProgram_, GL_GEOMETRY_OUTPUT_TYPE_ARB, 
                               GL_TRIANGLE_STRIP);
        GLint temp;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB, &temp);
        glProgramParameteriARB(shaderProgram_, GL_GEOMETRY_VERTICES_OUT_ARB, 
                               temp);
    }
}  // -----  end of method Shader::Shader  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  ~Shader
// Description:  destructor
//-----------------------------------------------------------------------------
Shader::~Shader ()
{
}  // -----  end of method Shader::~Shader  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::mat4& matrix )
{
    if ( shaderProgram_ ) {
        glUniformMatrix4fv(uniformLocs[name], 1, GL_FALSE, 
                           glm::value_ptr(matrix));
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, GLfloat val )
{
    if ( shaderProgram_ ) {
        glUniform1i(uniformLocs[name], val);
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::vec4& vec )
{
    if ( shaderProgram_ ) {
        glUniform4f(uniformLocs[name], vec.x, vec.y, vec.z, vec.w);
    }
    return true;
}		// -----  end of method Shader::setUniform  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  addCode
// Description:  Find the file, reads it in and returns the code as a string.
//-----------------------------------------------------------------------------
    std::string
Shader::addCode ( std::string fileName )
{
    std::string shaderCode;
    std::ifstream file;
    // Open the file
    file.open(fileName.c_str(), std::ios::in);
    if ( !file.good() ) {
        std::cerr << "Error opening " << fileName << "\n";
        exit(1);
    }
    // Get each character and append to string.
    while ( !file.eof() ) {
        shaderCode.push_back(file.get());
    }
    // The last character of the code needs to be set to a terminating
    // character.
    shaderCode[shaderCode.length()-1] = '\0';
    return shaderCode;
}		// -----  end of method Shader::addCode  -----

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
//      Method:  createProgram
// Description:  Creates a shader program from the specified shaders, by 
//               linking them and then verifying them.
//-----------------------------------------------------------------------------
    void
Shader::createProgram ()
{
    // Create the shader program
    shaderProgram_ = glCreateProgram();
    // Attach them
    glAttachShader(shaderProgram_, vertexShaderObject_);
    glAttachShader(shaderProgram_, fragmentShaderObject_);
    if ( geoShaderCode_.size() > 0 ) {
        glAttachShader(shaderProgram_, geoShaderObject_);
    }
    GLint success;
    // Link them
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
    unbind();
}		// -----  end of method Shader::createProgram  -----

