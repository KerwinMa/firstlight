#include "keyframe.h"

namespace flt
{
namespace anim
{

	//---------------------------------------------------

	KeyFrame::KeyFrame()
		:m_time(0),m_interpType(IT_LINEAR)
	{

	}

	//KeyFrame* KeyFrame::clone() const
	//{
	//	KeyFrame* newKf = new KeyFrame();
	//	newKf->m_time = m_time;
	//	return newKf;
	//}

	f32 KeyFrame::getBlendPecent(const KeyFrame& frameBefore, const KeyFrame& frameAfter, EInterpType interpType)
	{		
		f32 timePct;

		if(interpType==IT_STEP)
		{
			timePct = (m_time>=frameAfter.m_time)?1.0f:0.0f;
		}
		else
		{
			timePct = (f32)(m_time - frameBefore.m_time) / (frameAfter.m_time - frameBefore.m_time);
			if(timePct>1.0f)
				timePct=1.0f;
		}

		f32 blendPct = timePct;

		switch(interpType)
		{
		case IT_STEP://fall through						
		case IT_LINEAR:
			//use blendPct = timePct;
			break;
		case IT_CUBIC:
			blendPct = core::cubicInterp(0.f, 0.f, 1.f, 0.f, timePct);
			break;
		case IT_EASEIN:
			blendPct = core::interpEaseIn(0.f, 1.f, timePct, 2.0f);
			break;
		case IT_EASEOUT:
			blendPct = core::interpEaseOut(0.f, 1.f, timePct, 2.0f);
			break;
		case IT_EASEINOUT:
			blendPct = core::interpEaseInOut(0.f, 1.f, timePct, 2.0f);
			break;
		}

		return blendPct;		
	}

	void KeyFrame::setInterpTypeByString(const stringc& str)
	{
		for(int i=IT_STEP; i<IT_NUM; i++)
		{
			if(str==getInterpTypeString(EInterpType(i)))
			{
				m_interpType = EInterpType(i);
				break;
			}
		}		
	}

	const char* KeyFrame::getInterpTypeString(EInterpType interpType)
	{
		switch(interpType)
		{
		case IT_STEP:						
			return "step";
		case IT_LINEAR:
			return "linear";			
		case IT_CUBIC:
			return "cubic";			
		case IT_EASEIN:
			return "easein";			
		case IT_EASEOUT:
			return "easeout";
		case IT_EASEINOUT:
			return "easeinout";			
		}

		return "unknown";
	}

	void KeyFrame::interpTypeToString(char* str, bool last)
	{
		if(last)
			sprintf(str,"%s",getInterpTypeString(m_interpType));
		else
			sprintf(str,"%s,",getInterpTypeString(m_interpType));
	}

	//----------------------------------------------------
	FloatKeyFrame::FloatKeyFrame()
		:m_value(0.0f)
	{

	}

	void FloatKeyFrame::loadValue(f32* &values, u32 kfIndex)
	{	
		m_value = values[kfIndex];
	}	

	void FloatKeyFrame::valueToString(char* str, bool lastValue)
	{
		if(lastValue)
			sprintf(str,"%.3f",m_value);
		else
			sprintf(str,"%.3f,",m_value);
	}

	KeyFrame& FloatKeyFrame::operator=(const KeyFrame& rhs)
	{
		FloatKeyFrame& kf = (FloatKeyFrame&)rhs;
		m_time = kf.m_time;
		m_value = kf.m_value;
		return *this;
	}

	void FloatKeyFrame::interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter)
	{		
		FloatKeyFrame& kfBefore = (FloatKeyFrame&)frameBefore;
		FloatKeyFrame& kfAfter = (FloatKeyFrame&)frameAfter;

		f32 blendFactor = getBlendPecent(frameBefore, frameAfter, kfBefore.m_interpType);

		m_value = kfBefore.m_value + blendFactor*(kfAfter.m_value-kfBefore.m_value);					
	}

	void FloatKeyFrame::applyToTarget(void *target)
	{
		f32 * targetValue = (f32*)(target);
		*targetValue = m_value;
	}

	KeyFrame* FloatKeyFrame::clone() const
	{
		FloatKeyFrame *newKf = new FloatKeyFrame();
		newKf->m_time = m_time;
		newKf->m_value = m_value;
		return newKf;
	}

	//----------------------------------------------------
	Vector2dfKeyFrame::Vector2dfKeyFrame()
		:m_value(0.0f,0.0f)
	{

	}

	void Vector2dfKeyFrame::loadValue(f32* &values, u32 kfIndex)
	{	
		m_value.X = values[kfIndex*2];
		m_value.Y = values[kfIndex*2+1];
	}

	void Vector2dfKeyFrame::valueToString(char* str, bool lastValue)
	{
		if(lastValue)
			sprintf(str,"%.3f,%.3f",m_value.X,m_value.Y);
		else
			sprintf(str,"%.3f,%.3f,",m_value.X,m_value.Y);
	}

	KeyFrame& Vector2dfKeyFrame::operator=(const KeyFrame& rhs)
	{
		Vector2dfKeyFrame& kf = (Vector2dfKeyFrame&)rhs;
		m_time = kf.m_time;
		m_value.X = kf.m_value.X;
		m_value.Y = kf.m_value.Y;
		return *this;
	}

	void Vector2dfKeyFrame::interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter)
	{		
		Vector2dfKeyFrame& kfBefore = (Vector2dfKeyFrame&)frameBefore;
		Vector2dfKeyFrame& kfAfter = (Vector2dfKeyFrame&)frameAfter;

		f32 blendFactor = getBlendPecent(frameBefore, frameAfter, kfBefore.m_interpType);

		//m_value = kfBefore.m_value + blendFactor*(kfAfter.m_value-kfBefore.m_value);					

		m_value.interpolate(kfAfter.m_value, kfBefore.m_value, blendFactor);
	}

	void Vector2dfKeyFrame::applyToTarget(void *target)
	{
		core::vector2df * targetValue = (core::vector2df*)(target);
		targetValue->X = m_value.X;
		targetValue->Y = m_value.Y;
	}

	KeyFrame* Vector2dfKeyFrame::clone() const
	{
		Vector2dfKeyFrame *newKf = new Vector2dfKeyFrame();
		newKf->m_time = m_time;
		newKf->m_value = m_value;
		return newKf;
	}

	//----------------------------------------------------
	Vector3dfKeyFrame::Vector3dfKeyFrame()
		:m_value(0.0f,0.0f,0.0f)
	{

	}

	void Vector3dfKeyFrame::loadValue(f32* &values, u32 kfIndex)
	{	
		m_value.X = values[kfIndex*3];
		m_value.Y = values[kfIndex*3+1];
		m_value.Z = values[kfIndex*3+2];
	}

	void Vector3dfKeyFrame::valueToString(char* str, bool lastValue)
	{
		if(lastValue)
			sprintf(str,"%.3f,%.3f,%.3f",m_value.X,m_value.Y,m_value.Z);
		else
			sprintf(str,"%.3f,%.3f,%.3f,",m_value.X,m_value.Y,m_value.Z);
	}

	KeyFrame& Vector3dfKeyFrame::operator=(const KeyFrame& rhs)
	{
		Vector3dfKeyFrame& kf = (Vector3dfKeyFrame&)rhs;
		m_time = kf.m_time;
		m_value.X = kf.m_value.X;
		m_value.Y = kf.m_value.Y;
		m_value.Z = kf.m_value.Z;
		return *this;
	}

	void Vector3dfKeyFrame::interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter)
	{		
		Vector3dfKeyFrame& kfBefore = (Vector3dfKeyFrame&)frameBefore;
		Vector3dfKeyFrame& kfAfter = (Vector3dfKeyFrame&)frameAfter;
		
		f32 blendFactor = getBlendPecent(frameBefore, frameAfter, kfBefore.m_interpType);

		//m_value = kfBefore.m_value + blendFactor*(kfAfter.m_value-kfBefore.m_value);					

		m_value.interpolate(kfAfter.m_value, kfBefore.m_value, blendFactor);
	}

	void Vector3dfKeyFrame::applyToTarget(void *target)
	{
		core::vector3df * targetValue = (core::vector3df*)(target);
		targetValue->X = m_value.X;
		targetValue->Y = m_value.Y;
		targetValue->Z = m_value.Z;
	}


	KeyFrame* Vector3dfKeyFrame::clone() const
	{
		Vector3dfKeyFrame *newKf = new Vector3dfKeyFrame();
		newKf->m_time = m_time;
		newKf->m_value = m_value;
		return newKf;
	}

} //end namespace anim
} //end namespace flt