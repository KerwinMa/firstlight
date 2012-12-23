#ifndef FLT_TERRIAN_TEST_H
#define FLT_TERRIAN_TEST_H

#include "../ITestMod.h"
#include "terrian/terrian.h"
#include "renderer/Camera.h"
#include "3ddemo/gird.h"
#include "3ddemo/mesh.h"

using namespace flt::core;
using namespace flt::renderer;

class TerrianTest: public ITestMod, public flt::IEventReceiver
{
public:
	TerrianTest(const device::IDevicePtr& devicePtr)
		:ITestMod(devicePtr),m_pGrid(0)
	{
		EventManager& eventMgr = EventManager::getInstance();
		eventMgr.registerReceiver(events_id::EVT_TOUCH, this);

		//camera control
		m_isDraging = false;
		m_lastTouchX = m_lastTouchY = 0;

		//camera
		m_camera = devicePtr->getRenderer()->getCamera();

		m_camera->setPosition(vector3df(0,-100,100));
		m_camera->setTarget(vector3df(0,0,0));
		m_camera->setUpDirection(vector3df(0,0,1));

		
	}

	virtual ~TerrianTest()
	{
		delete m_pTerrian;

		delete m_pGrid;

		delete m_pMesh;
	}

	virtual bool init()
	{
		m_pTerrian = new Terrian();
		m_pTerrian->loadHeightMap("terrian/height64.raw",64);
		m_pTerrian->setXYScale(10.0f);
		m_pTerrian->setHeightScale(1.0f);
		m_pTerrian->buildMesh();
		
		m_pGrid = new Grid(10,10,100);
		m_pGrid->buildMesh();

		m_pMesh = new Mesh();
		m_pMesh->create();
		return true;
	}

	virtual void destroy()
	{
	}


	virtual bool onEvent(const IEvent& event)
	{
		if(event.getUID()==events_id::EVT_TOUCH)
		{

			EvtTouch* touch = (EvtTouch*)&event;
			if(touch->Phase==Touch_Began)
			{
				m_isDraging = true;
				m_lastTouchX = touch->X;
				m_lastTouchY = touch->Y;
			}
			else if(touch->Phase==Touch_Moved)
			{
				s16 deltaX = touch->X - m_lastTouchX;
				s16 deltaY = touch->Y - m_lastTouchY;

				//rotateCamera(deltaX, deltaY);
				moveCamera(deltaX,deltaY);

				m_lastTouchX = touch->X;
				m_lastTouchY = touch->Y;
			}
			else
			{
				m_isDraging = false;
			}
		}

		return false;
	}

	virtual void update(float dt)
	{
		if(!m_enable)
			return;
	}

	virtual void render(const renderer::IRendererPtr& renderer)
	{
		if(!m_enable)
			return;

		//core::matrix4 mat;
		//mat.setTranslation(core::vector3df(0,0,m_transZ));
		//renderer->setWorldTransform(mat);

		m_pMesh->render(renderer);

		m_pGrid->render(renderer);

		
		
		//m_pTerrian->render(renderer);
	}

protected:
	void moveCamera(s16 deltaX, s16 deltaY)
	{
		core::vector3df pos = m_camera->getPosition();
		vector3df dir = m_camera->getDirection();

		pos.X += deltaX;
		pos.Y += deltaY;
		m_camera->setPosition(pos);
		m_camera->setTarget(pos+dir);
	}

	void rotateCamera(s16 deltaX, s16 deltaY)
	{
		quaternion qYaw, qPitch; 
		qYaw.fromAngleAxis(deltaX*0.01f, m_camera->getUp());
		qPitch.fromAngleAxis(deltaY*0.01f,m_camera->getRight());

		vector3df dir = m_camera->getDirection();
		dir = qYaw * dir;
		dir = qPitch * dir;
		m_camera->setTarget(m_camera->getPosition()+dir);

		//m_transZ+=deltaY;
		/*vector3df target = m_camTarget;
		vector3df pos = m_camPos;
		vector3df up = m_camUp;
		up.normalize();
		vector3df lookAt = target - pos;
		lookAt.normalize();
		vector3df right = up.crossProduct(lookAt);
		right.Z = 0;
		right.normalize();

		f32 RotateSpeed = 1;
		float rotZ = 0;
		if (deltaX != 0)
		{
			rotZ = RotateSpeed / 10 * deltaX;
		}

		float rotX = 0;
		if (deltaY != 0)
		{
			rotX = RotateSpeed / 10 * deltaY;
		}

		quaternion qZ; qZ.fromAngleAxis(rotZ, up);
		quaternion qX; qX.fromAngleAxis(rotX, right);

		quaternion q = qX * qZ;

		vector3df toPos = target - pos;
		toPos = q * toPos;
		m_camTarget = pos + toPos;
		vector3df newRight = q * right;
		newRight.Z = 0;
		vector3df newUp = toPos.crossProduct(right);
		m_camUp = newUp;*/
	}

private:
	renderer::VertexArray m_testQuards, m_testQuards2;

	Terrian *m_pTerrian;

	Grid *m_pGrid;
	Mesh *m_pMesh;

	bool m_isDraging;
	s16 m_lastTouchX, m_lastTouchY;

	CameraPtr m_camera;
};

#endif //FLT_TERRIAN_TEST_H

