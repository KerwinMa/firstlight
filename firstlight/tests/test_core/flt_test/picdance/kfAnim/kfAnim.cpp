#include "kfAnim.h"

namespace kfAnim
{

Track::Track()
:m_target(0)
{
}

Track::~Track()
{
	for (u32 i = 0; i < m_keyframes.size(); ++i)
	{		
		delete m_keyframes[i];
	}
	m_keyframes.clear();

	m_target = 0;
}

int Track::getKeyFrameCount() const
{
	return m_keyframes.size();
}

KeyFrame* Track::getKeyFrame(u32 idx)
{
	return m_keyframes[idx];
}

const KeyFrame* Track::getKeyFrame(u32 idx) const
{
	return m_keyframes[idx];
}

void Track::apply(u32 time)
{
	if(m_target!=0)
	{		
		KeyFrame frame;

		getFrame(time, frame);

		switch(m_type)
		{
		case TT_POS3D:
			m_target->setPos3D(frame.Pos3D);
			break;
		case TT_ROT3D:
			m_target->setRot3D(frame.Rot3D);
		default:			
			break;
		}		
	}
}

bool Track::getFrame(u32 timeMs, KeyFrame& frame) const
{
	std::vector<KeyFrame*>::const_iterator iteratorCoreKeyframeBefore;
	std::vector<KeyFrame*>::const_iterator iteratorCoreKeyframeAfter;

	// get the keyframe after the requested time
	iteratorCoreKeyframeAfter = getUpperBound(timeMs);

	// check if the time is after the last keyframe
	if(iteratorCoreKeyframeAfter == m_keyframes.end())
	{
		// return the last keyframe
		--iteratorCoreKeyframeAfter;

		switch(m_type)
		{
		case TT_POS3D:
			frame.Pos3D = (*iteratorCoreKeyframeAfter)->Pos3D;
			break;
		case TT_ROT3D:
			frame.Rot3D = (*iteratorCoreKeyframeAfter)->Rot3D;
			break;
		default:
			break;
		}		

		return true;
	}

	// check if the time is before the first keyframe
	if(iteratorCoreKeyframeAfter == m_keyframes.begin())
	{
		// return the first keyframe
		switch(m_type)
		{
		case TT_POS3D:
			frame.Pos3D = (*iteratorCoreKeyframeAfter)->Pos3D;
			break;
		case TT_ROT3D:
			frame.Rot3D = (*iteratorCoreKeyframeAfter)->Rot3D;
			break;
		default:
			break;
		}		

		return true;
	}

	// get the keyframe before the requested one
	iteratorCoreKeyframeBefore = iteratorCoreKeyframeAfter;
	--iteratorCoreKeyframeBefore;

	// get the two keyframe pointers
	KeyFrame *pCoreKeyframeBefore;
	pCoreKeyframeBefore = *iteratorCoreKeyframeBefore;
	KeyFrame *pCoreKeyframeAfter;
	pCoreKeyframeAfter = *iteratorCoreKeyframeAfter;

	// calculate the blending factor between the two keyframe states
	float blendFactor;
	blendFactor = (float)(timeMs - pCoreKeyframeBefore->TimeMs) / (pCoreKeyframeAfter->TimeMs - pCoreKeyframeBefore->TimeMs);
	if(blendFactor>1.0f)
		blendFactor=1.0f;

	// blend between the two keyframes
	switch(m_type)
	{
	case TT_POS3D:
		frame.Pos3D = pCoreKeyframeBefore->Pos3D + blendFactor*(pCoreKeyframeAfter->Pos3D - pCoreKeyframeBefore->Pos3D);		
		break;
	case TT_ROT3D:
		frame.Rot3D.slerp(pCoreKeyframeBefore->Rot3D, pCoreKeyframeAfter->Rot3D, blendFactor);			
		break;
	default:
		break;
	}	

	return true;
}

std::vector<KeyFrame *>::const_iterator Track::getUpperBound(u32 timeMs) const
{
	int lowerBound = 0;
	int upperBound = m_keyframes.size()-1;	

	while(lowerBound<upperBound-1)
	{
		int middle = (lowerBound+upperBound)/2;

		if(timeMs >= m_keyframes[middle]->TimeMs)
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

u32 Track::getTotalTimeMs()
{
	int upperBound = m_keyframes.size()-1;	

	return m_keyframes[upperBound]->TimeMs;
}


/////////////////////////////////////////////////////////////////////////////////


Anim::Anim()
:m_bPaused(false),
 m_bLoop(false),
 m_maxTime(0),
 m_playTime(0)
{

}

Anim::~Anim()
{
	for (u32 i = 0; i < m_tracks.size(); ++i)
	{		
		delete m_tracks[i];
	}
	m_tracks.clear();	
}

void Anim::load(flt::FileStream* file)
{
	Json::Reader reader;
	Json::Value root;

	bool ok = reader.parse(file, root, false);
	FLT_ASSERT(ok);

	if(!ok)
		return;

	Json::Value& tracksData = root["tracks"];
	for(int i=0; i<tracksData.size(); i++)
	{
		Json::Value& trackData = tracksData[i];
		Track* track = new Track();
		m_tracks.push_back(track);

		std::string tmpStr = trackData["type"].asString();
		if(tmpStr=="pos")
		{
			track->m_type = TT_POS3D;
		}
		else if(tmpStr=="rot")
		{
			track->m_type = TT_ROT3D;
		}
		else
		{
			track->m_type = TT_UNKNOWN;
		}

		track->m_targetName = trackData["target"].asString();

		Json::Value& keysData = trackData["keys"];
		int keyNum = keysData.size();
		for(int j=0; j<keyNum; j++)
		{
			Json::Value& keyData = keysData[j];
			KeyFrame* kf = new KeyFrame();
			track->m_keyframes.push_back(kf);
			
			kf->TimeMs = keyData["time"].asInt();

			if(track->m_type==TT_POS3D)
			{
				kf->Pos3D.set(keyData["x"].asDouble(),keyData["y"].asDouble(),keyData["z"].asDouble());
			}
			else if(track->m_type==TT_ROT3D)
			{
				double degree = keyData["degree"].asDouble();
				kf->Rot3D.fromAngleAxis(degree*flt::core::DEGTORAD, flt::core::vector3df(0,0,1));
			}
		}

		if(m_maxTime<track->getTotalTimeMs())
		{
			m_maxTime = track->getTotalTimeMs();
		}
		
	}
}

void Anim::update(u32 dtMs)
{
	if(m_bPaused)
		return;

	for (u32 i = 0; i < m_tracks.size(); ++i)
	{		
		m_tracks[i]->apply(m_playTime);
	}

	m_playTime+=dtMs;
	if(m_playTime>m_maxTime)
	{
		if(m_bLoop)
			m_playTime=0;
		else		
			m_playTime-=dtMs;					
	}
}

void Anim::bindTracksTo(IAnimable* target)
{
	for (u32 i = 0; i < m_tracks.size(); ++i)
	{
		if(target->isMatch(m_tracks[i]->m_targetName))
		{
			m_tracks[i]->setTarget(target);
		}
	}
}

}// namespace kfAnim
