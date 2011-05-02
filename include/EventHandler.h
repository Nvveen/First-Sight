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

// ============================================================================
//        Class:  EventHandler
//  Description:  This class intercepts all events thrown by sf::Event, and
//                send to corresponding signals to the appropriate classes.
// ============================================================================
class EventHandler
{
    typedef boost::function<void()> Functionoid;
    public:
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

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
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
