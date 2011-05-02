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

//-----------------------------------------------------------------------------
//       Class:  EventHandler
//      Method:  pollEvents
// Description:  This function walks through every SDL event that is polled,
//               handlse standard events like closing a window, and stuff like
//               opening a callback on a bound function.
//-----------------------------------------------------------------------------
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
            SDL_Keysym sym = event_.key.keysym;
            Keyset keyset((Key::Mod)sym.mod, (Key::Code)sym.scancode);
            std::map<Keyset, Functionoid>::iterator it;
            for ( it = keyMap_.begin(); it != keyMap_.end(); it ++ )
                if ( it->first == keyset ) (it->second)();
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

