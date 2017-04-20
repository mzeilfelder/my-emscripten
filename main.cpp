#include <irrlicht.h>
#include "exampleHelper.h"
#include <emscripten.h>
#include <stdio.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
IGUIEnvironment* guienv = 0;

/*
	emscripten can't run things in an endless-loop or otherwise the browse will consider
	the script to hang.
*/
void one_iter()
{
    if(!device->run()) 
	{
        emscripten_cancel_main_loop();
        return;
    }
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
}


int main()
{
	
	irr::SIrrlichtCreationParameters params;
	params.DriverType = video::EDT_WEBGL1;
//	params.DriverType = video::EDT_OGLES2;
	params.WindowSize = core::dimension2d<u32>(640, 480);
	params.LoggingLevel = ELL_DEBUG;
	device = createDeviceEx(params);
	
	if (!device)
		return 1;

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	
	device->setWindowCaption(L"Hello World!");
	guienv->addStaticText(L"Hello World! This is the Irrlicht on emscripten!", rect<s32>(10,10,260,22), true);

	const io::path mediaPath = getExampleMediaPath();

#if 1
	IAnimatedMesh* mesh = smgr->getMesh(mediaPath + "sydney.md2");
//	IAnimatedMesh* mesh = smgr->getMesh(mediaPath + "room.3ds");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture(mediaPath + "sydney.bmp") );
	}
#else
	IAnimatedMesh* mesh = smgr->getMesh("media/box.obj");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IMeshSceneNode * node = smgr->addMeshSceneNode(mesh);
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMaterialTexture( 0, driver->getTexture(mediaPath + "sydney.bmp") );
	}	
#endif

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
	//scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0,100.0f,1.2f);


	// Setting fps to 0 or a negative value will use the browser’s requestAnimationFrame mechanism to call the main loop function. 
	// emscripten documentation recommends that one. 
	// But you can also set another fps value and the browser will try to call the main-loop as often.
	int fps = 0;	
	int simulate_infinite_loop = 1;
	emscripten_set_main_loop(one_iter, fps, simulate_infinite_loop);

	return 0;
}
