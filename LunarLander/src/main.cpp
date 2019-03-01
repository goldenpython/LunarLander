#include <irrlicht.h>
#include "driverChoice.h"

#include <Windows.h>
#include "IrrGraphics.h"
#include "CLunarModule.h"
#include "CTerrain.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

void DrawInterface(IGraphics *poGraphics, gui::IGUIFont* poFont, CLunarModule *poLunarModule, CTerrain *poTerrain) {
	const int nLifePosX = 20, nLifePosY = 10, nLifeWidth = 100, nLifeHeight = 12;
	poGraphics->SetColor(128, 0, 0);
	poGraphics->FillRect(nLifePosX, nLifePosY, nLifeWidth, nLifeHeight);

	poGraphics->SetColor(0, 255, 0);
	int nCurrentLifeWidth = 0;
	if (poLunarModule->GetLifePercent() > 0)
		nCurrentLifeWidth = (int)((nLifeWidth - 4) * poLunarModule->GetLifePercent());
	poGraphics->FillRect(nLifePosX + 2, nLifePosY + 2, nCurrentLifeWidth, nLifeHeight - 4);

	const int nLifeIconCenterX = nLifePosX / 2, nLifeIconCenterY = nLifePosY + nLifeHeight / 2, nLifeIconWH = nLifeHeight;
	poGraphics->SetColor(255, 255, 255);
	poGraphics->FillCenteredRect(nLifeIconCenterX, nLifeIconCenterY, nLifeIconWH, nLifeIconWH);
	poGraphics->SetColor(255, 0, 0);
	poGraphics->FillCenteredRect(nLifeIconCenterX, nLifeIconCenterY, nLifeIconWH / 2 - 2, nLifeIconWH - 2);
	poGraphics->FillCenteredRect(nLifeIconCenterX, nLifeIconCenterY, nLifeIconWH - 2, nLifeIconWH / 2 - 2);

	const int nWindCenterX = 40, nWindCenterY = 30, nWindMaxW = nWindCenterX - 4, nArrowW = 5;
	float fWindSpeed = poTerrain->GetWindSpeed() / MAX_WIND_SPEED;
	poGraphics->SetColor(255, 0, 0);
	int nArrowX = (int)(nWindCenterX + fWindSpeed * nWindMaxW);
	poGraphics->DrawLine(nWindCenterX, nWindCenterY, nArrowX, nWindCenterY);
	if (fWindSpeed < 0) {
		poGraphics->DrawLine(nArrowX, nWindCenterY, nArrowX + nArrowW, nWindCenterY + nArrowW);
		poGraphics->DrawLine(nArrowX, nWindCenterY, nArrowX + nArrowW, nWindCenterY - nArrowW);
	} else {
		poGraphics->DrawLine(nArrowX, nWindCenterY, nArrowX - nArrowW, nWindCenterY + nArrowW);
		poGraphics->DrawLine(nArrowX, nWindCenterY, nArrowX - nArrowW, nWindCenterY - nArrowW);
	}

	if (poFont) {
		int nMessagePosX = 20, nMessagePosY = 40;
		poFont->draw(L"Press W to reset state. Press LEFT, RIGHT to activate engines. Press SPACE to activate both engines.", core::rect<s32>(nMessagePosX, nMessagePosY, poGraphics->GetScreenWidth() / 4, poGraphics->GetScreenHeight()), video::SColor(255, 127, 127, 255));

		nMessagePosY += 20;
		switch (poLunarModule->GetState()) {
			case CLunarModule::STATE_LANDED:
 				poFont->draw(L"You have landed.", core::rect<s32>(nMessagePosX, nMessagePosY, poGraphics->GetScreenWidth(), poGraphics->GetScreenHeight()), video::SColor(255, 255, 255, 255));
				break;

			case CLunarModule::STATE_DEAD:
 				poFont->draw(L"You crashed the module.", core::rect<s32>(nMessagePosX, nMessagePosY, poGraphics->GetScreenWidth(), poGraphics->GetScreenHeight()), video::SColor(255, 255, 255, 255));
				break;
		}
	}
}

/*
At first, we let the user select the driver type, then start up the engine, set
a caption, and get a pointer to the video driver.
*/
int main()
{
	// ask user for driver
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if (driverType==video::EDT_COUNT)
		return 1;

	MyEventReceiver receiver;

	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const float MAP_WIDTH = WINDOW_WIDTH * 5, MAP_HEIGHT = WINDOW_HEIGHT;
	const float TERRAIN_SEGMENT_SIZE = 20.0f;

	// create device

	IrrlichtDevice *device = createDevice(driverType,
		core::dimension2d<u32>(WINDOW_WIDTH, WINDOW_HEIGHT),16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");

	video::IVideoDriver* driver = device->getVideoDriver();

	/*
	To be able to draw some text with two different fonts, we first load
	them. Ok, we load just one. As the first font we just use the default
	font which is built into the engine. Also, we define two rectangles
	which specify the position of the images of the red imps (little flying
	creatures) in the texture.
	*/
	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();

	/*
	Prepare a nicely filtering 2d render mode for special cases.
	*/
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	u32 then = device->getTimer()->getTime();

	
	IGraphics *poGraphics = new IrrGraphics(driver);
	CLunarModule oLunarModule;
	CTerrain oTerrain(MAP_WIDTH, MAP_HEIGHT, TERRAIN_SEGMENT_SIZE);
	oLunarModule.SetTerrain(&oTerrain);
	CVector2D<float> oCameraPos;

	/*
	Everything is prepared, now we can draw everything in the draw loop,
	between the begin scene and end scene calls. In this example, we are
	just doing 2d graphics, but it would be no problem to mix them with 3d
	graphics. Just try it out, and draw some 3d vertices or set up a scene
	with the scene manager and draw it.
	*/
	while(device->run() && driver)
	{
		if (device->isWindowActive())
		{
			u32 time = device->getTimer()->getTime();

			driver->beginScene(true, true, video::SColor(255, 68, 6, 151));



			DrawInterface(poGraphics, font, &oLunarModule, &oTerrain);


			const u32 now = device->getTimer()->getTime();
			const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
			then = now;

			if(receiver.IsKeyDown(irr::KEY_KEY_W) /* || oLunarModule.GetLifePercent() < 0 */) {
				oLunarModule = CLunarModule();
				oTerrain.Generate();
				oLunarModule.SetTerrain(&oTerrain);
			}

			static float fCameraOffsetX = 0, fCameraOffsetY = 0;

			bool bEngineLeftActive = receiver.IsKeyDown(irr::KEY_LEFT) || receiver.IsKeyDown(irr::KEY_SPACE);
			bool bEngineRightActive = receiver.IsKeyDown(irr::KEY_RIGHT) || receiver.IsKeyDown(irr::KEY_SPACE);

			oLunarModule.Update(frameDeltaTime, bEngineLeftActive, bEngineRightActive);
			oTerrain.Update(frameDeltaTime);

			oCameraPos.x = oLunarModule.GetPosX() - WINDOW_WIDTH / 2;
			oCameraPos.y = 0;
			

			oLunarModule.Draw(poGraphics, bEngineLeftActive, bEngineRightActive, oCameraPos);

			oTerrain.Draw(poGraphics, oCameraPos);

			driver->endScene();

			Sleep(10);
		}
	}

	device->drop();

	return 0;
}
