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
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
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
        if ( event_.type == SDL_QUIT ) {
            context_->windowOpened_ = false;
        }
        if ( event_.type == SDL_KEYDOWN ) {
            // Get the keyboard state.
            int size;
            Uint8 *keyState = SDL_GetKeyState(&size);
            Keyset keySet;
            // Walk through the state and add every pressed key to the set.
            for ( unsigned int i = 0; i < size; i += 1 )
                if ( keyState[i] ) keySet.insert(keySet.end(), (Key)i);
            // If the set is equal to one in the map, we can execute its
            // function.
            auto it = keyMap_.find(keySet);
            if ( it != keyMap_.end() ) (it->second)();
        }
        if ( event_.type == SDL_VIDEORESIZE ) {
            context_->resize(event_.resize.w, event_.resize.h);
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

