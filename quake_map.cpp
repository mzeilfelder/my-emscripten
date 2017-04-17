#include <irrlicht.h>
#include "exampleHelper.h"
#include <emscripten.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;

/*
	emscripten can't run things in an endless-loop or otherwise the browse will consider
	the script to hang.
*/
void one_iter()
{
	static IVideoDriver* driver = device->getVideoDriver();
	static ISceneManager* smgr = device->getSceneManager();
	static IGUIEnvironment* guienv = device->getGUIEnvironment();
	
    if(!device->run()) 
	{
        emscripten_cancel_main_loop();
        return;
    }
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
}

int main()
{
	device = createDevice(video::EDT_OGLES2, dimension2d<u32>(640, 480), 32, false, false, false, 0);

	if (!device)
		return 1;

	device->getFileSystem()->addFileArchive(getExampleMediaPath() + "map-20kdm2.pk3");

	ISceneManager* smgr = device->getSceneManager();
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

	smgr->addCameraSceneNodeFPS(0, 10.f);

	device->getCursorControl()->setVisible(false);

	
	// Setting fps to 0 or a negative value will use the browser’s requestAnimationFrame mechanism to call the main loop function. 
	// emscripten documentation recommends that one. 
	// But you can also set another fps value and the browser will try to call the main-loop as often.
	int fps = 60;	
	int simulate_infinite_loop = 1;
	emscripten_set_main_loop(one_iter, fps, simulate_infinite_loop);
	
	return 0;
}
