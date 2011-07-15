#include "Input.h"
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



IrrlichtDevice *device =

        createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));



IVideoDriver* driver = device->getVideoDriver();

ISceneManager* smgr = device->getSceneManager();

ISceneCollisionManager* cmgr = smgr->getSceneCollisionManager();

ICameraSceneNode* camera;

/*IAnimatedMeshSceneNode* gun_node;

IAnimatedMeshSceneNode* cube_node;

IAnimatedMesh* cube_mesh;

IAnimatedMesh* gun_mesh;
*/
bool CheckCollisionAB(IAnimatedMeshSceneNode* node_a=0, IAnimatedMeshSceneNode* node_b=0, ICameraSceneNode* player = 0 )
{
	line3d<f32> ray;
	
	if(node_a == 0)
		ray.start = camera->getPosition();
	if(player == 0)
		ray.start = node_a->getPosition();
	ray.end = ray.start + (node_b->getPosition() - ray.start).normalize() * 2.5f;

	vector3df intersection;
	triangle3df hitTriangle;
	
	ISceneNode * selectedSceneNode = cmgr->getSceneNodeAndCollisionPointFromRay( ray, intersection, hitTriangle); 

	if(selectedSceneNode)
	{
		if(selectedSceneNode->getName() == node_b->getName())
			return true;
		else
			return false;

	}
	else
		return false;

}


struct Actor
{
    IAnimatedMesh* mesh;
    IAnimatedMeshSceneNode* node;

    Actor( const char* meshFile, const char* textureFile, 
           ISceneNode* parent=0, int id=0 )
        : mesh( smgr->getMesh( meshFile )                          )
        , node( smgr->addAnimatedMeshSceneNode( mesh, parent, id ) )
    {
        node->setMaterialFlag( EMF_LIGHTING, true );
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

IAnimatedMeshSceneNode* animated_node( const char* meshFile, const char* textureFile, ISceneNode* parent=0, int id=0 )
{
    IAnimatedMesh* mesh          = smgr->getMesh( meshFile );
    IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh, parent, id );

    node->setMaterialFlag( EMF_LIGHTING, false );
    node->setMaterialTexture( 0, driver->getTexture(textureFile) );

    return node;
}

void set_state( IAnimatedMeshSceneNode* node, vector3df pos, vector3df rot = vector3df(0,0,0) )
{
    node->setPosition( pos );
    node->setRotation( rot );
}

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
       
    if (level_node)

    {

        level_node->setPosition(vector3df(-1300,-144,-1249));

        

        selector = smgr->createOctreeTriangleSelector(

                level_node->getMesh(), level_node, 128);

        level_node->setTriangleSelector(selector);

    }



    //setup scene camera

	SKeyMap fps_keymap[10];
	fps_keymap[0].Action = EKA_MOVE_FORWARD;
	fps_keymap[0].KeyCode = KEY_UP;
	fps_keymap[1].Action = EKA_MOVE_FORWARD;
	fps_keymap[1].KeyCode = KEY_KEY_W;

	fps_keymap[2].Action = EKA_MOVE_BACKWARD;
	fps_keymap[2].KeyCode = KEY_DOWN;
	fps_keymap[3].Action = EKA_MOVE_BACKWARD;
	fps_keymap[3].KeyCode = KEY_KEY_S;

	fps_keymap[4].Action = EKA_STRAFE_LEFT;
	fps_keymap[4].KeyCode = KEY_LEFT;
	fps_keymap[5].Action = EKA_STRAFE_LEFT;
	fps_keymap[5].KeyCode = KEY_KEY_A;

	fps_keymap[6].Action = EKA_STRAFE_RIGHT;
	fps_keymap[6].KeyCode = KEY_RIGHT;
	fps_keymap[7].Action = EKA_STRAFE_RIGHT;
	fps_keymap[7].KeyCode = KEY_KEY_D;

	/*fps_keymap[8].Action = EKA_JUMP_UP;
	fps_keymap[8].KeyCode = KEY_KEY_J;

	fps_keymap[9].Action = EKA_CROUCH;
	fps_keymap[9].KeyCode = KEY_KEY_C;*/

    camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, fps_keymap, 10, true, 3.f);

    camera->setPosition(vector3df(0, 50 , 0));

    

    //adding a cube to play with collision detection
    IAnimatedMesh* cube_mesh = smgr->getMesh("media/Meshes/cube.obj");
	
    IAnimatedMeshSceneNode* cube_node = animated_node( "media/Meshes/cube.dae", "media/Textures/cube.jpg", 0, 15 );
    set_state( cube_node, vector3df(290,128,456) );
	cube_node->setScale(vector3df(10,10,10));

    IAnimatedMesh* gun_mesh = smgr->getMesh("media/Meshes/gun.md2");

    IAnimatedMeshSceneNode* gun_node = smgr->addAnimatedMeshSceneNode(gun_mesh, smgr->getActiveCamera(), 10, vector3df(0,0,0), vector3df(-90,-90,90));

    gun_node->setMaterialFlag(EMF_LIGHTING, false);

    gun_node->setMaterialTexture(0, driver->getTexture("media/Textures/gun.jpg"));

    gun_node->setLoopMode(true);

    gun_node->setMD2Animation("idle");

	
    if(selector)
    {

                ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, vector3df(30, 50, 30),

                                    vector3df(0, -10, 0), vector3df(0, 30, 0));

                camera->addAnimator(anim);

                anim->drop();

    }

    selector = smgr->createTriangleSelector(cube_node);
	cube_node->setTriangleSelector(selector);
	selector->drop();

	selector = smgr->createTriangleSelector(gun_node);
	gun_node->setTriangleSelector(selector);
	selector->drop();

    device->getCursorControl()->setVisible(false);

	 int lastFPS = -1;



    while(device->run())

    {

        if (device->isWindowActive())

        {

            driver->beginScene(true, true, video::SColor(255,200,200,200));

            smgr->drawAll();

			if(CheckCollisionAB(0, cube_node, camera))
			{
				cube_node->setVisible(false);
			}

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

    return true;
}

void main_loop()
{
   
}

int main()

{

    if( ! initialize() )
        return 1;

    //main_loop();

    device->drop();

    return 0;

}