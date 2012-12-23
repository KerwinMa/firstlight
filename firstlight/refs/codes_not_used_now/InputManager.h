#ifndef FLT_INPUT_MANAGER_H
#define FLT_INPUT_MANAGER_H

#include "externs/ois/includes/OISMouse.h"
#include "externs/ois/includes/OISKeyboard.h"
#include "externs/ois/includes/OISJoyStick.h"
#include "externs/ois/includes/OISInputManager.h"
#include "utils/ISingleton.h"
#include "sharePtr.h"


namespace flt
{

	namespace device
	{	
		FLT_FORWARD_PTR(IDevice)
	}

	class InputManager : public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener, public utils::ISingleton<InputManager>
	{
	public:
		InputManager( void );
		virtual ~InputManager( void );

		void init( const device::IDevicePtr& device );
		void capture( void );

		void addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName );
		void addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName );
		void addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName );

		void removeKeyListener( const std::string& instanceName );
		void removeMouseListener( const std::string& instanceName );
		void removeJoystickListener( const std::string& instanceName );

		void removeKeyListener( OIS::KeyListener *keyListener );
		void removeMouseListener( OIS::MouseListener *mouseListener );
		void removeJoystickListener( OIS::JoyStickListener *joystickListener );

		void removeAllListeners( void );
		void removeAllKeyListeners( void );
		void removeAllMouseListeners( void );
		void removeAllJoystickListeners( void );

		void setWindowExtents( int width, int height );

		OIS::Mouse*    getMouse( void );
		OIS::Keyboard* getKeyboard( void );
		OIS::JoyStick* getJoystick( unsigned int index );

		int getNumOfJoysticks( void );

	  
	private:    
		InputManager( const InputManager& ) { }
		InputManager & operator = ( const InputManager& );

		bool keyPressed( const OIS::KeyEvent &e );
		bool keyReleased( const OIS::KeyEvent &e );

		bool mouseMoved( const OIS::MouseEvent &e );
		bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

		bool povMoved( const OIS::JoyStickEvent &e, int pov );
		bool axisMoved( const OIS::JoyStickEvent &e, int axis );
		bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
		bool buttonPressed( const OIS::JoyStickEvent &e, int button );
		bool buttonReleased( const OIS::JoyStickEvent &e, int button );

		OIS::InputManager *m_pInputSystem;
		OIS::Mouse        *m_pMouse;
		OIS::Keyboard     *m_pKeyboard;

		std::vector<OIS::JoyStick*> m_pJoysticks;
		std::vector<OIS::JoyStick*>::iterator m_joystickIter;
		std::vector<OIS::JoyStick*>::iterator m_joystickIterEnd;

		std::map<std::string, OIS::KeyListener*> m_keyListeners;
		std::map<std::string, OIS::MouseListener*> m_mouseListeners;
		std::map<std::string, OIS::JoyStickListener*> m_joystickListeners;

		std::map<std::string, OIS::KeyListener*>::iterator m_keyListenerIter;
		std::map<std::string, OIS::MouseListener*>::iterator m_mouseListenerIter;
		std::map<std::string, OIS::JoyStickListener*>::iterator m_joystickListenerIter;

		std::map<std::string, OIS::KeyListener*>::iterator m_keyListenerIterEnd;
		std::map<std::string, OIS::MouseListener*>::iterator m_mouseListenerIterEnd;
		std::map<std::string, OIS::JoyStickListener*>::iterator m_joystickListenerIterEnd;
	};

}//namespace flt

#endif //FLT_INPUT_MANAGER_H
