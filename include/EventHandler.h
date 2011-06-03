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

#include    <SDL/SDL.h>
#include    <boost/function.hpp>
#include    <boost/bind.hpp>
#include    <map>
#include    "Context.h"

namespace Key{
    enum Code {
        A = SDLK_a,
        B = SDLK_b,
        C = SDLK_c,
        D = SDLK_d,
        E = SDLK_e,
        F = SDLK_f,
        G = SDLK_g,
        H = SDLK_h,
        I = SDLK_i,
        J = SDLK_j,
        K = SDLK_k,
        L = SDLK_l,
        M = SDLK_m, 
        N = SDLK_n,
        O = SDLK_o,
        P = SDLK_p,
        Q = SDLK_q,
        R = SDLK_r,
        S = SDLK_s,
        T = SDLK_t,
        U = SDLK_u,
        V = SDLK_v,
        W = SDLK_w,
        X = SDLK_x,
        Y = SDLK_y,
        Z = SDLK_z,
        Escape = SDLK_ESCAPE,
        Left = SDLK_LEFT,
        Up = SDLK_UP,
        Right = SDLK_RIGHT,
        Down = SDLK_DOWN,
    };
    enum Mod {
        Lshift = KMOD_LSHIFT,
        Rshift = KMOD_RSHIFT,
        Lctrl = KMOD_LCTRL,
        Rctrl = KMOD_RCTRL,
        Lalt = KMOD_LALT,
        Ralt = KMOD_RALT,
        None = KMOD_NONE,
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
