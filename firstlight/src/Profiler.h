#ifndef FLT_PROFILER_H
#define FLT_PROFILER_H

#include "common.h"

namespace flt
{	

	class Profiler
	{
	public:
		Profiler();		

		//! return fps
		f32 getFPS() const;		

		//! called every frame
		void onFrame(u32 nowTimeMS);

	private:
		
		f32 m_fps;

		u32 m_timeStart;

		u32 m_frameCount;

	};

} //end namespace flt

#endif //FLT_PROFILER_H
