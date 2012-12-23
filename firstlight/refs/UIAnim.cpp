#include "UIAnim.h"
#include "IUIObject.h"

UITrack::UITrack()
:m_target(0)
{
}

UITrack::~UITrack()
{
	for (unsigned int i = 0; i < m_keyframes.size(); ++i)
	{		
		delete m_keyframes[i];
	}
	m_keyframes.clear();

	m_target = 0;
}

int UITrack::GetKeyFrameCount() const
{
	return m_keyframes.size();
}

UIKeyFrame* UITrack::GetKeyFrame(int idx)
{
	return m_keyframes[idx];
}

const UIKeyFrame* UITrack::GetKeyFrame(int idx) const
{
	return m_keyframes[idx];
}

void UITrack::Apply(float time)
{
	if(m_target!=0)
	{		
		UIKeyFrame frame;

		GetFrame(time, frame);

		switch(m_type)
		{
		case TT_POS2D:
			m_target->SetPos(frame.pos.x, frame.pos.y);
			break;
		case TT_SCALE:
			m_target->SetScale(frame.scale);
			break;
		case TT_ROTATION:
			//
			break;
		}		
	}
}

bool UITrack::GetFrame(float time, UIKeyFrame& frame) const
{
	std::vector<UIKeyFrame*>::const_iterator iteratorCoreKeyframeBefore;
	std::vector<UIKeyFrame*>::const_iterator iteratorCoreKeyframeAfter;

	// get the keyframe after the requested time
	iteratorCoreKeyframeAfter = GetUpperBound(time);

	// check if the time is after the last keyframe
	if(iteratorCoreKeyframeAfter == m_keyframes.end())
	{
		// return the last keyframe
		--iteratorCoreKeyframeAfter;

		switch(m_type)
		{
		case TT_POS2D:
			frame.pos = (*iteratorCoreKeyframeAfter)->pos;
			break;
		case TT_SCALE:
			frame.scale = (*iteratorCoreKeyframeAfter)->scale;
			break;
		case TT_ROTATION:
			frame.rotation = (*iteratorCoreKeyframeAfter)->rotation;
		}		

		return true;
	}

	// check if the time is before the first keyframe
	if(iteratorCoreKeyframeAfter == m_keyframes.begin())
	{
		// return the first keyframe
		switch(m_type)
		{
		case TT_POS2D:
			frame.pos = (*iteratorCoreKeyframeAfter)->pos;
			break;
		case TT_SCALE:
			frame.scale = (*iteratorCoreKeyframeAfter)->scale;
			break;
		case TT_ROTATION:
			frame.rotation = (*iteratorCoreKeyframeAfter)->rotation;
		}		

		return true;
	}

	// get the keyframe before the requested one
	iteratorCoreKeyframeBefore = iteratorCoreKeyframeAfter;
	--iteratorCoreKeyframeBefore;

	// get the two keyframe pointers
	UIKeyFrame *pCoreKeyframeBefore;
	pCoreKeyframeBefore = *iteratorCoreKeyframeBefore;
	UIKeyFrame *pCoreKeyframeAfter;
	pCoreKeyframeAfter = *iteratorCoreKeyframeAfter;

	// calculate the blending factor between the two keyframe states
	float blendFactor;
	blendFactor = (time - pCoreKeyframeBefore->time) / (pCoreKeyframeAfter->time - pCoreKeyframeBefore->time);

	// blend between the two keyframes
	switch(m_type)
	{
	case TT_POS2D:
		frame.pos.x = pCoreKeyframeBefore->pos.x + (pCoreKeyframeAfter->pos.x - pCoreKeyframeBefore->pos.x)*blendFactor;
		frame.pos.y = pCoreKeyframeBefore->pos.y + (pCoreKeyframeAfter->pos.y - pCoreKeyframeBefore->pos.y)*blendFactor;
		break;
	case TT_SCALE:
		frame.scale = pCoreKeyframeBefore->scale + blendFactor*(pCoreKeyframeAfter->scale - pCoreKeyframeBefore->scale);
		break;
	case TT_ROTATION:
		frame.rotation = pCoreKeyframeBefore->rotation + blendFactor*(pCoreKeyframeAfter->rotation - pCoreKeyframeBefore->rotation);//need change to quater
	}	

	return true;
}

std::vector<UIKeyFrame *>::const_iterator UITrack::GetUpperBound(float time) const
{
	int lowerBound = 0;
	int upperBound = m_keyframes.size()-1;	

	while(lowerBound<upperBound-1)
	{
		int middle = (lowerBound+upperBound)/2;

		if(time >= m_keyframes[middle]->time)
		{
			lowerBound=middle;
		}
		else
		{
			upperBound=middle;
		}		
	}

	return m_keyframes.begin() + upperBound;
}

/////////////////////////////////////////////////////////////////////////////////

UIAnim::UIAnim()
:m_bPaused(false),
 m_bLoop(false),
 m_maxTime(0.0f),
 m_time(0.0f)
{

}

UIAnim::~UIAnim()
{
	for (unsigned int i = 0; i < m_tracks.size(); ++i)
	{		
		delete m_tracks[i];
	}
	m_tracks.clear();	
}

void UIAnim::Update(float dt)
{
	if(m_bPaused)
		return;

	for (unsigned int i = 0; i < m_tracks.size(); ++i)
	{		
		m_tracks[i]->Apply(m_time);
	}

	m_time+=dt;
	if(m_time>m_maxTime)
	{
		if(m_bLoop)
			m_time=0.0f;
		else		
			m_time-=dt;					
	}

}