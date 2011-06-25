#include <irrlicht.h>
#include <iostream>
#include <irrbullet.h>

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
world =

int main()
{

	if (device == 0)
		return 1; 

	device->getFileSystem()->addZipFileArchive("./map-20kdm2.pk3");

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
	IAnimatedMesh* cube_mesh = smgr->getMesh("cube.obj");
	IAnimatedMeshSceneNode* cube_node = smgr->addAnimatedMeshSceneNode(cube_mesh);
	cube_node->setMaterialFlag(EMF_LIGHTING, false);
	cube_node->setMaterialTexture(0, driver->getTexture("cube.jpg"));
	cube_node->setPosition(vector3df(290,128,456));
    
	//added FPS weapon
	IAnimatedMesh* gun_mesh = smgr->getMesh("gun.md2");
    IAnimatedMeshSceneNode* gun_node = smgr->addAnimatedMeshSceneNode(gun_mesh, smgr->getActiveCamera(), 10, vector3df(0,0,0), vector3df(-90,-90,90));
    gun_node->setMaterialFlag(EMF_LIGHTING, false);
	gun_node->setMaterialTexture(0, driver->getTexture("gun.jpg"));
	gun_node->setLoopMode(true);
	gun_node->setMD2Animation("idle");

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
	device->drop();
	return 0;
}