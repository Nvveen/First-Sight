// ============================================================================
// 
//       Filename:  Functionoid.h
// 
//    Description:  Functionoid base class. Using this class we can implement
//                  callback functions.
// 
//        Version:  1.0
//        Created:  04/24/2011 11:10:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  FUNCTIONOID_H
#define  FUNCTIONOID_H

// ============================================================================
//        Class:  Functionoid
//  Description:  Base class for functionoids.
// ============================================================================
class Functionoid
{
    public:
        virtual ~Functionoid() = 0;
        virtual void operator() () = 0;
}; // -----  end of class Functionoid  -----

//-----------------------------------------------------------------------------
//       Class:  Functionoid
//      Method:  ~Functionoid
// Description:  destructor
//-----------------------------------------------------------------------------
    inline
Functionoid::~Functionoid ()
{
}  // -----  end of method Functionoid::~Functionoid  (destructor)  -----

#endif   // ----- #ifndef FUNCTIONOID_H  -----
