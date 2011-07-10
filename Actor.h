
#pragma once

#include "Engine.h"

#include <irrlicht/irrlicht.h>

#include <memory>

struct Actor
{
    typedef irr::core::vector3df Vector;

    irr::scene::IAnimatedMesh*          mesh;
    irr::scene::IAnimatedMeshSceneNode* node;

    Actor( const char* meshFile, const char* textureFile, 
           irr::scene::ISceneNode* parent=0, int id=0 );

    void state( Vector pos, Vector rot = Vector(0,0,0) );

    ~Actor();
};

typedef std::unique_ptr< Actor > ActorPtr;
