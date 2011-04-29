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
#include    <map>
#include    "Context.h"

// ============================================================================
//        Class:  EventHandler
//  Description:  This class intercepts all events thrown by sf::Event, and
//                send to corresponding signals to the appropriate classes.
// ============================================================================
class EventHandler
{
    typedef boost::function<void()> Functionoid;
    public:
//        template <class T, class U>
//            void bindFunction ( T t, U u, sf::Key::Code key );
//        template <class T, class U, class V>
//            void bind ( T t, U u, V v, sf::Key::Code key );
//        template <class T, class U, class V, class W>
//            void bind ( T t, U u, V v, W w, sf::Key::Code key );
//        template <class T, class U, class V, class W, class X>
//            void bind ( T t, U u, V v, W w, X x, sf::Key::Code key );
//        template <class T, class U, class V, class W, class X, class Y>
//            void bind ( T* t, U u, V v, W w, X x, Y y, sf::Key::Code key );
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
}; // -----  end of class EventHandler  -----
//
//template < class T, class U >
//    void
//EventHandler::bindFunction ( T t, U u, sf::Key::Code key )
//{
//    keyMap_[key] = boost::bind(u, t);
//}		// -----  end of method EventHandler::bind  -----
//
//template < class T, class U, class V >
//    void
//EventHandler::bind ( T t, U u, V v, sf::Key::Code key )
//{
//    keyMap_[key] = boost::bind(u, t, v);
//}		// -----  end of method EventHandler::bind  -----
//
//template < class T, class U, class V, class W >
//    void
//EventHandler::bind ( T t, U u, V v, W w, sf::Key::Code key )
//{
//    keyMap_[key] = boost::bind(u, t, v, w);
//}		// -----  end of method EventHandler::bind  -----
//
//template < class T, class U, class V, class W, class X >
//    void
//EventHandler::bind ( T t, U u, V v, W w, X x, sf::Key::Code key )
//{
//    keyMap_[key] = boost::bind(u, t, v, w, x);
//}		// -----  end of method EventHandler::bind  -----
//
//template < class T, class U, class V, class W, class X, class Y >
//    void
//EventHandler::bind ( T* t, U u, V v, W w, X x, Y y, sf::Key::Code key )
//{
//    keyMap_[key] = boost::bind(u, t, v, w, x, y);
//}		// -----  end of method EventHandler::bind  -----
