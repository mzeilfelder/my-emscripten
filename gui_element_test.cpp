// Code is under the zlib license (same as Irrlicht)
// Written by Michael Zeilfelder
//
// Tests with pretty much all Irrlicht gui elements.

#include <irrlicht.h>
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

enum
{
	GUI_ID_CHECKBOX_VISIBLE_PARENT,
	GUI_ID_CHECKBOX_VISIBLE_ELEMENTS,
	GUI_ID_CHECKBOX_ENABLED_PARENT,
	GUI_ID_CHECKBOX_ENABLED_ELEMENTS,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
	GUI_ID_BUTTON_CLEAR,
	GUI_ID_BUTTON_SAVE,
	GUI_ID_BUTTON_LOAD,
	GUI_ID_BUTTON_CREATE_ELEMENTS
};


struct SAppContext
{
	IrrlichtDevice * device;
	IGUIElement* mGuiParent;
	array<IGUIElement*> mGuiElements;
};


void ClearAllTestGuiElements(SAppContext & context)
{
	const core::list<IGUIElement*>& children = context.mGuiParent->getChildren();
	while (!children.empty())
		(*children.getLast())->remove();
	context.mGuiElements.clear();
}

void SetTabStopsForAllElements(SAppContext & context)
{
    for ( u32 i=0; i < context.mGuiElements.size(); ++i )
    {
        context.mGuiElements[i]->setTabStop(true);
        context.mGuiElements[i]->setTabOrder((s32)i);
    }
}

void AddTestGuiElements(IGUIEnvironment* env, IGUIElement * parent, SAppContext & context)
{
	context.mGuiElements.push_back( env->addToolBar (parent, /*s32 id=*/-1) );
	
	s32 top = 40;
	s32 default_height = 15;
	s32 default_width = 150;
	s32 default_gap = 20;
	core::rect<s32> rect(10, top, 10 + default_width, top + default_height);
	
	IGUIButton * button = env->addButton (rect, parent, /*s32 id=*/-1, /*const wchar_t *text=*/L"button", /*const wchar_t *tooltiptext=*/L"tooltip");
	context.mGuiElements.push_back(button);
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;
	IGUICheckBox * checkbox = env->addCheckBox (/*bool checked*/true, rect, parent, /*s32 id=*/-1, /*const wchar_t *text=*/L"checkbox");
	checkbox->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(checkbox);
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;
	IGUIComboBox * combo = env->addComboBox (rect, parent, /*s32 id=*/-1);
	for ( int i = 0; i < 100; ++i)
	{
		core::stringw wstr(i);
		wstr += L"Alpha";
		combo->addItem(wstr.c_str());
		wstr += L"-Beta";
		combo->addItem(wstr.c_str());
	}
	combo->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back( combo );
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 2* default_height;
	IGUIEditBox * multiLineEditBox = env->addEditBox (/*const wchar_t *text*/L"editbox", rect, /*bool border=*/true, parent, /*s32 id=*/-1);
	multiLineEditBox->setMultiLine(true);
	multiLineEditBox->setWordWrap(true);
	multiLineEditBox->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back( multiLineEditBox );
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;	
	IGUIEditBox * editboxnotext = env->addEditBox (/*const wchar_t *text*/L"", rect, /*bool border=*/true, parent, /*s32 id=*/-1);
	editboxnotext->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(editboxnotext);
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height*3;	
	IGUIListBox * lb = env->addListBox (rect, parent, /*s32 id=*/-1, /*bool drawBackground=*/true);
	for ( int i = 0; i < 100; ++i)
	{
		core::stringw wstr(i);
		wstr += L"Alpha";
		lb->addItem(wstr.c_str());
		wstr += L"-Beta";
		lb->addItem(wstr.c_str());
	}
	lb->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back( lb );	
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;	
	IGUIScrollBar * scrollbar = env->addScrollBar (/*bool horizontal*/true, rect, parent, /*s32 id=*/-1);
	scrollbar->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(scrollbar);
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;	
	IGUISpinBox * spinbox = env->addSpinBox (/*const wchar_t *text*/L"0", rect, /*bool border=*/true, parent, /*s32 id=*/-1);
	spinbox->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(spinbox);
	
	rect.UpperLeftCorner.X = 10 + default_width + default_gap;
	rect.LowerRightCorner.X = rect.UpperLeftCorner.X + default_width;
	rect.UpperLeftCorner.Y = top;
	rect.LowerRightCorner.Y = top + default_height;	
	IGUIStaticText * statictext = env->addStaticText (/*const wchar_t *text*/L"static", rect, /*bool border=*/false, /*bool wordWrap=*/true, parent, /*s32 id=*/-1,/* bool fillBackground=*/true);
	statictext->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(statictext);
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height;	
	IGUIStaticText * statictextnotext = env->addStaticText (/*const wchar_t *text*/L"", rect, /*bool border=*/true, /*bool wordWrap=*/true, parent, /*s32 id=*/-1,/* bool fillBackground=*/true);
	statictextnotext->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(statictextnotext);	// no text
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 4*default_height;
	IGUITabControl * tabctrl = env->addTabControl (rect, parent, /*bool fillbackground=*/true, /*bool border=*/true, /*s32 id=*/-1);
	tabctrl->addTab(/*const wchar_t *caption*/L"tab1", /*s32 id=*/-1);
	tabctrl->addTab(/*const wchar_t *caption*/L"tab2", /*s32 id=*/-1);
	tabctrl->addTab(/*const wchar_t *caption*/L"tab3", /*s32 id=*/-1);
	tabctrl->addTab(/*const wchar_t *caption*/L"tab4", /*s32 id=*/-1);
	tabctrl->addTab(/*const wchar_t *caption*/L"tab5", /*s32 id=*/-1);
	tabctrl->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back( tabctrl );
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + default_height*5;	
	IGUITable * table = env->addTable (rect, parent, /*s32 id=*/-1, /*bool drawBackground=*/true);
	table->addColumn(L"col1");
	table->addColumn(L"col2");
	table->addColumn(L"col3");
	for ( int i=0; i<10; ++i )
		table->addRow(i);
	table->setCellText(/*u32 rowIndex*/1, /*u32 columnIndex*/1, /*const wchar_t *text*/L"text");
	table->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(table );
	
	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 4*default_height;	
	IGUIWindow * window = env->addWindow (rect, /*bool modal=*/false, /*const wchar_t *text=*/L"window long titletext for colortesting", parent, /*s32 id=*/-1);
	window->setToolTipText(L"tooltip"); 
	context.mGuiElements.push_back(window);	
    
    rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y + default_gap;
    rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + 4*default_height;
    bool treeViewBackground = false;
    bool treeScrollBarVertical=true;
    bool treeScrollBarHorizontal=true;
    IGUITreeView * tree = env->addTreeView(rect, parent, -1, treeViewBackground, treeScrollBarVertical, treeScrollBarHorizontal);
    IGUITreeViewNode * treeNode = tree->getRoot();
    for ( int i=0; i < 10; ++i )
        treeNode->addChildBack(L"child");
    for ( int i=0; i < 10; ++i )
    {
        treeNode = treeNode->addChildBack(L"branch");
        treeNode->setExpanded(true);
    }
    context.mGuiElements.push_back(tree);	

    IGUIContextMenu * contextMenu =	env->addMenu(parent);
    contextMenu->addItem(L"File", -1, false, true);
    contextMenu->getSubMenu (0)->addItem(L"One", -1, false, true);
    contextMenu->getSubMenu (0)->addItem(L"Two", -1, true, true);
	contextMenu->addItem(L"Options", -1, true, true);
	contextMenu->getSubMenu (1)->addItem(L"One", -1, false, true);
    contextMenu->getSubMenu (1)->addItem(L"Two", -1, true, true);
	contextMenu->addItem(L"Help", -1, true, true);
	contextMenu->getSubMenu (2)->addItem(L"One", -1, false, true);
    contextMenu->getSubMenu (2)->addItem(L"Two", -1, true, true);
	context.mGuiElements.push_back(contextMenu);	
}

void AddControlElements(IGUIEnvironment* env, IGUIElement * parent)
{
	env->addStaticText (L"", rect<s32>(500, 50, 630, 480), /*bool border=*/true, /*bool wordWrap=*/false, parent, -1,/* bool fillBackground=*/false);
	env->addCheckBox (true, rect<s32>(510, 60, 620, 80), parent, GUI_ID_CHECKBOX_VISIBLE_PARENT, L"parent visible");
	env->addCheckBox (true, rect<s32>(510, 100, 620, 120), parent, GUI_ID_CHECKBOX_ENABLED_PARENT, L"parent enabled");
	env->addCheckBox (true, rect<s32>(510, 140, 620, 160), parent, GUI_ID_CHECKBOX_VISIBLE_ELEMENTS, L"elements visible");
	env->addCheckBox (true, rect<s32>(510, 180, 620, 200), parent, GUI_ID_CHECKBOX_ENABLED_ELEMENTS, L"elements enabled");

	env->addStaticText(L"Transparent Control:", rect<s32>(510,220,620,240), true);
	IGUIScrollBar* scrollbar = env->addScrollBar(true, rect<s32>(510, 250, 620, 270), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());	// set scrollbar position to alpha value of an arbitrary element

	env->addButton(rect<s32>(510, 280, 620, 300), parent, GUI_ID_BUTTON_CLEAR, L"clear", L"remove gui elements");
	env->addButton(rect<s32>(510, 310, 620, 330), parent, GUI_ID_BUTTON_SAVE, L"save", L"save gui");
	env->addButton(rect<s32>(510, 340, 620, 360), parent, GUI_ID_BUTTON_LOAD, L"load", L"clear & load gui");
	env->addButton(rect<s32>(510, 370, 620, 390), parent, GUI_ID_BUTTON_CREATE_ELEMENTS, L"create elements", L"clear & create new");
}

// move the children from source to target
void MoveChildren(irr::gui::IGUIElement* target, irr::gui::IGUIElement* source)
{
	core::list<IGUIElement*> children = source->getChildren();	// note - no reference, we need a copy here
	for ( core::list<IGUIElement*>::Iterator it = children.begin(); it != children.end();  ++it )
	{
		target->addChild(*it);
	}
}

void SaveTextGuiElements(SAppContext & context)
{
	// That line would save the complete gui (for testing to compare results)
	// context.device->getGUIEnvironment()->saveGUI("gui_all_elements.xml");

	context.device->getGUIEnvironment()->saveGUI("gui_all_elements.xml", context.mGuiParent);
}

void LoadTextGuiElements(SAppContext & context)
{
	context.device->getGUIEnvironment()->loadGUI("gui_all_elements.xml", context.mGuiParent);
	if ( !context.mGuiParent->getChildren().empty() )
	{
		// Workaround because I can't load _into_ an element so far in Irrlicht. So I have now one more copy of 
		// context.mGuiParent below context.mGuiParent which I have to remove again.
		// Irrlicht might change that on loading some day with another paramter (can't easily change it on saving 
		// without changing the format as xml-files need a single root).
		// First element is our dummy, remove that from the hierarchy.
		IGUIElement * dummy = *(context.mGuiParent->getChildren().begin());
		MoveChildren(context.mGuiParent, dummy);
		dummy->remove();
	}
}

class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

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
						case GUI_ID_BUTTON_CLEAR:
							ClearAllTestGuiElements(Context);
							break;
						case GUI_ID_BUTTON_SAVE:
							SaveTextGuiElements(Context);
							break;
						case GUI_ID_BUTTON_LOAD:
							ClearAllTestGuiElements(Context);
							LoadTextGuiElements(Context);
							break;
						case GUI_ID_BUTTON_CREATE_ELEMENTS:
							ClearAllTestGuiElements(Context);
							AddTestGuiElements(Context.device->getGUIEnvironment(), Context.mGuiParent, Context);
							break;
					}
				}
				break;
				case EGET_CHECKBOX_CHANGED:
				{
					IGUICheckBox *cb = static_cast<IGUICheckBox *>(event.GUIEvent.Caller);
					switch ( id )
					{
						case GUI_ID_CHECKBOX_VISIBLE_PARENT:
							Context.mGuiParent->setVisible( cb->isChecked() );
							break;
						case GUI_ID_CHECKBOX_VISIBLE_ELEMENTS:
							for ( u32 i=0; i < Context.mGuiElements.size(); ++i )
							{
								Context.mGuiElements[i]->setVisible( cb->isChecked() );
							}
							break;
						case GUI_ID_CHECKBOX_ENABLED_PARENT:
							Context.mGuiParent->setEnabled( cb->isChecked() );
							break;
						case GUI_ID_CHECKBOX_ENABLED_ELEMENTS:
							for ( u32 i=0; i < Context.mGuiElements.size(); ++i )
							{
								Context.mGuiElements[i]->setEnabled( cb->isChecked() );
							}
							break;
					default:
						break;
					}
					break;
				}
				case EGET_SCROLL_BAR_CHANGED:
				{
					if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
					{
						s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
						
						for (u32 i=0; i<EGDC_COUNT ; ++i)
						{
							IGUIEnvironment * env = Context.device->getGUIEnvironment();
							SColor col = env->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
							col.setAlpha(pos);
							env->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, col);
						}
						
					}
					break;
				}
			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};

void one_iter()
{
    if(!device->run()) 
	{
        emscripten_cancel_main_loop();
        return;
    }
    driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(0,200,200,200));

    smgr->drawAll();
    guienv->drawAll();

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

	SAppContext context;
	context.device = device;
	
	context.mGuiParent = guienv->addWindow(irr::core::recti(0,0,500,450), false, L"sometext");

	AddTestGuiElements(guienv, context.mGuiParent, context);
	SetTabStopsForAllElements(context);
	
	MyEventReceiver receiver(context);
	device->setEventReceiver(&receiver);
	
	AddControlElements(guienv, 0);
	
	int fps = 0;	
	int simulate_infinite_loop = 1;
	emscripten_set_main_loop(one_iter, fps, simulate_infinite_loop);

	return 0;
}
