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

enum
{
	GUI_ID_BUTTON_LOAD
};


class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver() { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			switch(event.GUIEvent.EventType)
			{
				case EGET_BUTTON_CLICKED:
				{
					switch ( id )
					{
						case GUI_ID_BUTTON_LOAD:
						{
							// just testing what happens when loading a big file							
							//core::stringc url("http://www.irrgheist.com/media/div/hcraft_v1_3b.zip");				// ~25mb
							core::stringc url("http://www.irrgheist.com/emscripten/examples/01.HelloWorld.js");	// ~5mb
							
							void* pbuffer = nullptr;
							int pnum;
							int perror;
							
							emscripten_wget_data(url.c_str(), &pbuffer, &pnum, &perror);
						
							wprintf(L"loaded %d bytes\n", pnum);
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}

		return false;
	}
};

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

MyEventReceiver eventReceiver;	

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
	
	device->setEventReceiver(&eventReceiver);	

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	
	device->setWindowCaption(L"Hello World!");
	guienv->addStaticText(L"Hello World! This is the Irrlicht on emscripten!", rect<s32>(10,10,260,22), true);
	guienv->addButton(rect<s32>(10, 30, 110, 50), 0, GUI_ID_BUTTON_LOAD, L"LOAD");

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
