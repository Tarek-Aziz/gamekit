
#include "IrrBlendNew.h"

#include "autogenerated/blender.h"
#include "bMain.h"
#include "bFile.h"



IrrBlendNew::IrrBlendNew(irr::IrrlichtDevice* device,irr::scene::ISceneManager* irrlichtSceneManager, btDiscreteDynamicsWorld* bulletWorld, btLogicManager* logicManager)
:BulletBlendReaderNew(bulletWorld)
{

}


IrrBlendNew::~IrrBlendNew()
{

}


void	IrrBlendNew::convertLogicBricks()
{
}

void	IrrBlendNew::createParentChildHierarchy()
{
}
	
	//after each object is converter, including collision object, create a graphics object (and bind them)
void* IrrBlendNew::createGraphicsObject(Blender::Object* tmpObject, class btCollisionObject* bulletObject)
{
	return 0;
}

void	IrrBlendNew::addCamera(Blender::Object* tmpObject)
{

}

void	IrrBlendNew::addLight(Blender::Object* tmpObject)
{

}
