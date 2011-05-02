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
<<<<<<< HEAD
    keyPressed_ = false;
    window_ = context_->window_;
}  // -----  end of method EventHandler::EventHandler  (constructor)  -----

    void
EventHandler::pollEvents ()
{
    while ( window_->PollEvent(event_) ) {
        if ( event_.Type == sf::Event::KeyPressed ) {
            keyPressed_ = true;
            code = event_.Key.Code;
        }
        if ( event_.Type == sf::Event::Closed ) {
            window_->Close();
        }
        if ( (event_.Type == sf::Event::KeyPressed) && 
             (event_.Key.Code == Code::Escape) ) {
            window_->Close();
        }
        if ( event_.Type == sf::Event::Resized ) {
            context_->resize(event_.Size.Width, event_.Size.Height);
        }
        if ( event_.Type == sf::Event::KeyPressed ) {
            std::map<sf::Key::Code, Functionoid>::iterator it;
            it = keyMap_.find(event_.Key.Code);
            if ( it != keyMap_.end() ) (it->second)();
=======
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
>>>>>>> sdl
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

