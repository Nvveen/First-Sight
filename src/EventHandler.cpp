// ============================================================================
// 
//       Filename:  EventHandler.cpp
// 
//    Description:  Handles all events from SFML.
// 
//        Version:  1.0
//        Created:  04/20/2011 02:12:19 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    "EventHandler.h"

//-----------------------------------------------------------------------------
//       Class:  EventHandler
//      Method:  EventHandler
// Description:  constructor
//-----------------------------------------------------------------------------
EventHandler::EventHandler ( Context& context ) :
    context_(&context)
{
    keyPressed_ = false;
}  // -----  end of method EventHandler::EventHandler  (constructor)  -----

    void
EventHandler::pollEvents ()
{
    while ( SDL_PollEvent(&event_) ) {
        if ( event_.type == SDL_WINDOWEVENT ) {
            if ( event_.window.event == SDL_WINDOWEVENT_CLOSE ) {
                context_->windowOpened = false;
            }
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

