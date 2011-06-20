
#include "Input.h"

Keyboard::Keyboard()
{
    std::fill_n( states, irr::KEY_KEY_CODES_COUNT, false );
}

bool Keyboard::OnEvent( const irr::SEvent& e )
{
    if( e.EventType == irr::EET_KEY_INPUT_EVENT )
    {
        states[ e.KeyInput.Key ] = e.KeyInput.PressedDown;
    }

    return false;
}

bool Keyboard::key_pressed( irr::EKEY_CODE c )
{
    return states[ c ];
}

bool Keyboard::key_pressed( char c )
{
    if( c >= 'a' && c <= 'z' )
        // Correct ASCII value for Irrlicht value.
        c -= 0x20;

    return states[ c ];
}
