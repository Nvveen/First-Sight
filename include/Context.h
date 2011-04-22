// ============================================================================
// 
//       Filename:  Context.h
// 
//    Description:  Class that handles the context creation of the window in
//                  freeglut.
// 
//        Version:  1.0
//        Created:  04/07/2011 07:56:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  CONTEXT_H
#define  CONTEXT_H

#include    <string>
#include    <vector>
#include    <map>
#include    "Projection.h"
#include    "Object.h"
#include    "Shader.h"

// ============================================================================
//        Class:  Context
//  Description:  Context creation class that handles the calls to the objects.
// ============================================================================
class Context
{
    public:

        // ====================  LIFECYCLE     ================================
        Context ( GLfloat w, GLfloat h, std::string windowName,
                  Projection* proj=NULL ); 
        ~Context ();
        void setup ();
        void render ();
        void resize ( int w, int h );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        GLfloat w_;
        GLfloat h_;
        std::string windowName_;

        Projection proj_;

        std::vector<Object> objects;
        std::map<std::string, Shader> shaders;

}; // -----  end of class Context  -----


#endif   // ----- #ifndef CONTEXT_H  -----
