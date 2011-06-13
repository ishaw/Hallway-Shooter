#include <irrlicht.h>
#include <iostream>

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

enum
{
	ID_IsNotPickable = 0,

	IDFlag_IsPickable = 1 << 0,

	IDFlag_IsHighlightable = 1 << 1
};

int main()
{
	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; 

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addZipFileArchive("./map-20kdm2.pk3");

	IAnimatedMesh* level_mesh = smgr->getMesh("20kdm2.bsp");
	IMeshSceneNode* level_node = 0;
    
	if (level_mesh)
		level_node = smgr->addOctreeSceneNode(level_mesh->getMesh(0), 0, -1, 1024);
	
    ITriangleSelector* selector = 0;
    	
	if (level_node)
	{
		level_node->setPosition(vector3df(-1300,-144,-1249));
		
        selector = smgr->createOctreeTriangleSelector(
				level_node->getMesh(), level_node, 128);
		level_node->setTriangleSelector(selector);
    }
    
    IAnimatedMesh* gun_m = smgr->getMesh("gun.3ds");
    
	ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
    
    camera->setPosition(vector3df(0, 100 , 0));
    
    ISceneNode* gun_n = smgr->addAnimatedMeshSceneNode(gun_m, camera, -1);
    
    gun_n->setPosition(vector3df(10,-5,20));
    
    if(selector)
    {
                ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(30, 50, 30),
                                    vector3df(0, -10, 0), vector3df(0, 30, 0));
                selector->drop();
                camera->addAnimator(anim);
                anim->drop();
    }
    
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
