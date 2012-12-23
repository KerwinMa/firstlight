#include "Logger.h"
#include <stdio.h>
#include <fstream>
#include <stdarg.h>
#include <string>
#include "config.h"

#if defined(FLT_DEVICE_WIN32)
#include <windows.h>
#endif

//for debug
#include "app/App.h"

namespace flt
{
namespace utils
{

LogFile::LogFile(const std::string& logFileName)
{
#if defined(FLT_DEVICE_WIN32)
	m_ostrm.open(logFileName.c_str()) ; 
#endif
}

LogFile::~LogFile()
{
#if defined(FLT_DEVICE_WIN32)
	writeLine("-------------------------") ;
	writeLine("Log File closed normally.") ;
	m_ostrm.close() ;
#endif
}

void LogFile::writeLine(const char* log)
{
#if defined(FLT_DEVICE_WIN32)	
	m_ostrm<<log<<std::endl ;
#endif
}

void LogFile::log(const char* log,...)
{
#if defined(FLT_DEVICE_WIN32)
	va_list va;
	char str[1024];

	va_start(va,log);
	vsprintf(str,log,va);
	va_end(va);

	m_ostrm<<str<<std::endl ;
#endif
}

void LogFile::logList(const char* log, va_list va)
{
#if defined(FLT_DEVICE_WIN32)
	char str[1024];

	vsprintf(str,log,va);	

	m_ostrm<<str ;
#endif
};

///////////////////////////////////////////////////////////////

Logger::Logger()
	:m_logFile(0)
{	
#if defined(FLT_DEVICE_WIN32)
#ifdef FLT_DEBUG
	m_logFile = new LogFile("fltLog_d.txt");
#else
	m_logFile = new LogFile("fltLog.txt");
#endif
	memset(m_buffer, 0, sizeof(m_buffer));

	m_logFile->writeLine("------------ FirstLight Engine Log File ---------------");	
#endif
}

Logger::~Logger()
{
#if defined(FLT_DEVICE_WIN32)
	delete m_logFile;
#endif
}

void Logger::enableGroup(const char* group, bool enable)
{
	m_groupSwitchs[group] = enable;
}

bool Logger::isGroupEnabled(const char* group) const
{
	GroupSwitch::const_iterator it = m_groupSwitchs.find(group);
	if (it != m_groupSwitchs.end()) 
	{
		return it->second;
	} 
	else 
	{
		return false;
	}
}

void Logger::logOut(const char* group, const char* fmt, ...)
{
#if defined(FLT_DEVICE_WIN32)
	if(!isGroupEnabled(group))
		return;

	int index = sprintf(m_buffer, "[%s][%ld]", group, (long)App::s_frameNum);
	va_list va; 
	va_start (va, fmt); 	
	vsprintf(m_buffer+index,fmt,va);
	va_end (va);

	m_logFile->writeLine(m_buffer);	
#endif
}

void Logger::logOutCodeLine(const char* file, int line, const char* group, const char* fmt, ...)
{
#if defined(FLT_DEVICE_WIN32)
	if(!isGroupEnabled(group))
		return;

	int index = sprintf(m_buffer, "%s(%d):[%s][%ld]", file, line, group, (long)App::s_frameNum);
	va_list va; 
	va_start (va, fmt); 	
	vsprintf(m_buffer+index,fmt,va);
	va_end (va); 	

	m_logFile->writeLine(m_buffer);
#endif
}

void Logger::debugOut(const char* group, const char* fmt, ...)
{
#if defined(FLT_DEVICE_WIN32)
#if defined(FLT_DEBUG) 
	if(!isGroupEnabled(group))
		return;
	
	int index = sprintf(m_buffer, "[%s][%ld]", group, (long)App::s_frameNum);
	va_list va; 
	va_start (va, fmt); 	
	vsprintf(m_buffer+index,fmt,va);
	va_end (va); 	

#if defined(FLT_DEVICE_WIN32)
	OutputDebugStringA(m_buffer);
	OutputDebugStringA("\n");
#else
	printf(m_buffer);
	printf("\n");
#endif

#endif //FLT_DEBUG
#endif
}

void Logger::debugOutCodeLine(const char* file, int line, const char* group, const char* fmt, ...)
{
#if defined(FLT_DEVICE_WIN32)
#if defined(FLT_DEBUG)
	if(!isGroupEnabled(group))
		return;
	
	int index = sprintf(m_buffer, "%s(%d):[%s][%ld]", file, line, group, (long)App::s_frameNum);
	va_list va; 
	va_start (va, fmt); 	
	vsprintf(m_buffer+index,fmt,va);
	va_end (va); 	

#if defined(FLT_DEVICE_WIN32)
	OutputDebugStringA(m_buffer);	
	OutputDebugStringA("\n");
#else
	printf(m_buffer);
	printf("\n");
#endif

#endif //FLT_DEBUG
#endif
}

} //end namespace utils
} //end namespace flt