
#include "Engine.h"

irr::IrrlichtDevice *device =
        irr::createDevice ( 
            irr::video::EDT_OPENGL, 
            irr::core::dimension2d<irr::u32>(640, 480) 
        );

irr::video::IVideoDriver*  driver = device->getVideoDriver();
irr::scene::ISceneManager* smgr   = device->getSceneManager();
