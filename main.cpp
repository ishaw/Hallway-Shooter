
#include "Input.h"

#include <irrlicht/irrlicht.h>
#include <iostream>
#include <memory>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

void CheckPlayerCollision();

enum
{
    ID_IsNotPickable = 0,

    IDFlag_IsPickable = 1 << 0,

    IDFlag_IsHighlightable = 1 << 1
};

IrrlichtDevice *device =
        createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

IVideoDriver* driver = device->getVideoDriver();
ISceneManager* smgr = device->getSceneManager();

struct Actor
{
    IAnimatedMesh* mesh;
    IAnimatedMeshSceneNode* node;

    Actor( const char* meshFile, const char* textureFile, 
           ISceneNode* parent=0, int id=0 )
        : mesh( smgr->getMesh(meshFile) )
        , node( smgr->addAnimatedMeshSceneNode(mesh, parent, id) )
    {
        node->setMaterialFlag( EMF_LIGHTING, false );
        node->setMaterialTexture( 0, driver->getTexture(textureFile) );
    }

    void state( vector3df pos, vector3df rot = vector3df(0,0,0) )
    {
        node->setPosition( pos );
        node->setRotation( rot );
    }

    ~Actor()
    {
        mesh->drop();
        node->drop();
    }
};

typedef std::unique_ptr< Actor > ActorPtr;

ActorPtr cube, gun;

bool initialize()
{
    if (device == 0)
        return false; 

    device->getFileSystem()->addZipFileArchive("./media/Meshes/map-20kdm2.pk3");

    IAnimatedMesh* level_mesh = smgr->getMesh("20kdm2.bsp");
    IMeshSceneNode* level_node = 0;
    
    if (level_mesh)
        level_node = smgr->addOctreeSceneNode(level_mesh->getMesh(0), 0, -1, 1024);
    
    ITriangleSelector* selector = 0;
    ISceneCollisionManager* collision_manager = smgr->getSceneCollisionManager();
        
    if (level_node)
    {
        level_node->setPosition(vector3df(-1300,-144,-1249));
        
        selector = smgr->createOctreeTriangleSelector(
                level_node->getMesh(), level_node, 128);
        level_node->setTriangleSelector(selector);
    }

    //setup scene camera
    ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
    camera->setPosition(vector3df(0, 50 , 0));
    
    //adding a cube to play with collision detection
    cube.reset( new Actor("media/Meshes/gun.md2", "media/Textures/cube.jpg") );
    cube->state( vector3df(290,120,456) );
    
    //added FPS weapon
    gun.reset( new Actor("media/Meshes/gun.md2", "media/Textures/gun.jpg", camera) );
    gun->state( vector3df(0,0,0), vector3df(-90,-90,90) );
    gun->node->setLoopMode(true);
    gun->node->setMD2Animation("idle");

    if(selector)
    {
                ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(30, 50, 30),
                                    vector3df(0, -10, 0), vector3df(0, 30, 0));
                camera->addAnimator(anim);
                anim->drop();
    }

    //ISceneNodeAnimatorCollisionResponse* collresp = smgr->createCollisionResponseAnimator(selector, camera, vector3df(30,50,30), vector3df(0,0,0), vector3df(0, 30, 0));

    selector->drop();

    
    device->getCursorControl()->setVisible(false);

    return true;
}

void main_loop()
{
    int lastFPS = -1;

    while(device->run())
    {
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, video::SColor(255,200,200,200));
            smgr->drawAll();
            driver->endScene();

            int fps = driver->getFPS();

            if (lastFPS != fps)
            {
                core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
            }
        }
        else
            device->yield();
    }
}

int main()
{
    if( ! initialize() )
        return 1;

    main_loop();

    device->drop();
    return 0;
}
