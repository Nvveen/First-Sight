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
        if ( event_.type == SDL_KEYDOWN ) {
            int size;
            Uint8 *keys = SDL_GetKeyboardState(&size);
            for ( int i = 0; i < size; i += 1 ) {
                if ( keys[i] )
                    keyPressed_.insert((Key::Code)i);
            }
            std::map<Key::Code, Functionoid>::iterator it;
            it = keyMap_.find(*keyPressed_.begin());
            if ( it != keyMap_.end() ) (it->second)();
        }
        if ( event_.type == SDL_KEYUP ) {
            SDL_keysym key = event_.key.keysym;
            Key::Code scanCode = (Key::Code)key.scancode;
            keyPressed_.erase(keyPressed_.find(scanCode));
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

