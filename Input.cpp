
#include "Input.h"

#include <functional>

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

Mouse::Mouse()
    : wheelPos( 0 )
{
    std::fill_n( buttons, N_BUTTONS, RELEASED );
}

bool Mouse::OnEvent( const irr::SEvent& e )
{
    if( e.EventType == irr::EET_MOUSE_INPUT_EVENT )
    {
        typedef std::function< void(int,KeySate) > KeyFunc;
        KeyFunc on_change = [&]( int i, KeySate inputState )
        {
            int other = inputState ^ ( PRESSED | RELEASED );
            if( buttons[ i ] & other )
                buttons[ i ] = KeySate( inputState | ACTIVE );
            else
                // Unsets the ACTIVE bit if set.
                buttons[ i ] = KeySate( buttons[i] & inputState ); 
        };


        switch( e.MouseInput.Event )
        {
          case irr::EMIE_LMOUSE_PRESSED_DOWN: on_change( LEFT,   PRESSED  ); break;
          case irr::EMIE_RMOUSE_PRESSED_DOWN: on_change( RIGHT,  PRESSED  ); break;
          case irr::EMIE_MMOUSE_PRESSED_DOWN: on_change( MIDDLE, PRESSED  ); break;
          case irr::EMIE_LMOUSE_LEFT_UP:      on_change( LEFT,   RELEASED ); break;
          case irr::EMIE_RMOUSE_LEFT_UP:      on_change( RIGHT,  RELEASED ); break;
          case irr::EMIE_MMOUSE_LEFT_UP:      on_change( MIDDLE, RELEASED ); break;

          case irr::EMIE_MOUSE_WHEEL: wheelPos += e.MouseInput.Wheel;

          case irr::EMIE_MOUSE_MOVED: 
            position.X = e.MouseInput.X;
            position.Y = e.MouseInput.Y;
        }
    }

    return false;
}

bool Mouse::button_pressed( Buttons b )
{
    return buttons[ b ] & ( PRESSED | ACTIVE );
}

bool Mouse::button_released( Buttons b )
{
    return buttons[ b ] & ( RELEASED | ACTIVE );
}

bool Mouse::button_down( Buttons b )
{
    return buttons[ b ] & PRESSED;
}
