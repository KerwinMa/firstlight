#ifndef FLTDEMO_LEVEL_H
#define FLTDEMO_LEVEL_H

#include "firstlight.h"

using namespace flt;
using namespace utils;
using namespace renderer;
using namespace ui;

class Ball;

extern UITouchZone* g_fullScreenZone;

class Level
{
public:
	Level();

	~Level();	

	bool loadWorld(const stringc& levelXMLFile);

	void releaseLevel();

	void update(f32 dt);
	void render(const renderer::IRendererPtr& renderer);	
	void postUpdate(f32 dt);

	phy2d::Phy2dWorld* getWorld() { return m_world; }

	void resetLevel();	
	
	bool processEvent(const flt::IEvent& event); 

private:
		
	phy2d::Phy2dWorld *m_world;
	Ball *m_playerBall;	
};

#endif //FLTDEMO_LEVEL_H
