// ============================================================================
// 
//       Filename:  EventHandler.h
// 
//    Description:  Handles all events from SFML.
// 
//        Version:  1.0
//        Created:  04/20/2011 02:12:42 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <SFML/Graphics.hpp>
#include    "Context.h"

namespace Code {
    using sf::Key::A;
    using sf::Key::B;
    using sf::Key::C;
    using sf::Key::D;
    using sf::Key::E;
    using sf::Key::F;
    using sf::Key::G;
    using sf::Key::H;
    using sf::Key::I;
    using sf::Key::J;
    using sf::Key::K;
    using sf::Key::L;
    using sf::Key::M;
    using sf::Key::N;
    using sf::Key::O;
    using sf::Key::P;
    using sf::Key::Q;
    using sf::Key::R;
    using sf::Key::S;
    using sf::Key::T;
    using sf::Key::U;
    using sf::Key::V;
    using sf::Key::W;
    using sf::Key::X;
    using sf::Key::Y;
    using sf::Key::Z;
    using sf::Key::Num0;
    using sf::Key::Num1;
    using sf::Key::Num2;
    using sf::Key::Num3;
    using sf::Key::Num4;
    using sf::Key::Num5;
    using sf::Key::Num6;
    using sf::Key::Num7;
    using sf::Key::Num8;
    using sf::Key::Num9;
    using sf::Key::Escape; 
    using sf::Key::LControl;     
    using sf::Key::LShift;       
    using sf::Key::LAlt;         
    using sf::Key::LSystem;      
    using sf::Key::RControl;     
    using sf::Key::RShift;       
    using sf::Key::RAlt;         
    using sf::Key::RSystem;      
    using sf::Key::Menu;         
    using sf::Key::LBracket;     
    using sf::Key::RBracket;     
    using sf::Key::SemiColon;    
    using sf::Key::Comma;        
    using sf::Key::Period;       
    using sf::Key::Quote;        
    using sf::Key::Slash;        
    using sf::Key::BackSlash;    
    using sf::Key::Tilde;        
    using sf::Key::Equal;        
    using sf::Key::Dash;         
    using sf::Key::Space;        
    using sf::Key::Return;       
    using sf::Key::Back;         
    using sf::Key::Tab;          
    using sf::Key::PageUp;       
    using sf::Key::PageDown;     
    using sf::Key::End;          
    using sf::Key::Home;         
    using sf::Key::Insert;       
    using sf::Key::Delete;       
    using sf::Key::Add;          
    using sf::Key::Subtract;     
    using sf::Key::Multiply;     
    using sf::Key::Divide;       
    using sf::Key::Left;         
    using sf::Key::Right;        
    using sf::Key::Up;           
    using sf::Key::Down;         
    using sf::Key::Numpad0;      
    using sf::Key::Numpad1;      
    using sf::Key::Numpad2;      
    using sf::Key::Numpad3;      
    using sf::Key::Numpad4;      
    using sf::Key::Numpad5;      
    using sf::Key::Numpad6;      
    using sf::Key::Numpad7;      
    using sf::Key::Numpad8;      
    using sf::Key::Numpad9;      
    using sf::Key::F1;           
    using sf::Key::F2;           
    using sf::Key::F3;           
    using sf::Key::F4;           
    using sf::Key::F5;           
    using sf::Key::F6;           
    using sf::Key::F7;           
    using sf::Key::F8;           
    using sf::Key::F9;           
    using sf::Key::F10;          
    using sf::Key::F11;          
    using sf::Key::F12;          
    using sf::Key::F13;          
    using sf::Key::F14;          
    using sf::Key::F15;          
    using sf::Key::Pause;        
    using sf::Key::Count;        
}

//        Class:  EventHandler
//  Description:  This class intercepts all events thrown by sf::Event, and
//                send to corresponding signals to the appropriate classes.
// ============================================================================
class EventHandler
{
    public:

        // ====================  LIFECYCLE     ================================
        EventHandler ( Context& context );
        void pollEvents ();
        bool isKeyPressed ();

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

        sf::Key::Code code;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        sf::Event event_;
        Context* context_;
        sf::RenderWindow* window_;
        
        bool keyPressed_;

}; // -----  end of class EventHandler  -----

    inline bool
EventHandler::isKeyPressed ()
{
    return keyPressed_;
}		// -----  end of method EventHandler::isKeyPressed  -----

