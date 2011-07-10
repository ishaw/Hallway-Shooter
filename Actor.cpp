
#include "Actor.h"

Actor::Actor( const char* meshFile, const char* textureFile, 
              irr::scene::ISceneNode* parent, int id )
        : mesh( smgr->getMesh(meshFile) )
        , node( smgr->addAnimatedMeshSceneNode(mesh, parent, id) )
{
    node->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    node->setMaterialTexture( 0, driver->getTexture(textureFile) );
}

void Actor::state( Vector pos, Vector rot )
{
    node->setPosition( pos );
    node->setRotation( rot );
}

Actor::~Actor()
{
    mesh->drop();
    node->drop();
}

