#include "InputManager.h"
#include "device/IDevice.h"
#include <iostream>
#include <vector>
#include <sstream>

namespace flt
{


InputManager::InputManager( void )
	:m_pMouse( 0 ),
    m_pKeyboard( 0 ),
    m_pInputSystem( 0 )
{
}

InputManager::~InputManager( void ) 
{
    if( m_pInputSystem ) 
	{
        if( m_pMouse ) 
		{
            m_pInputSystem->destroyInputObject( m_pMouse );
            m_pMouse = 0;
        }

        if( m_pKeyboard ) 
		{
            m_pInputSystem->destroyInputObject( m_pKeyboard );
            m_pKeyboard = 0;
        }

        if( m_pJoysticks.size() > 0 ) 
		{
            m_joystickIter    = m_pJoysticks.begin();
            m_joystickIterEnd = m_pJoysticks.end();
            for(; m_joystickIter != m_joystickIterEnd; ++m_joystickIter ) 
			{
                m_pInputSystem->destroyInputObject( *m_joystickIter );
            }

            m_pJoysticks.clear();
        }

		OIS::InputManager::destroyInputSystem(m_pInputSystem);
        m_pInputSystem = 0;

        // Clear Listeners
        m_keyListeners.clear();
        m_mouseListeners.clear();
        m_joystickListeners.clear();
    }
}

void InputManager::init( const device::IDevicePtr& device ) 
{
    if( !m_pInputSystem ) 
	{
        // Setup basic variables
        OIS::ParamList paramList;    
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;

        // Fill parameter list
        windowHndStr << (unsigned int) device->getWindowID();
        paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

        // Create inputsystem
        m_pInputSystem = OIS::InputManager::createInputSystem( paramList );

        // If possible create a buffered keyboard
		if( m_pInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0 ) 
		{
            m_pKeyboard = static_cast<OIS::Keyboard*>( m_pInputSystem->createInputObject( OIS::OISKeyboard, true ) );
            m_pKeyboard->setEventCallback( this );
        }

        // If possible create a buffered mouse
		if( m_pInputSystem->getNumberOfDevices(OIS::OISMouse) > 0 ) 
		{
            m_pMouse = static_cast<OIS::Mouse*>( m_pInputSystem->createInputObject( OIS::OISMouse, true ) );
            m_pMouse->setEventCallback( this );

            // Get window size            
            int left, top, width, height;
			device->getWindowRect(left,top,width,height);            

            // Set mouse region
            this->setWindowExtents( width, height );
        }

        // If possible create all joysticks in buffered mode
		int joystickNum = m_pInputSystem->getNumberOfDevices(OIS::OISJoyStick);
		if(  joystickNum> 0 )
		{
            m_pJoysticks.resize( joystickNum );

            m_joystickIter    = m_pJoysticks.begin();
            m_joystickIterEnd = m_pJoysticks.end();
            for(; m_joystickIter != m_joystickIterEnd; ++m_joystickIter ) {
                (*m_joystickIter) = static_cast<OIS::JoyStick*>( m_pInputSystem->createInputObject( OIS::OISJoyStick, true ) );
                (*m_joystickIter)->setEventCallback( this );
            }
        }
    }
}

void InputManager::capture( void ) 
{
    // Need to capture / update each device every frame
    if( m_pMouse ) 
	{
        m_pMouse->capture();
    }

    if( m_pKeyboard ) 
	{
        m_pKeyboard->capture();
    }

    if( m_pJoysticks.size() > 0 ) 
	{
        m_joystickIter    = m_pJoysticks.begin();
        m_joystickIterEnd = m_pJoysticks.end();
        for(; m_joystickIter != m_joystickIterEnd; ++m_joystickIter ) {
            (*m_joystickIter)->capture();
        }
    }
}

void InputManager::addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName ) 
{
    if( m_pKeyboard ) 
	{
        // Check for duplicate items
        m_keyListenerIter = m_keyListeners.find( instanceName );
        if( m_keyListenerIter == m_keyListeners.end() ) 
		{
            m_keyListeners[ instanceName ] = keyListener;
        }
    }
}

void InputManager::addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName ) 
{
    if( m_pMouse ) 
	{
        // Check for duplicate items
        m_mouseListenerIter = m_mouseListeners.find( instanceName );
        if( m_mouseListenerIter == m_mouseListeners.end() ) {
            m_mouseListeners[ instanceName ] = mouseListener;
        }
    }
}

void InputManager::addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName ) 
{
    if( m_pJoysticks.size() > 0 ) 
	{
        // Check for duplicate items
        m_joystickListenerIter = m_joystickListeners.find( instanceName );
        if( m_joystickListenerIter != m_joystickListeners.end() ) 
		{
            m_joystickListeners[ instanceName ] = joystickListener;
        }        
    }
}

void InputManager::removeKeyListener( const std::string& instanceName ) 
{
    // Check if item exists
    m_keyListenerIter = m_keyListeners.find( instanceName );
    if( m_keyListenerIter != m_keyListeners.end() ) 
	{
        m_keyListeners.erase( m_keyListenerIter );
    }
    else 
	{
        // Doesn't Exist
    }
}

void InputManager::removeMouseListener( const std::string& instanceName ) 
{
    // Check if item exists
    m_mouseListenerIter = m_mouseListeners.find( instanceName );
    if( m_mouseListenerIter != m_mouseListeners.end() ) 
	{
        m_mouseListeners.erase( m_mouseListenerIter );
    }
    else 
	{
        // Doesn't Exist
    }
}

void InputManager::removeJoystickListener( const std::string& instanceName ) 
{
    // Check if item exists
    m_joystickListenerIter = m_joystickListeners.find( instanceName );
    if( m_joystickListenerIter != m_joystickListeners.end() ) 
	{
        m_joystickListeners.erase( m_joystickListenerIter );
    }
    else 
	{
        // Doesn't Exist
    }
}

void InputManager::removeKeyListener( OIS::KeyListener *keyListener ) 
{
    m_keyListenerIter    = m_keyListeners.begin();
    m_keyListenerIterEnd = m_keyListeners.end();
    for(; m_keyListenerIter != m_keyListenerIterEnd; ++m_keyListenerIter ) 
	{
        if( m_keyListenerIter->second == keyListener ) 
		{
            m_keyListeners.erase( m_keyListenerIter );
            break;
        }
    }
}

void InputManager::removeMouseListener( OIS::MouseListener *mouseListener ) 
{
    m_mouseListenerIter    = m_mouseListeners.begin();
    m_mouseListenerIterEnd = m_mouseListeners.end();
    for(; m_mouseListenerIter != m_mouseListenerIterEnd; ++m_mouseListenerIter ) 
	{
        if( m_mouseListenerIter->second == mouseListener ) {
            m_mouseListeners.erase( m_mouseListenerIter );
            break;
        }
    }
}

void InputManager::removeJoystickListener( OIS::JoyStickListener *joystickListener ) 
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter ) 
	{
        if( m_joystickListenerIter->second == joystickListener ) 
		{
            m_joystickListeners.erase( m_joystickListenerIter );
            break;
        }
    }
}

void InputManager::removeAllListeners( void ) 
{
    m_keyListeners.clear();
    m_mouseListeners.clear();
    m_joystickListeners.clear();
}

void InputManager::removeAllKeyListeners( void ) 
{
    m_keyListeners.clear();
}

void InputManager::removeAllMouseListeners( void ) 
{
    m_mouseListeners.clear();
}

void InputManager::removeAllJoystickListeners( void ) 
{
    m_joystickListeners.clear();
}

void InputManager::setWindowExtents( int width, int height ) 
{
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = m_pMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}

OIS::Mouse* InputManager::getMouse( void ) 
{
    return m_pMouse;
}

OIS::Keyboard* InputManager::getKeyboard( void ) 
{
    return m_pKeyboard;
}

OIS::JoyStick* InputManager::getJoystick( unsigned int index ) 
{
    // Make sure it's a valid index
    if( index < m_pJoysticks.size() ) 
	{
        return m_pJoysticks[ index ];
    }

    return 0;
}

int InputManager::getNumOfJoysticks( void ) 
{
    return (int) m_pJoysticks.size();
}

bool InputManager::keyPressed( const OIS::KeyEvent &e ) 
{
    m_keyListenerIter    = m_keyListeners.begin();
    m_keyListenerIterEnd = m_keyListeners.end();
    for(; m_keyListenerIter != m_keyListenerIterEnd; ++m_keyListenerIter ) 
	{
        m_keyListenerIter->second->keyPressed( e );
    }

    return true;
}

bool InputManager::keyReleased( const OIS::KeyEvent &e ) 
{
    m_keyListenerIter    = m_keyListeners.begin();
    m_keyListenerIterEnd = m_keyListeners.end();
    for(; m_keyListenerIter != m_keyListenerIterEnd; ++m_keyListenerIter ) 
	{
        m_keyListenerIter->second->keyReleased( e );
    }

    return true;
}

bool InputManager::mouseMoved( const OIS::MouseEvent &e )
{
    m_mouseListenerIter    = m_mouseListeners.begin();
    m_mouseListenerIterEnd = m_mouseListeners.end();
    for(; m_mouseListenerIter != m_mouseListenerIterEnd; ++m_mouseListenerIter ) 
	{
        m_mouseListenerIter->second->mouseMoved( e );
    }

    return true;
}

bool InputManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) 
{
    m_mouseListenerIter    = m_mouseListeners.begin();
    m_mouseListenerIterEnd = m_mouseListeners.end();
    for(; m_mouseListenerIter != m_mouseListenerIterEnd; ++m_mouseListenerIter ) 
	{
        m_mouseListenerIter->second->mousePressed( e, id );
    }

    return true;
}

bool InputManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) 
{
    m_mouseListenerIter    = m_mouseListeners.begin();
    m_mouseListenerIterEnd = m_mouseListeners.end();
    for(; m_mouseListenerIter != m_mouseListenerIterEnd; ++m_mouseListenerIter ) 
	{
        m_mouseListenerIter->second->mouseReleased( e, id );
    }

    return true;
}

bool InputManager::povMoved( const OIS::JoyStickEvent &e, int pov ) 
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter ) 
	{
        m_joystickListenerIter->second->povMoved( e, pov );
    }

    return true;
}

bool InputManager::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter ) 
	{
        m_joystickListenerIter->second->axisMoved( e, axis );
    }

    return true;
}

bool InputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter )
	{
        m_joystickListenerIter->second->sliderMoved( e, sliderID );
    }

    return true;
}

bool InputManager::buttonPressed( const OIS::JoyStickEvent &e, int button ) 
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter )
	{
        m_joystickListenerIter->second->buttonPressed( e, button );
    }

    return true;
}

bool InputManager::buttonReleased( const OIS::JoyStickEvent &e, int button ) 
{
    m_joystickListenerIter    = m_joystickListeners.begin();
    m_joystickListenerIterEnd = m_joystickListeners.end();
    for(; m_joystickListenerIter != m_joystickListenerIterEnd; ++m_joystickListenerIter )
	{
        m_joystickListenerIter->second->buttonReleased( e, button );
    }

    return true;
}


}//namespace flt
