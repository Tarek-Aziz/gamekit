/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier Thomas.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#include "OgreKit.h"
#include "vdVehicle.h"

class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:

    OgreKit(const gkString &blend, const gkString& startCfg) 
		: m_blend(gkUtils::getFile(blend))
	{
		gkPath path = gkUtils::getFile(startCfg);

		if(path.isFile())
		{
			m_prefs.load(path.getPath());
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << startCfg);
		}
	}

	~OgreKit()
	{
		delete m_vehicle;
	}

	
	void tick(gkScalar rate)
	{
		m_vehicle->tick(rate);
	}

	bool load()
	{
		gkBlendFile* pBlendFile = m_engine->loadBlendFile(m_blend);

		if(pBlendFile)
		{
			gkScene* pScene = pBlendFile->getMainScene();

			GK_ASSERT(pScene);

			pScene->load();

			m_vehicle = new vdVehicle(pScene);
			
			gkTransformState startline;
			startline.setIdentity();
			startline.loc = gkVector3(-67.0, 289.0,-8.0);
			m_vehicle->setTransfrom(startline);
			
			// add input hooks
			gkWindowSystem::getSingleton().addListener(this);
			return true;
		}
		else
		{
			gkLogMessage("ERROR: FILE NOT FOUND ---> " << m_blend);
			return false;
		}
	}
	
	void keyPressed(const gkKeyboard &key, const gkScanCode &sc)
	{
		if (sc == KC_UPARROWKEY)
			m_vehicle->setGaz(1.0f);
		if (sc == KC_DOWNARROWKEY)
			m_vehicle->setBreak(1.0f);
		if (sc == KC_LEFTARROWKEY)
			m_vehicle->setSteer(1.0f);
		if (sc == KC_RIGHTARROWKEY)
			m_vehicle->setSteer(-1.0f);
	}
	void keyReleased(const gkKeyboard &key, const gkScanCode &sc)
	{
		if (sc == KC_ESCKEY)
			m_engine->requestExit();
			
			
		if (sc == KC_UPARROWKEY)
			m_vehicle->setGaz(0.0f);
		if (sc == KC_DOWNARROWKEY)
			m_vehicle->setBreak(0.0f);
		if (sc == KC_LEFTARROWKEY)
			m_vehicle->setSteer(0.0f);
		if (sc == KC_RIGHTARROWKEY)
			m_vehicle->setSteer(0.0f);
	}

private:

	gkString  m_blend;
	vdVehicle *m_vehicle;
};


int main(int argc, char **argv)
{
	// See ReadMe.txt for how to use this demo

	TestMemory;

	gkLogger::enable("VehicleDemo.log", true);

	OgreKit okit("clubsilo_demo.blend", "OgreKitStartup.cfg");

	okit.run();

	return 0;
}

