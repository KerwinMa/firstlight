//输入设备涵盖了各平台上可能的各种输入设备

#ifndef FLT_INPUT_DEVICE_H
#define FLT_INPUT_DEVICE_H

#include "BaseObject.h"
#include "sharePtr.h"

namespace flt
{	
namespace device
{
	enum InputDeviceType
	{
		IDT_UNKNOWN = 0,
		IDT_KEYBOARD = 1,
		IDT_MOUSE = 2,
		IDT_MULTI_TOUCH = 3,
		IDT_ACCELE_SENSOR = 4,
		IDT_JOYSTICK = 5,
	};

	FLT_FORWARD_PTR(InputDevice)

	class InputDevice: public BaseObject
	{
	public:		
		virtual ~InputDevice(){}	

		virtual bool init()=0;
		virtual InputDeviceType getType()=0;

	protected:
		InputDeviceType m_type;				
	};

	class Mouse: public InputDevice
	{
	public:
		Mouse();
		virtual ~Mouse(){}

		virtual bool init()
		{
			return true;
		}

		virtual InputDeviceType getType()
		{
			return IDT_MOUSE;
		}
	private:

	};

} //end namespace device
} //end namespace flt

#endif //FLT_INPUT_DEVICE_H