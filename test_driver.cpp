// Code is under the zlib license (same as Irrlicht)
// Written by Michael Zeilfelder
//
// Tests driver draw functions which didn't get catched by some other tests already

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
IVideoDriver* driver = 0;
ISceneManager* smgr = 0;
IGUIEnvironment* guienv = 0;
ITexture* textureAxe512x512 = 0;

void one_iter()
{
    if(!device->run()) 
	{
        emscripten_cancel_main_loop();
        return;
    }
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(0,200,200,200));

	for ( int i=1; i<90; i+=5 )
		driver->draw2DLine( core::position2d<s32>(i,i), core::position2d<s32>(100+i,20+i*2), video::SColor(255, 127+10*i, 255, 255)) ;
	
	for ( int i=1; i<200; i+= 3 )
		driver->drawPixel(i, 50+i, video::SColor(255, 255, 100 + i*5, 50 + i*10));
	
	driver->draw2DLine( core::position2d<s32>(10,10), core::position2d<s32>(150, 100), video::SColor(255, 127, 255, 127));
	driver->draw2DImage(textureAxe512x512, core::rect<s32>(0,200, 256, 456), core::rect<s32>(0,0,512,512), 0, 0, true);
	
	driver->draw2DImage(textureAxe512x512, core::rect<s32>(300,200, 400, 300), core::rect<s32>(0,0,100,100), 0, 0, false);
	
	
	core::array<core::rect<s32> > sourceRects;
	core::array<s32> indices;
	for ( int i=0; i<8; ++i )
	{
		sourceRects.push_back(core::rect<s32>(0+32*i,0, 32+32*i, 200));
		indices.push_back(i);
	}
	driver->draw2DImageBatch(textureAxe512x512,  core::position2d<s32>(200, 5), sourceRects, indices);

	smgr->drawAll(); // for camera

	/*
	video::SMaterial mat;
	mat.Lighting = false;
	driver->setMaterial(mat);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

	driver->draw3DLine(core::vector3df(-10, 0, 0), core::vector3df(30,20,10), video::SColor(255, 255, 0, 0));
	driver->draw3DLine(core::vector3df(0, -10, 0), core::vector3df(20,10,0), video::SColor(255, 0, 255, 0));
	*/
    
//    guienv->drawAll();

    driver->endScene();
}

int main()
{
	//device = createDevice(video::EDT_OGLES2, dimension2d<u32>(640, 480), 16, false, false, false, 0);
	device = createDevice(video::EDT_WEBGL1, dimension2d<u32>(640, 480), 16, false, false, false, 0);

	if (!device)
		return 1;

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	
	const io::path mediaPath = getExampleMediaPath();
	
	textureAxe512x512 = driver->getTexture(mediaPath + "axe.jpg");
	smgr->addCameraSceneNode(0, vector3df(0,0,-100), vector3df(0,0,0));

	int fps = 0;	
	int simulate_infinite_loop = 1;
	emscripten_set_main_loop(one_iter, fps, simulate_infinite_loop);

	return 0;
}
