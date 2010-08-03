/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkBlenderSkyBox_h_
#define _gkBlenderSkyBox_h_

#include "OgreResource.h"
#include "OgrePixelFormat.h"

namespace Blender
{
class World;
};

// simple skybox creator
class gkSkyBoxGradient : public Ogre::ManualResourceLoader
{
protected:

	Blender::World *m_world;

	void fill(Ogre::Image &ima, int size, const Ogre::ColourValue &v, const Ogre::PixelFormat &fmt);
	void grad(Ogre::Image &ima, int x1, int y1, int x2, int y2, int size,
	          const Ogre::ColourValue &s, const Ogre::ColourValue &e, const Ogre::PixelFormat &fmt);

public:
	gkSkyBoxGradient(Blender::World *wo);
	virtual ~gkSkyBoxGradient();

	void loadResource(Ogre::Resource *resource);
};

#endif//_gkBlenderSkyBox_h_