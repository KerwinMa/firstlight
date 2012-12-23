#include "common.h"
#include "utils/Logger.h"
#include "app/App.h"

#if defined(FLT_DEVICE_WIN32)

#include "Win32Device.h"
#include "core/dimension2d.h"
#include "event/EvtMouse.h"
#include "event/EvtKeyboard.h"
#include "event/EvtTouch.h"
#include "event/EventManager.h"

#include   <comdef.h>

//for find device by HWND
typedef std::map<HWND, flt::device::IDevice*> WindowDeviceMap;
static WindowDeviceMap s_deviceMap;

static flt::device::IDevice* getDeviceFromHWnd(HWND hWnd)
{
	WindowDeviceMap::iterator iter = s_deviceMap.find(hWnd);
	if(iter!=s_deviceMap.end())
		return iter->second;
	else
		return 0;	
}

static bool removeFromDeviceMap(HWND hWnd)
{
	WindowDeviceMap::iterator iter = s_deviceMap.find(hWnd);
	if(iter!=s_deviceMap.end())
	{
		s_deviceMap.erase(iter);
		return true;
	}
	else
	{
		return false;
	}
}

HKL KEYBOARD_INPUT_HKL=0;
unsigned int KEYBOARD_INPUT_CODEPAGE = 1252; 

// Get the codepage from the locale language id 
// Based on the table from http://www.science.co.il/Language/Locale-Codes.asp?s=decimal
static unsigned int LocaleIdToCodepage(unsigned int lcid)
{
	switch ( lcid )
	{
	case 1098:  // Telugu
	case 1095:  // Gujarati
	case 1094:  // Punjabi
	case 1103:  // Sanskrit
	case 1111:  // Konkani
	case 1114:  // Syriac
	case 1099:  // Kannada
	case 1102:  // Marathi
	case 1125:  // Divehi
	case 1067:  // Armenian
	case 1081:  // Hindi
	case 1079:  // Georgian
	case 1097:  // Tamil
		return 0;
	case 1054:  // Thai
		return 874;
	case 1041:  // Japanese
		return 932;
	case 2052:  // Chinese (PRC)
	case 4100:  // Chinese (Singapore)
		return 936;
	case 1042:  // Korean
		return 949;
	case 5124:  // Chinese (Macau S.A.R.)
	case 3076:  // Chinese (Hong Kong S.A.R.)
	case 1028:  // Chinese (Taiwan)
		return 950;
	case 1048:  // Romanian
	case 1060:  // Slovenian
	case 1038:  // Hungarian
	case 1051:  // Slovak
	case 1045:  // Polish
	case 1052:  // Albanian
	case 2074:  // Serbian (Latin)
	case 1050:  // Croatian
	case 1029:  // Czech
		return 1250;
	case 1104:  // Mongolian (Cyrillic)
	case 1071:  // FYRO Macedonian
	case 2115:  // Uzbek (Cyrillic)
	case 1058:  // Ukrainian
	case 2092:  // Azeri (Cyrillic)
	case 1092:  // Tatar
	case 1087:  // Kazakh
	case 1059:  // Belarusian
	case 1088:  // Kyrgyz (Cyrillic)
	case 1026:  // Bulgarian
	case 3098:  // Serbian (Cyrillic)
	case 1049:  // Russian
		return 1251;
	case 8201:  // English (Jamaica)
	case 3084:  // French (Canada)
	case 1036:  // French (France)
	case 5132:  // French (Luxembourg)
	case 5129:  // English (New Zealand)
	case 6153:  // English (Ireland)
	case 1043:  // Dutch (Netherlands)
	case 9225:  // English (Caribbean)
	case 4108:  // French (Switzerland)
	case 4105:  // English (Canada)
	case 1110:  // Galician
	case 10249:  // English (Belize)
	case 3079:  // German (Austria)
	case 6156:  // French (Monaco)
	case 12297:  // English (Zimbabwe)
	case 1069:  // Basque
	case 2067:  // Dutch (Belgium)
	case 2060:  // French (Belgium)
	case 1035:  // Finnish
	case 1080:  // Faroese
	case 1031:  // German (Germany)
	case 3081:  // English (Australia)
	case 1033:  // English (United States)
	case 2057:  // English (United Kingdom)
	case 1027:  // Catalan
	case 11273:  // English (Trinidad)
	case 7177:  // English (South Africa)
	case 1030:  // Danish
	case 13321:  // English (Philippines)
	case 15370:  // Spanish (Paraguay)
	case 9226:  // Spanish (Colombia)
	case 5130:  // Spanish (Costa Rica)
	case 7178:  // Spanish (Dominican Republic)
	case 12298:  // Spanish (Ecuador)
	case 17418:  // Spanish (El Salvador)
	case 4106:  // Spanish (Guatemala)
	case 18442:  // Spanish (Honduras)
	case 3082:  // Spanish (International Sort)
	case 13322:  // Spanish (Chile)
	case 19466:  // Spanish (Nicaragua)
	case 2058:  // Spanish (Mexico)
	case 10250:  // Spanish (Peru)
	case 20490:  // Spanish (Puerto Rico)
	case 1034:  // Spanish (Traditional Sort)
	case 14346:  // Spanish (Uruguay)
	case 8202:  // Spanish (Venezuela)
	case 1089:  // Swahili
	case 1053:  // Swedish
	case 2077:  // Swedish (Finland)
	case 5127:  // German (Liechtenstein)
	case 1078:  // Afrikaans
	case 6154:  // Spanish (Panama)
	case 4103:  // German (Luxembourg)
	case 16394:  // Spanish (Bolivia)
	case 2055:  // German (Switzerland)
	case 1039:  // Icelandic
	case 1057:  // Indonesian
	case 1040:  // Italian (Italy)
	case 2064:  // Italian (Switzerland)
	case 2068:  // Norwegian (Nynorsk)
	case 11274:  // Spanish (Argentina)
	case 1046:  // Portuguese (Brazil)
	case 1044:  // Norwegian (Bokmal)
	case 1086:  // Malay (Malaysia)
	case 2110:  // Malay (Brunei Darussalam)
	case 2070:  // Portuguese (Portugal)
		return 1252;
	case 1032:  // Greek
		return 1253;
	case 1091:  // Uzbek (Latin)
	case 1068:  // Azeri (Latin)
	case 1055:  // Turkish
		return 1254;
	case 1037:  // Hebrew
		return 1255;
	case 5121:  // Arabic (Algeria)
	case 15361:  // Arabic (Bahrain)
	case 9217:  // Arabic (Yemen)
	case 3073:  // Arabic (Egypt)
	case 2049:  // Arabic (Iraq)
	case 11265:  // Arabic (Jordan)
	case 13313:  // Arabic (Kuwait)
	case 12289:  // Arabic (Lebanon)
	case 4097:  // Arabic (Libya)
	case 6145:  // Arabic (Morocco)
	case 8193:  // Arabic (Oman)
	case 16385:  // Arabic (Qatar)
	case 1025:  // Arabic (Saudi Arabia)
	case 10241:  // Arabic (Syria)
	case 14337:  // Arabic (U.A.E.)
	case 1065:  // Farsi
	case 1056:  // Urdu
	case 7169:  // Arabic (Tunisia)
		return 1256;
	case 1061:  // Estonian
	case 1062:  // Latvian
	case 1063:  // Lithuanian
		return 1257;
	case 1066:  // Vietnamese
		return 1258;
	}
	return 65001;   // utf-8
} 

bool ProcessMouseEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	#ifndef WM_MOUSEWHEEL
	#define WM_MOUSEWHEEL 0x020A
	#endif
	#ifndef WHEEL_DELTA
	#define WHEEL_DELTA 120
	#endif


	static flt::s32 ClickCount=0;
	if (GetCapture() != hWnd && ClickCount > 0)
		ClickCount = 0;


	struct messageMap
	{
		flt::s32 group;
		UINT winMessage;
		flt::s32 fltMessage;
	};

	static messageMap mouseMap[] =
	{
		{0, WM_LBUTTONDOWN, flt::EMIE_LMOUSE_PRESSED_DOWN},
		{1, WM_LBUTTONUP,   flt::EMIE_LMOUSE_LEFT_UP},
		{0, WM_RBUTTONDOWN, flt::EMIE_RMOUSE_PRESSED_DOWN},
		{1, WM_RBUTTONUP,   flt::EMIE_RMOUSE_LEFT_UP},
		{0, WM_MBUTTONDOWN, flt::EMIE_MMOUSE_PRESSED_DOWN},
		{1, WM_MBUTTONUP,   flt::EMIE_MMOUSE_LEFT_UP},
		{2, WM_MOUSEMOVE,   flt::EMIE_MOUSE_MOVED},
		{3, WM_MOUSEWHEEL,  flt::EMIE_MOUSE_WHEEL},
		{-1, 0, 0}
	};

	// handle grouped events
	messageMap * m = mouseMap;
	while ( m->group >=0 && m->winMessage != message )
		m += 1;

	if ( m->group >= 0 )
	{
		if ( m->group == 0 )	// down
		{
			ClickCount++;
			SetCapture(hWnd);
		}
		else
			if ( m->group == 1 )	// up
			{
				ClickCount--;
				if (ClickCount<1)
				{
					ClickCount=0;
					ReleaseCapture();
				}
			}

			flt::EMOUSE_INPUT_EVENT mouseEventType = (flt::EMOUSE_INPUT_EVENT) m->fltMessage;
			int mouseX = (short)LOWORD(lParam);
			int mouseY = (short)HIWORD(lParam);
			bool Shift = ((LOWORD(wParam) & MK_SHIFT) != 0);
			bool Control = ((LOWORD(wParam) & MK_CONTROL) != 0);
			bool leftButtonDown = (wParam & ( MK_LBUTTON ) );
//			// left and right mouse buttons
//			flt::u32 ButtonStates = wParam & ( MK_LBUTTON | MK_RBUTTON);
//			// middle and extra buttons
//			if (wParam & MK_MBUTTON)
//				ButtonStates |= flt::EMBSM_MIDDLE;
//#if(_WIN32_WINNT >= 0x0500)
//			if (wParam & MK_XBUTTON1)
//				ButtonStates |= flt::EMBSM_EXTRA1;
//			if (wParam & MK_XBUTTON2)
//				ButtonStates |= flt::EMBSM_EXTRA2;
//#endif
			flt::f32 Wheel = 0.f;

			// wheel
			if ( m->group == 3 )
			{
				POINT p; // fixed by jox
				p.x = 0; p.y = 0;
				ClientToScreen(hWnd, &p);
				mouseX -= p.x;
				mouseY -= p.y;
				Wheel = ((flt::f32)((short)HIWORD(wParam))) / (flt::f32)WHEEL_DELTA;
			}

			switch(mouseEventType)
			{
			case flt::EMIE_MOUSE_MOVED:
				{
					const flt::EvtMouseMove evt(mouseX, mouseY, Shift, Control);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			case flt::EMIE_MOUSE_WHEEL:
				{
					const flt::EvtMouseWheel evt(Wheel);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			case flt::EMIE_LMOUSE_LEFT_UP:
			case flt::EMIE_LMOUSE_PRESSED_DOWN:
				{
					const flt::EvtMouseButton evt(flt::EvtMouseButton::MOUSE_BTN_LEFT, mouseEventType == flt::EMIE_LMOUSE_PRESSED_DOWN, mouseX, mouseY);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			case flt::EMIE_MMOUSE_LEFT_UP:
			case flt::EMIE_MMOUSE_PRESSED_DOWN:
				{
					const flt::EvtMouseButton evt(flt::EvtMouseButton::MOUSE_BTN_MIDDLE, mouseEventType == flt::EMIE_MMOUSE_PRESSED_DOWN, mouseX, mouseY);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			case flt::EMIE_RMOUSE_LEFT_UP:
			case flt::EMIE_RMOUSE_PRESSED_DOWN:
				{
					const flt::EvtMouseButton evt(flt::EvtMouseButton::MOUSE_BTN_RIGHT, mouseEventType == flt::EMIE_RMOUSE_PRESSED_DOWN, mouseX, mouseY);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			}	

			//for Win32, use mouse event to emulate touch event
			switch(mouseEventType)
			{
			case flt::EMIE_LMOUSE_PRESSED_DOWN:
				{
					const flt::EvtTouch evt(0, flt::Touch_Began, mouseX, mouseY);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			case flt::EMIE_MOUSE_MOVED:
				{
					if(leftButtonDown)
					{
						const flt::EvtTouch evt(0, flt::Touch_Moved, mouseX, mouseY);
						flt::EventManager::getInstance().raiseEvent(evt);
					}
				}
				break;

			case flt::EMIE_LMOUSE_LEFT_UP:
				{
					const flt::EvtTouch evt(0, flt::Touch_Ended, mouseX, mouseY);
					flt::EventManager::getInstance().raiseEvent(evt);
				}
				break;

			}
			
			return true;
	}

	return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(ProcessMouseEvent(hWnd, message, wParam, lParam))
		return 0;
	
	switch (message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		return 0;

	case WM_ERASEBKGND:
		return 0;		

	case WM_SIZE:
		{
			// resize window
			flt::device::IDevice* pDevice = getDeviceFromHWnd(hWnd);
			if (pDevice)
				pDevice->onWindowSizeChange(LOWORD(lParam),HIWORD(lParam));			
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		// prevent screensaver or monitor powersave mode from starting
		if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
			(wParam & 0xFFF0) == SC_MONITORPOWER)
			return 0;
		break;	

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			BYTE allKeys[256];

			flt::EvtKeyboard event;
			
						
			event.KeyCode = (flt::KEY_CODE)wParam;
			event.Pressed = (message==WM_KEYDOWN || message == WM_SYSKEYDOWN);

			const UINT MY_MAPVK_VSC_TO_VK_EX = 3; // MAPVK_VSC_TO_VK_EX should be in SDK according to MSDN, but isn't in mine.
			if ( event.KeyCode == flt::KEY_SHIFT )
			{
				// this will fail on systems before windows NT/2000/XP, not sure _what_ will return there instead.
				event.KeyCode = (flt::KEY_CODE)MapVirtualKey( (UINT)((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
			}
			if ( event.KeyCode == flt::KEY_CONTROL )
			{
				event.KeyCode = (flt::KEY_CODE)MapVirtualKey( (UINT)((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
				// some keyboards will just return LEFT for both - left and right keys. So also check extend bit.
				if (lParam & 0x1000000)
					event.KeyCode = flt::KEY_RCONTROL;
			}
			if ( event.KeyCode == flt::KEY_MENU )
			{
				event.KeyCode = (flt::KEY_CODE)MapVirtualKey( (UINT)((lParam>>16) & 255), MY_MAPVK_VSC_TO_VK_EX );
				if (lParam & 0x1000000)
					event.KeyCode = flt::KEY_RMENU;
			}

			GetKeyboardState(allKeys);

			event.Shift = ((allKeys[VK_SHIFT] & 0x80)!=0);
			event.Control = ((allKeys[VK_CONTROL] & 0x80)!=0);			

			// Handle unicode and deadkeys in a way that works since Windows 95 and nt4.0
			// Using ToUnicode instead would be shorter, but would to my knowledge not run on 95 and 98.
			WORD keyChars[2];
			UINT scanCode = HIWORD(lParam);
			int conversionResult = ToAsciiEx((UINT)wParam,scanCode,allKeys,keyChars,0,KEYBOARD_INPUT_HKL);
			if (conversionResult == 1)
			{
				WORD unicodeChar;
				MultiByteToWideChar(
					KEYBOARD_INPUT_CODEPAGE,
					MB_PRECOMPOSED, // default
					(LPCSTR)keyChars,
					sizeof(keyChars),
					(WCHAR*)&unicodeChar,
					1 );
				event.Char = unicodeChar; 
			}
			else
				event.Char = 0;

			// allow composing characters like '@' with Alt Gr on non-US keyboards
			if ((allKeys[VK_MENU] & 0x80) != 0)
				event.Control = false;
			
			flt::EventManager::getInstance().raiseEvent(event);

			// Emulate device orientation change by pressing ctrl+up/down/left/right
			if(event.Control)
			{
				flt::device::IDevice* pDevice = getDeviceFromHWnd(hWnd);
				
				flt::EDeviceOrientation nowOri = pDevice->getOrientation();
				flt::EDeviceOrientation newOri = nowOri;

				switch(event.KeyCode)
				{
				case flt::KEY_UP:
					newOri = flt::EDO_Portrait; 
					break;
				case flt::KEY_DOWN:
					newOri = flt::EDO_PortraitUpsideDown;
					break;
				case flt::KEY_LEFT:
					newOri = flt::EDO_LandscapeLeft;
					break;
				case flt::KEY_RIGHT:
					newOri = flt::EDO_LandscapeRight;
					break;
				}

				if(nowOri!=newOri)
				{					
					if(flt::App::TheApp)
					{
						flt::App::TheApp->onRotate(newOri);
					}
					else
					{
						pDevice->setOrientation(newOri);
					}
				}
			}	

			if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP)
				return DefWindowProc(hWnd, message, wParam, lParam);
			else
				return 0;
		}
		break;

	case WM_INPUTLANGCHANGE:
		{
			// get the new codepage used for keyboard input
			KEYBOARD_INPUT_HKL = GetKeyboardLayout(0);
			KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage( LOWORD(KEYBOARD_INPUT_HKL) );
		}
		return 0; 
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

namespace flt
{	

namespace renderer
{

#if defined(FLT_COMPILE_WITH_OPENGLES1)
	IRendererPtr createOpenGLES1Renderer(const core::dimension2di& screenSize, HWND hWnd, void* shareContext);
#endif

}

#ifdef UNICODE
	#define FLT_WIN_CLASS_NAME		L"FLT_Win32Device"
	#define FLT_WIN_NAME			L""
#else
	#define FLT_WIN_CLASS_NAME		"FLT_Win32Device"
	#define FLT_WIN_NAME			""
#endif

namespace device
{

Win32Device::Win32Device(const RenderDeviceAttribute& deviceAttr)
	:StubDevice(deviceAttr),
	 m_useExternalWindow(false),
	 m_hWnd(0)
{
	LARGE_INTEGER freq;
	::QueryPerformanceFrequency(&freq);
	m_secsPerTick = 1.0/freq.QuadPart;

	HINSTANCE hInstance = GetModuleHandle(0);

	// if not use external window, create one
	if (m_deviceAttr.nativeWindowHandle==0)
	{		
		// Register Class
		WNDCLASSEX wcex;
		wcex.cbSize		= sizeof(WNDCLASSEX);
		wcex.style		= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon		= NULL;
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= FLT_WIN_CLASS_NAME;
		wcex.hIconSm		= 0;

		// if there is an icon, load it
		wcex.hIcon = (HICON)LoadImage(hInstance, "firstlight.ico", IMAGE_ICON, 0,0, LR_LOADFROMFILE); 

		RegisterClassEx(&wcex);

	
		DWORD style = WS_POPUP;

		//if (!Fullscreen)
		style = WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		adjustWindowSize(m_deviceAttr.width, m_deviceAttr.height);		

		// create window

		m_hWnd = CreateWindow( FLT_WIN_CLASS_NAME, FLT_WIN_NAME, style, m_winLeft, m_winTop,
					m_winWidth, m_winHeight, NULL, NULL, hInstance, NULL);

		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
		MoveWindow(m_hWnd, m_winLeft, m_winTop, m_winWidth, m_winHeight, TRUE);
	}

	// attach external window
	if (m_deviceAttr.nativeWindowHandle!=0)
	{
		m_hWnd = static_cast<HWND>(m_deviceAttr.nativeWindowHandle);
		RECT r;
		GetWindowRect(m_hWnd, &r);
		m_deviceAttr.width = r.right - r.left;
		m_deviceAttr.height = r.bottom - r.top;
		//Fullscreen = false;
		m_useExternalWindow = true;		
	}

	createRenderer();	

	setWindowTitle(L"FLT-Win32");

	SetActiveWindow(m_hWnd);
	SetForegroundWindow(m_hWnd);

	resetRand();

	s_deviceMap[m_hWnd] = this;

	LOG("engine","Win32Device created.");
	LOG("engine","screen=%dX%d, bits=%d, useExternalWindow=%d",m_deviceAttr.width,m_deviceAttr.height,m_deviceAttr.colorBits,m_useExternalWindow);
}

Win32Device::~Win32Device()
{	
	LOG("engine","Win32Device shut down.");
}

void Win32Device::createRenderer()
{		
	switch(m_deviceAttr.rendererType)
	{

	case renderer::RENDERER_OPENGLES1:
	{

#if defined(FLT_COMPILE_WITH_OPENGLES1)
		void* shareRender = NULL;
		if (m_deviceAttr.shareDevice)
		{
			 IDevice* device = static_cast<IDevice *>(m_deviceAttr.shareDevice);
			 shareRender = device->getRenderer();
		}
		m_renderer = renderer::createOpenGLES1Renderer(core::dimension2di(m_deviceAttr.width,m_deviceAttr.height),m_hWnd,shareRender);
		if(m_renderer.isValid())
		{
			LOG("engine","OpenGLES1 renderer created.");
		}
		else
		{
			LOG("engine","Failed to create OpenGLES1 renderer!");
		}
#else
		LOG("engine","OpenGL ES1.1 renderer not compiled!");
#endif
	}
		break;

	default:
		LOG("engine","Error: renderer not supported by this device!");
		break;

	}
		
}

s32 Win32Device::getWindowID() const
{
	return (s32)m_hWnd;
}

void Win32Device::getWindowRect(s32& left, s32& top, s32& width, s32& height) const
{
	left = m_winLeft;
	top = m_winTop;
	width = m_winWidth;
	height = m_winHeight;
}

bool Win32Device::run()
{
	MSG msg;

	bool quit = false;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);

		if (m_useExternalWindow && msg.hwnd == m_hWnd)
			WndProc(m_hWnd, msg.message, msg.wParam, msg.lParam);
		else
			DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			quit = true;
	}

	if(quit)
	{
		removeFromDeviceMap(m_hWnd);
	}

	//if (!quit)
	//	resizeIfNecessary();
	
	return !quit;
}

void Win32Device::close()
{

}

f64 Win32Device::getSystemTime() const
{	
	LARGE_INTEGER time;
	::QueryPerformanceCounter(&time);
	return time.QuadPart * m_secsPerTick * 1000;
}

void Win32Device::sleep(u32 timeMs)
{
	SleepEx(timeMs, TRUE);
}


void Win32Device::setWindowTitle(const stringw& title)
{
#ifdef UNICODE
	SetWindowText(m_hWnd, title.c_str());
#else
		
	_bstr_t bstr1(title.c_str());
	stringc str(bstr1);
	 
	SetWindowText(m_hWnd, str.c_str());
#endif
}

void Win32Device::adjustWindowSize(u32 newWidth, u32 newHeight)
{
	RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = newWidth;
	clientSize.bottom = newHeight;

	DWORD style = WS_OVERLAPPEDWINDOW | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&clientSize, style, FALSE);

	const s32 realWidth = clientSize.right - clientSize.left;
	const s32 realHeight = clientSize.bottom - clientSize.top;

	s32 windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
	s32 windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;
			
	if(m_hWnd)
		MoveWindow(m_hWnd, windowLeft, windowTop, realWidth, realHeight, TRUE);

	m_winLeft = windowLeft;
	m_winTop = windowTop;
	m_winWidth = realWidth;
	m_winHeight = realHeight;
}

void Win32Device::setOrientation(EDeviceOrientation newOrientation)
{
	EDeviceOrientation oldOri = getOrientation();

	if(newOrientation==oldOri)
		return;

	StubDevice::setOrientation(newOrientation);

	if(newOrientation==EDO_LandscapeLeft && oldOri==EDO_LandscapeRight ||
		newOrientation==EDO_LandscapeRight && oldOri==EDO_LandscapeLeft ||
		newOrientation==EDO_Portrait && oldOri==EDO_PortraitUpsideDown ||
		newOrientation==EDO_PortraitUpsideDown && oldOri==EDO_Portrait)
			return;

	//change between portrait and landscape
	u32 newWidth = m_deviceAttr.height;
	u32 newHeight = m_deviceAttr.width;

	onWindowSizeChange(newWidth, newHeight);

	//adjust window
	adjustWindowSize(newWidth, newHeight);
}

} //end namespace device


device::IDevicePtr createDevice(const device::RenderDeviceAttribute& renderDeviceAttr)
{
	device::IDevicePtr devicePtr = new device::Win32Device(renderDeviceAttr);	

	return devicePtr;
}


} //end namespace flt

#endif //FLT_DEVICE_WIN32