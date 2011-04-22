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
             (event_.Key.Code == sf::Key::Escape) ) {
            window_->Close();
        }
        if ( event_.Type == sf::Event::Resized ) {
            context_->resize(event_.Size.Width, event_.Size.Height);
            std::cout << "test\n";
        }
    }
}		// -----  end of method EventHandler::pollEvents  -----

