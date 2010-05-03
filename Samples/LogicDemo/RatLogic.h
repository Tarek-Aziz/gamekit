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

#ifndef _RatLogic_h_
#define _RatLogic_h_

#include "OgreKit.h"

class SceneLogic;
class MomoLogic;

class RatLogic : public gkReferences
{
	typedef gkPtrRef<MomoLogic> PMOMO;

public:
	RatLogic(const gkString& name, SceneLogic* scene, PMOMO momo);
	~RatLogic();

private:
	void CreateNodes();
	void CreatePlayer();
	void CreateAnimation();
	void CreatePathfinding();
	void CreateStateMachine();
	void CreateRayTest();

	gkRayTestNode* HasHit();
	
	typedef gkIfNode<int, CMP_EQUALS> NODE;
	typedef NODE* PNODE;
	PNODE MyCurrentStatusIs(int status);

private:
	gkString m_name;
	SceneLogic* m_scene;
	PMOMO m_momo;
	gkAnimationNode* m_animNode;
	gkFindPathNode* m_pathFindingNode;
	gkStateMachineNode* m_stateMachineNode;
	gkFollowPathNode* m_followPathNode;
	gkObjNode* m_playerNode;

	typedef std::map<int, PNODE> MAP;
	MAP m_statuses;
	gkRayTestNode* m_hasHit;


};

#endif//_RatLogic_h_