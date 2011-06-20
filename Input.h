
#include <irrlicht/irrlicht.h>
#include <algorithm>

struct Keyboard : public irr::IEventReceiver
{
    bool states[ irr::KEY_KEY_CODES_COUNT ];

    Keyboard();

    virtual bool OnEvent( const irr::SEvent& e );

    bool key_pressed( irr::EKEY_CODE c );
    bool key_pressed( char c ); // Works for a-z, 0-9, whitespace.
};
