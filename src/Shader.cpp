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
#include    <GL/glew.h>
#include    <glm/gtc/type_ptr.hpp>

#include    "Shader.h"

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
Shader::Shader ( std::string vertexShaderFile, std::string fragmentShaderFile )
{
    // Read in code
    vertexShaderCode_ = addCode(vertexShaderFile);
    fragmentShaderCode_ = addCode(fragmentShaderFile);
    // Create shader program
    createProgram();
}  // -----  end of method Shader::Shader  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  bind
// Description:  Binds a shader program for active use.
//-----------------------------------------------------------------------------
    void
Shader::bind ()
{
    if ( shaderProgram_ ) glUseProgram(shaderProgram_);
}		// -----  end of method Shader::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  unbind
// Description:  Unbinds the bound shader program.
//-----------------------------------------------------------------------------
    void
Shader::unbind ()
{
    if ( shaderProgram_ ) glUseProgram(0);
}		// -----  end of method Shader::unbind  -----

//-----------------------------------------------------------------------------
//       Class:  Shader
//      Method:  setUniform
// Description:  Sets a uniform variable for the shader.
//-----------------------------------------------------------------------------
    bool
Shader::setUniform ( std::string name, glm::mat4 matrix )
{
    if ( shaderProgram_ ) {
        GLint loc = glGetUniformLocation(shaderProgram_, name.c_str());
        if ( loc == -1 ) {
            std::cerr << "Could not find the uniform in the shader.\n";
            return false;
        }
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
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
        GLint loc = glGetUniformLocation(shaderProgram_, name.c_str());
        if ( loc == -1 ) {
            std::cerr << "Could not find the uniform in the shader.\n";
            return false;
        }
        glUniform1i(loc, val);
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
        std::cerr << infoLog << "\n";
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
    shaderProgram_ = glCreateProgram();
    // Create the two shader objects
    vertexShaderObject_ = compileShader(vertexShaderCode_, GL_VERTEX_SHADER);
    fragmentShaderObject_ = compileShader(fragmentShaderCode_, 
                                         GL_FRAGMENT_SHADER);

    // Attach them
    glAttachShader(shaderProgram_, vertexShaderObject_);
    glAttachShader(shaderProgram_, fragmentShaderObject_);
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
}		// -----  end of method Shader::createProgram  -----
