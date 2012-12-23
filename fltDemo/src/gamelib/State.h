#ifndef XLIB_ENGINE_STATE_H
#define XLIB_ENGINE_STATE_H

#include "firstlight.h"

template <class entity_type>
class State
{
public:
	State(){}
	virtual ~State(){}

	//executed when entity enter this state
	virtual void onEnter(entity_type*) = 0;

	//process event, return if absorb the event
	virtual bool processEvent(entity_type*, const flt::IEvent& event) = 0;

	//entity update this state
	virtual void update(entity_type*, float dt) = 0;

	//entity post update this state
	virtual void postUpdate(entity_type*, float dt) = 0;

	//entity render in this state
	virtual void render(entity_type*, const flt::renderer::IRendererPtr& renderer) = 0;

	//executed when entity exit this state
	virtual void onExit(entity_type*) = 0;

};


#endif //XLIB_ENGINE_STATE_H