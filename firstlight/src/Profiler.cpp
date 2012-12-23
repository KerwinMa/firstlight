
#include "Profiler.h"
#include "core/fltMath.h"

namespace flt
{	
	
	Profiler::Profiler()
		:m_fps(0.0f),m_timeStart(0),
		 m_frameCount(0)
	{

	}

	//! return fps
	f32 Profiler::getFPS() const
	{
		return m_fps;
	}

	//! called every frame
	void Profiler::onFrame(u32 nowTimeMS)
	{
		m_frameCount++;							

		const u32 milliseconds = nowTimeMS - m_timeStart;

		if (milliseconds >= 1500 )
		{
			const f32 invMilli = flt::core::reciprocal ( (f32) milliseconds );

			m_fps = ceilf( ( 1000 * m_frameCount ) * invMilli );
			
			m_frameCount = 0;			
			m_timeStart = nowTimeMS;
		}
	}


} //end namespace flt

