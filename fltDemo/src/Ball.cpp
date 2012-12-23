#include "Ball.h"
#include "Level.h"

Ball::Ball(phy2d::Phy2dActor* actor)
:m_playerActor(actor),m_state(EBS_Normal)
{
}

Ball::~Ball()
{
}

void Ball::update(f32 dt)
{
	//switch(m_state)
	//{
	//case EBS_Normal:
	//	{
	//		if(g_fullScreenZone->isHolding())
	//		{
	//			b2Body* body = m_playerActor->getBody();

	//			b2Vec2 vec = body->GetLinearVelocity();
	//			body->ApplyLinearImpulse(-vec, body->GetPosition());				
	//		}
	//	}
	//	break;
	//case EBS_InAir:
	//	{
	//		if(g_fullScreenZone->isHolding())
	//		{
	//			m_playerActor->applyForce(core::vector2df(0,-500));
	//		}
	//	}
	//	break;
	//}	
}

void Ball::onSlide(const core::vector2df& dir, f32 speed)
{
	////speed *= 5000.0f;
	//if(speed==0)
	//	return;
	//
	//speed = 35000.0f;

	//core::vector2df vec= dir*speed;
	//vec.Y*=-1;

	////if(core::abs_(dir.X)>=core::abs_(dir.Y))
	////{
	////	if(dir.X<0)
	////		vec.X = -speed;
	////	else if(dir.X>0) 
	////		vec.X = speed;
	////}
	////else
	////{
	////	if(dir.Y<0)
	////		vec.Y = speed;
	////	else if(dir.Y>0) 
	////		vec.Y = -speed;
	////}
	//	
	//m_playerActor->applyForce(vec);
}
