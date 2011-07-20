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
#include    <set>
#include    <functional>
#include    <map>
#include    "Context.h"

// ============================================================================
//        Class:  EventHandler
//  Description:  A class to handle events by catching and wrapping them from
//                SDL.
// ============================================================================
class EventHandler
{
    typedef std::function<void()> Function;
    public:
        enum class Key : unsigned int;
        typedef std::set<Key> Keyset;

        EventHandler ( Context& context );
        void pollEvents ();
        template<typename Lambda>
            void bind ( Lambda l, Key key );
        template<typename Lambda>
            void bind ( Lambda l, Keyset set );
    protected:
    private:
        SDL_Event event_;
        Context* context_;
        std::multimap<Keyset, Function> keyMap_;
}; // -----  end of class EventHandler  -----

template<typename Lambda>
    void
EventHandler::bind ( Lambda l, Key key )
{
    Keyset set = Keyset{key};
    Function f = Function(l);
    keyMap_.insert(std::pair<Keyset, Function>(set, f));
}		// -----  end of method EventHandler::bind  -----

template<typename Lambda>
    void
EventHandler::bind ( Lambda l, Keyset set )
{
    Function f = Function(l);
    keyMap_.insert(std::pair<Keyset, Function>(set, f));
}		// -----  end of method EventHandler::bind  -----

enum class EventHandler::Key : unsigned int {
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
    Lshift = SDLK_LSHIFT,
    Rshift = SDLK_RSHIFT,
    Lctrl = SDLK_LCTRL,
    Rctrl = SDLK_RCTRL,
    Lalt = SDLK_LALT,
    Ralt = SDLK_RALT,
};
