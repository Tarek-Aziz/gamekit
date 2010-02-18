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

#include "gkArcBallNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkWindowSystem.h"
#include "OgreRenderWindow.h"
#include "OgreRoot.h"
#include "gkLogger.h"

using namespace Ogre;

gkArcBallNode::gkArcBallNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_rayQuery(0),
m_RotationNode(0),
m_gameObj(0),
m_scene(0),
m_center(Ogre::Vector3::ZERO),
m_centerUpdated(false),
m_positionUpdated(false)
{
	ADD_ISOCK(*getUpdateCenter(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getUpdatePosition(), this, gkLogicSocket::ST_BOOL);

	ADD_ISOCK(*getX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getY(), this, gkLogicSocket::ST_REAL);

	ADD_ISOCK(*getRelX(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getRelY(), this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(*getRelZ(), this, gkLogicSocket::ST_REAL);
}

gkArcBallNode::~gkArcBallNode()
{
	if(m_rayQuery)
		m_scene->getManager()->destroyQuery(m_rayQuery);

	if(m_RotationNode)
		delete m_RotationNode;
}

bool gkArcBallNode::evaluate(Real tick)
{
	m_centerUpdated = getUpdateCenter()->getValueBool();

	m_positionUpdated = getUpdatePosition()->getValueBool();

	return m_centerUpdated || m_positionUpdated;
}

void gkArcBallNode::update(Real tick)
{
	if(!m_RotationNode)
	{
		m_gameObj = getAttachedObject();

		GK_ASSERT(m_gameObj);

		m_scene = m_gameObj->getOwner();

		GK_ASSERT(m_scene);

		m_rayQuery = m_scene->getManager()->createRayQuery(Ray());

		m_rayQuery->setSortByDistance(true);

		GK_ASSERT(m_rayQuery);

		m_RotationNode = new Ogre::SceneNode(m_scene->getManager());

		GK_ASSERT(m_RotationNode);
		
		m_RotationNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);
	}

	if(m_centerUpdated)
	{
		GetNewCenter();
	}

	SetNewPosition();
}

void gkArcBallNode::GetNewCenter()
{
	gkCamera* pCamera = m_scene->getMainCamera();

	GK_ASSERT(pCamera);

	Vector2 pos(getX()->getValueReal(), getY()->getValueReal());

	gkWindowSystem* pWindowSystem = gkWindowSystem::getSingletonPtr();

	Real width = pWindowSystem->getMainWindow()->getWidth();

	Real height = pWindowSystem->getMainWindow()->getHeight();

	GK_ASSERT(width && height);

	Ray ray(pCamera->getCamera()->getCameraToViewportRay(pos.x/width, pos.y/height));

	Vector3 p0 = ray.getOrigin();

	const Real MAX_DISTANCE = 1000000;

	Vector3 p1 = p0 + ray.getDirection() * MAX_DISTANCE;

	ray.setDirection(p1-p0);

	m_rayQuery->setRay(ray);

	const RaySceneQueryResult& result = m_rayQuery->execute();

	RaySceneQueryResult::const_iterator it = result.begin();

	while(it != result.end())
	{
		const MovableObject* pObj = it->movable;

		if(it->distance > 1)
		{
			// point is outside the segment(p0,p1)
			// because query is sorted by distance then following objects will be far away...
			break;
		}

		if(it->distance > 0 && pObj && pObj->isVisible())
		{
			m_center = ray.getPoint(it->distance);

			break;
		}

		++it;
	}
}

void gkArcBallNode::SetNewPosition()
{
	gkGameObject* pGameObj = getAttachedObject();

	GK_ASSERT(pGameObj);

	Ogre::Vector3 currentPosition = pGameObj->getPosition();

	Ogre::Vector3 dir = m_center - currentPosition;

	m_RotationNode->resetOrientation();

	m_RotationNode->setDirection(dir);

	Ogre::Quaternion q = m_RotationNode->getOrientation();

	if(m_positionUpdated)
	{
		Ogre::Vector3 offset(getRelX()->getValueReal(), getRelY()->getValueReal(), getRelZ()->getValueReal());

		Vector3 axis(offset.y, offset.x, 0);

		Angle angle(axis.normalise());

		m_RotationNode->rotate(axis, angle, Ogre::Node::TS_LOCAL);

		q = m_RotationNode->getOrientation();

		Real distance = dir.length();

		Vector3 oDir = Ogre::Vector3::NEGATIVE_UNIT_Z * distance;

		if(offset.z)
		{
			oDir += oDir * offset.z * 0.5;
		}

		const Real MIN_DISTANCE = 0.01;

		if(oDir.length() > MIN_DISTANCE)
		{
			Vector3 newPosition = m_center - q * oDir;

			pGameObj->setPosition(newPosition);
		}
	}

	pGameObj->setOrientation(q);
}
