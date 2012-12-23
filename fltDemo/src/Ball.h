#ifndef FLTDEMO_BALL_H
#define FLTDEMO_BALL_H

#include "firstlight.h"

using namespace flt;
using namespace utils;
using namespace renderer;

enum EBallState
{
	EBS_Normal,
	EBS_InAir,
};

class Ball
{
public:
	Ball(phy2d::Phy2dActor* actor);
	~Ball();

	phy2d::Phy2dActor* getActor() 
	{
		return m_playerActor;
	}

	void update(f32 dt);

	void onSlide(const core::vector2df& dir, f32 speed);

private:
	phy2d::Phy2dActor *m_playerActor;

	EBallState m_state;
};

#endif //FLTDEMO_BALL_H