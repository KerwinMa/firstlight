#ifndef XLIB_ENGINE_STATEMACHINE_H
#define XLIB_ENGINE_STATEMACHINE_H

#include "firstlight.h"
#include "State.h"

#include <typeinfo>


template <class entity_type>
class StateMachine
{
public:
	explicit StateMachine(entity_type* pOwner)
		:m_pOwner(pOwner),
		 m_pCurrentState(NULL),
		 m_pPreviousState(NULL),
		 m_pGlobalState(NULL)
	{
	}

	virtual ~StateMachine(){}

	//set first state, will call OnEnter
	void setFirstState(State<entity_type>* pState)
	{
		m_pCurrentState = pState;
		m_pCurrentState->onEnter(m_pOwner);
	}

	//set global state
	void setGlobalState(State<entity_type>* pState)
	{
		m_pGlobalState = pState;
	}

	bool processEvent(const flt::IEvent& event)
	{
		bool absorb = false;

		if(m_pGlobalState)
			absorb = m_pGlobalState->processEvent(m_pOwner, event);

		if(!absorb && m_pCurrentState)
		{
			absorb = m_pCurrentState->processEvent(m_pOwner, event);
		}

		return absorb;
	}	

	//call this to update the FSM
	void update(float dt) const
	{
		if(m_pGlobalState)
			m_pGlobalState->update(m_pOwner, dt);

		if(m_pCurrentState)
			m_pCurrentState->update(m_pOwner, dt);
	}

	//call this to post update the FSM
	void postUpdate(float dt) const
	{
		if(m_pGlobalState)
			m_pGlobalState->postUpdate(m_pOwner, dt);

		if(m_pCurrentState)
			m_pCurrentState->postUpdate(m_pOwner, dt);
	}

	//call this to do render
	void render(const flt::renderer::IRendererPtr& renderer) const
	{
		if(m_pGlobalState)
			m_pGlobalState->render(m_pOwner, renderer);

		if(m_pCurrentState)
			m_pCurrentState->render(m_pOwner, renderer);
	}

	//change to a new state
	void changeState(State<entity_type>* pNewState)
	{
		FLT_ASSERT(pNewState!=NULL && "<StateMachine::ChangeState>: trying to change to NULL state>");

		//auto-set previous state
		m_pPreviousState = m_pCurrentState;

		//exit the previous state
		m_pCurrentState->onExit(m_pOwner);

		//change to the new state
		m_pCurrentState = pNewState;

		//on enter the new state
		m_pCurrentState->onEnter(m_pOwner);
	}

	//change state back to the previous state
	void revertToPreviousState()
	{
		FLT_ASSERT(m_pPreviousState!=NULL && "<StateMachine::RevertToPreviousState>: previous state is NULL>");
		changeState(m_pPreviousState);
	}

	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter.
	bool isInState(const State<entity_type>& st) const
	{
		return typeid(*m_pCurrentState) == typeid(st);
	}

	State<entity_type>*  getCurrentState()  const{return m_pCurrentState;}
	State<entity_type>*  getGlobalState()   const{return m_pGlobalState;}
	State<entity_type>*  getPreviousState() const{return m_pPreviousState;}

	
private:
	//a pointer to the owner entity of this FSM
	entity_type*	m_pOwner;

	State<entity_type>* m_pCurrentState;

	State<entity_type>* m_pPreviousState;

	State<entity_type>* m_pGlobalState;

};


#endif //XLIB_ENGINE_STATEMACHINE_H