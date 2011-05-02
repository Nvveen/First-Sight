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

<<<<<<< HEAD
#include    <SFML/Graphics.hpp>
#include    <boost/function.hpp>
#include    <boost/bind.hpp>
#include    <map>
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

=======
#include    <SDL.h>
#include    <boost/function.hpp>
#include    <boost/bind.hpp>
#include    <set>
#include    <map>
#include    "Context.h"

namespace Key{
    enum Code {
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M, 
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,
        Escape = SDL_SCANCODE_ESCAPE,
        Left = SDL_SCANCODE_LEFT,
        Up = SDL_SCANCODE_UP,
        Right = SDL_SCANCODE_RIGHT,
        Down = SDL_SCANCODE_DOWN,
    };
    enum Mod {
        None = 0,
        Lctrl = KMOD_LCTRL,
        Lalt = KMOD_LALT,
        Lshift = KMOD_LSHIFT,
        Rctrl = KMOD_RCTRL,
        Ralt = KMOD_RALT,
        Rshift = KMOD_RSHIFT,
    };
}
>>>>>>> sdl
// ============================================================================
//        Class:  EventHandler
//  Description:  This class intercepts all events thrown by sf::Event, and
//                send to corresponding signals to the appropriate classes.
// ============================================================================
class EventHandler
{
    typedef boost::function<void()> Functionoid;
    public:
<<<<<<< HEAD
        template <class T, class U>
            void bindFunction ( T t, U u, sf::Key::Code key );
        template <class T, class U, class V>
            void bind ( T t, U u, V v, sf::Key::Code key );
        template <class T, class U, class V, class W>
            void bind ( T t, U u, V v, W w, sf::Key::Code key );
        template <class T, class U, class V, class W, class X>
            void bind ( T t, U u, V v, W w, X x, sf::Key::Code key );
        template <class T, class U, class V, class W, class X, class Y>
            void bind ( T* t, U u, V v, W w, X x, Y y, sf::Key::Code key );
        // ====================  LIFECYCLE     ================================
        EventHandler ( Context& context );
        void pollEvents ();
        bool isKeyPressed ();
=======
//        typedef std::set<Key::Code> Keyset;
        typedef std::pair<Key::Mod, Key::Code> Keyset;
        template <class T, class U>
            void bind ( T* t, U u, Key::Code key );
        template <class T, class U>
            void bind ( T* t, U u, Keyset keys );
        template <class T, class U, class V>
            void bind ( T* t, U u, V v, Key::Code key );
        template <class T, class U, class V>
            void bind ( T* t, U u, V v, Keyset keys );
        template <class T, class U, class V, class W>
            void bind ( T* t, U u, V v, W w, Key::Code key );
        template <class T, class U, class V, class W>
            void bind ( T* t, U u, V v, W w, Keyset keys );
        template <class T, class U, class V, class W, class X>
            void bind ( T* t, U u, V v, W w, X x, Key::Code key );
        template <class T, class U, class V, class W, class X>
            void bind ( T* t, U u, V v, W w, X x, Keyset keys );
        template <class T, class U, class V, class W, class X, class Y>
            void bind ( T* t, U u, V v, W w, X x, Y y, Key::Code key );
        template <class T, class U, class V, class W, class X, class Y>
            void bind ( T* t, U u, V v, W w, X x, Y y, 
                        Keyset keys );
        // ====================  LIFECYCLE     ================================
        EventHandler ( Context& context );
        void pollEvents ();
>>>>>>> sdl

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

<<<<<<< HEAD
        sf::Key::Code code;
=======
>>>>>>> sdl
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
<<<<<<< HEAD
        sf::Event event_;
        Context* context_;
        sf::RenderWindow* window_;

        std::map<sf::Key::Code, Functionoid> keyMap_;

        bool keyPressed_;
}; // -----  end of class EventHandler  -----

    inline bool
EventHandler::isKeyPressed ()
{
    return keyPressed_;
}		// -----  end of method EventHandler::isKeyPressed  -----

template < class T, class U >
    void
EventHandler::bindFunction ( T t, U u, sf::Key::Code key )
{
    keyMap_[key] = boost::bind(u, t);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V >
    void
EventHandler::bind ( T t, U u, V v, sf::Key::Code key )
{
    keyMap_[key] = boost::bind(u, t, v);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W >
    void
EventHandler::bind ( T t, U u, V v, W w, sf::Key::Code key )
{
    keyMap_[key] = boost::bind(u, t, v, w);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X >
    void
EventHandler::bind ( T t, U u, V v, W w, X x, sf::Key::Code key )
{
    keyMap_[key] = boost::bind(u, t, v, w, x);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X, class Y >
    void
EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, sf::Key::Code key )
{
    keyMap_[key] = boost::bind(u, t, v, w, x, y);
}		// -----  end of method EventHandler::bind  -----
=======
        SDL_Event event_;
        Context* context_;

        std::map<Keyset, Functionoid> keyMap_;
}; // -----  end of class EventHandler  -----

//-----------------------------------------------------------------------------
//       Class:  EventHandler
//      Method:  bind
// Description:  The following template functions bind the parameter that
//               specifies which key(combination) should be bound to which 
//               function.
//-----------------------------------------------------------------------------
template < class T, class U >
    inline void
EventHandler::bind ( T* t, U u, Key::Code key )
{
    Keyset keys(Key::None, key);
    keyMap_[keys] = boost::bind(u, t);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U >
    inline void 
EventHandler::bind ( T* t, U u, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V >
    inline void
EventHandler::bind ( T* t, U u, V v, Key::Code key )
{
    Keyset keys(Key::None, key);
    keyMap_[keys] = boost::bind(u, t, v);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V >
    inline void
EventHandler::bind ( T* t, U u, V v, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, Key::Code key )
{
    Keyset keys(Key::None, key);
    keyMap_[keys] = boost::bind(u, t, v, w);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, X x, Key::Code key )
{
    Keyset keys(Key::None, key);
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, X x, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X, class Y >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, Key::Code key )
{
    Keyset keys(Key::None, key);
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method eventhandler::bind  -----

template < class T, class U, class V, class W, class X, class Y >
    inline void
EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, 
                     Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w, x, y);
}		// -----  end of method eventhandler::bind  -----
>>>>>>> sdl
