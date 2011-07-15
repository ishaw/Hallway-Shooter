
#include <irrlicht.h>
#include <algorithm>

enum KeySate 
{
    PRESSED  = 1,
    RELEASED = 2,
    ACTIVE   = 4 // Was it pressed or released this frame?
};

struct Keyboard : public irr::IEventReceiver
{
    bool states[ irr::KEY_KEY_CODES_COUNT ];

    Keyboard();

    bool OnEvent( const irr::SEvent& e );

    bool key_pressed( irr::EKEY_CODE c );
    bool key_pressed( char c ); // Works for a-z, 0-9, whitespace.
};

struct Mouse : public irr::IEventReceiver
{
    enum Buttons { LEFT, RIGHT, MIDDLE, N_BUTTONS };

    irr::core::position2di position;
    KeySate buttons[ N_BUTTONS ];
    float wheelPos;

    Mouse();

    bool OnEvent( const irr::SEvent& e );

    bool button_pressed(  Buttons b );
    bool button_released( Buttons b );
    bool button_down(     Buttons b );
};
