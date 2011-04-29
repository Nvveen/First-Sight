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
        typedef std::set<Key::Code> Keyset;
        template <class T, class U>
            void bind ( T* t, U u, Key::Code key );
        template <class T, class U>
            void bind ( T* t, U u, std::set<Key::Code> keys );
        template <class T, class U, class V>
            void bind ( T* t, U u, V v, Key::Code key );
        template <class T, class U, class V>
            void bind ( T* t, U u, V v, std::set<Key::Code> keys );
        template <class T, class U, class V, class W>
            void bind ( T* t, U u, V v, W w, Key::Code key );
        template <class T, class U, class V, class W>
            void bind ( T* t, U u, V v, W w, std::set<Key::Code> keys );
        template <class T, class U, class V, class W, class X>
            void bind ( T* t, U u, V v, W w, X x, Key::Code key );
        template <class T, class U, class V, class W, class X>
            void bind ( T* t, U u, V v, W w, X x, std::set<Key::Code> keys );
        template <class T, class U, class V, class W, class X, class Y>
            void bind ( T* t, U u, V v, W w, X x, Y y, Key::Code key );
        template <class T, class U, class V, class W, class X, class Y>
            void bind ( T* t, U u, V v, W w, X x, Y y, 
                        std::set<Key::Code> keys );
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
        Keyset keyPressed_;
        std::map<Keyset, Functionoid> keyMap_;
}; // -----  end of class EventHandler  -----

template < class T, class U >
    void
EventHandler::bind ( T* t, U u, Key::Code key )
{
    Keyset keys;
    keys.insert(key);
    keyMap_[keys] = boost::bind(u, t);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U >
void EventHandler::bind ( T* t, U u, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V >
    void
EventHandler::bind ( T* t, U u, V v, Key::Code key )
{
    Keyset keys(1, key);
    keyMap_[keys] = boost::bind(u, t, v);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V >
    void
EventHandler::bind ( T* t, U u, V v, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W >
    void
EventHandler::bind ( T* t, U u, V v, W w, Key::Code key )
{
    Keyset keys;
    keys.insert(key);
    keyMap_[keys] = boost::bind(u, t, v, w);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W >
    void
EventHandler::bind ( T* t, U u, V v, W w, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X >
    void
EventHandler::bind ( T* t, U u, V v, W w, X x, Key::Code key )
{
    Keyset keys;
    keys.insert(key);
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X >
    void
EventHandler::bind ( T* t, U u, V v, W w, X x, Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method EventHandler::bind  -----

template < class T, class U, class V, class W, class X, class Y >
    void
EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, Key::Code key )
{
    Keyset keys;
    keys.insert(key);
    keyMap_[keys] = boost::bind(u, t, v, w, x);
}		// -----  end of method eventhandler::bind  -----

template < class T, class U, class V, class W, class X, class Y >
    void
EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, 
                     Keyset keys )
{
    keyMap_[keys] = boost::bind(u, t, v, w, x, y);
}		// -----  end of method eventhandler::bind  -----
