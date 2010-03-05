/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "gkWindowSystem.h"
#include "gkSetterNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogger.h"

gkSetterNode::gkSetterNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_hasOneSet(false)
{
	ADD_ISOCK(*getUpdate(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getInput(), this, gkLogicSocket::ST_STRING);
	ADD_ISOCK(*getJustOnce(), this, gkLogicSocket::ST_BOOL);
}

bool gkSetterNode::evaluate(gkScalar tick)
{
	return getUpdate()->getValueBool() && (!(getJustOnce()->getValueBool() && m_hasOneSet) || DoEvaluate());
}

void gkSetterNode::update(gkScalar tick)
{
	DoUpdate();

	m_hasOneSet = true;
}

/////////////////////////////////////////////
gkStringSetterNode::gkStringSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_STRING);
}

bool gkStringSetterNode::DoEvaluate()
{
	return false;
}

void gkStringSetterNode::DoUpdate()
{
	getOutput()->setValue(getInput()->getValueString());
}

/////////////////////////////////////////////

gkObjectSetterNode::gkObjectSetterNode(gkLogicTree *parent, size_t id)
	: gkSetterNode(parent, id)
{
	ADD_OSOCK(*getOutput(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_ISOCK(*getUnload(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getLoad(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getReload(), this, gkLogicSocket::ST_BOOL);
}

bool gkObjectSetterNode::DoEvaluate()
{
	return getReload()->getValueBool() || getLoad()->getValueBool() || getUnload()->getValueBool();
}

void gkObjectSetterNode::DoUpdate()
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	gkGameObject* pObj = pScene->getObject(getInput()->getValueString());

	if(getReload()->getValueBool())
	{
		pObj->reload();
	}
	else if(getLoad()->getValueBool())
	{
		pObj->load();
	}
	else if(getUnload()->getValueBool())
	{
		pObj->unload();
	}

	getOutput()->setValue(pObj);
}


