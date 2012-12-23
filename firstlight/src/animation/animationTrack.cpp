#include "animationTrack.h"
#include "utils/StringUtil.h"

namespace flt
{
namespace anim
{
	AnimationTrack::AnimationTrack()
		:m_dataType(ATDT_NULL),m_computedFrame(0)
	{

	}

	AnimationTrack::~AnimationTrack()
	{
		clearAll();
	}

	void AnimationTrack::clearAll()
	{
		delete m_computedFrame;

		for(int i=0, kfNum=(int)m_keyFrames.size(); i<kfNum; ++i)
		{
			delete m_keyFrames[i];			
		}
		m_keyFrames.clear();		
	}

	EAnimTrackDataType AnimationTrack::stringToDataType(const stringc& str)
	{
		if(str=="float")
			return ATDT_FLOAT;
		else if(str=="vector2f")
			return ATDT_VECTOR2F;
		else if(str=="vector3f")
			return ATDT_VECTOR3F;
		else
			return ATDT_NULL;
	}

	void AnimationTrack::dataTypeToString(EAnimTrackDataType dataType, stringc& str)
	{
		switch(dataType)
		{
		case ATDT_FLOAT:
			str = "float";
			break;
		case ATDT_VECTOR2F:
			str = "vector2f";
			break;
		case ATDT_VECTOR3F:
			str = "vector3f";
			break;
		default:
			str = "";
			break;
		}
	}

	KeyFrame* AnimationTrack::createKeyFrame()
	{
		KeyFrame* kf = 0;

		switch(m_dataType)
		{
		case ATDT_FLOAT:
			kf = new FloatKeyFrame();
			break;
		case ATDT_VECTOR2F:
			kf = new Vector2dfKeyFrame();
			break;
		case ATDT_VECTOR3F:
			kf = new Vector3dfKeyFrame();
			break;
		default:
			break;
		}

		return kf;
	}

	bool AnimationTrack::loadFromXML(TiXmlElement *xmlNodeTrack)
	{
		xmlNodeTrack->QueryStringAttribute("name",&m_name);			
		stringc dataType;
		xmlNodeTrack->QueryStringAttribute("dataType",&dataType);
		m_dataType = stringToDataType(dataType);
		
		TiXmlHandle hXmlNode(xmlNodeTrack);

		//load keyframes data
		TiXmlElement* xmlNodeTimes = hXmlNode.FirstChild( "times" ).Element();
		if(xmlNodeTimes!=0)
		{
			int kfNum = 0;

			//times
			const char* timesStr = xmlNodeTimes->GetText();
			if(timesStr!=0)
			{
				float *times=0;
				kfNum = StringUtil::splitOutFloats(times, timesStr);

				for(int i=0; i<kfNum; ++i)
				{
					KeyFrame* kf = createKeyFrame();
					if(kf!=0)
					{
						kf->setTime(times[i]);
						m_keyFrames.push_back(kf);
					}
				}

				delete times;
			}

			//at least has 1 kf
			if(kfNum>0)
			{
				//KF datas
				float *kfDatas=0;
				TiXmlElement* xmlNodeKFs = hXmlNode.FirstChild( "keyframes" ).Element();
				if(xmlNodeKFs!=0)
				{
					int valueNum = StringUtil::splitOutFloats(kfDatas, xmlNodeKFs->GetText());
					u32 dataElemNum = m_keyFrames[0]->getDataElementNum();
					FLT_ASSERT(dataElemNum>0 && valueNum/dataElemNum==kfNum);					
					
					for(int i=0; i<kfNum; ++i)
					{
						m_keyFrames[i]->loadValue(kfDatas, i);						
					}

					delete kfDatas;
				}

				//interp datas
				TiXmlElement* xmlNodeInterps = hXmlNode.FirstChild( "interps" ).Element();
				if(xmlNodeInterps!=0)
				{
					array_t<stringc> strs;
					strs = StringUtil::split(xmlNodeInterps->GetText(), ",", kfNum);
										
					FLT_ASSERT(strs.size()==kfNum);					

					for(int i=0; i<kfNum; ++i)
					{
						m_keyFrames[i]->setInterpTypeByString(strs[i]);						
					}					
				}
			}

			//create a computed frame, for interpolation
			m_computedFrame = createKeyFrame();
		}

		return true;
	}

	void AnimationTrack::saveToXML(TiXmlElement *xmlNodeTrack)
	{
		xmlNodeTrack->SetAttribute("name",m_name.c_str());	
		stringc dataType;
		dataTypeToString(m_dataType, dataType);
		xmlNodeTrack->SetAttribute("dataType",dataType);

		s32 kfNum = (s32)m_keyFrames.size();

		//save times
		TiXmlElement * timesNode = new TiXmlElement( "times" );  
		xmlNodeTrack->LinkEndChild( timesNode );
		stringc tempStr;
		char tempBuf[256];
		for(s32 i=0; i<kfNum; ++i)
		{
			f32 time = m_keyFrames[i]->getTime();
			if(i!=kfNum-1)
				sprintf(tempBuf,"%.3f,",time);
			else
				sprintf(tempBuf,"%.3f",time);
			tempStr.append(tempBuf);
		}
		timesNode->LinkEndChild(new TiXmlText(tempStr));

		//save kf datas
		TiXmlElement * kfsNode = new TiXmlElement( "keyframes" );  
		xmlNodeTrack->LinkEndChild( kfsNode );
		tempStr.clear();
		for(int i=0; i<kfNum; ++i)
		{
			m_keyFrames[i]->valueToString(tempBuf,i==kfNum-1);			
			tempStr.append(tempBuf);
		}
		kfsNode->LinkEndChild(new TiXmlText(tempStr));

		//save interp datas (per frame)
		TiXmlElement *interpsNode = new TiXmlElement("interps");
		xmlNodeTrack->LinkEndChild( interpsNode );
		tempStr.clear();
		for(int i=0; i<kfNum; ++i)
		{
			m_keyFrames[i]->interpTypeToString(tempBuf,i==kfNum-1);			
			tempStr.append(tempBuf);
		}
		interpsNode->LinkEndChild(new TiXmlText(tempStr));
	}

	void AnimationTrack::apply(f32 timeMs, void* &target)
	{		
		m_computedFrame->setTime(timeMs);

		getFrame(timeMs, *m_computedFrame);

		m_computedFrame->applyToTarget(target);		
	}

	f32 AnimationTrack::getTotalTimeMs()
	{
		s32 kfNum = (s32)m_keyFrames.size();

		if(kfNum>0)
			return m_keyFrames[kfNum-1]->getTime();
		else
			return 0.0f;
	}

	array_t<KeyFrame *>::const_iterator AnimationTrack::getUpperBound(f32 timeMs) const
	{
		int lowerBound = 0;
		int upperBound = m_keyFrames.size()-1;	
		if(upperBound<=0)
			return m_keyFrames.begin();
		if(timeMs<m_keyFrames[0]->getTime())
			return m_keyFrames.begin();

		while(lowerBound<upperBound-1)
		{
			int middle = (lowerBound+upperBound)/2;

			if(timeMs >= m_keyFrames[middle]->getTime())
			{
				lowerBound=middle;
			}
			else
			{
				upperBound=middle;
			}		
		}

		return m_keyFrames.begin() + upperBound;
	}

	bool AnimationTrack::getFrame(f32 timeMs, KeyFrame& frame) const
	{
		array_t<KeyFrame*>::const_iterator iterKeyframeBefore;
		array_t<KeyFrame*>::const_iterator iterKeyframeAfter;

		// get the key frame after the requested time
		iterKeyframeAfter = getUpperBound(timeMs);

		// check if the time is after the last key frame
		if(iterKeyframeAfter == m_keyFrames.end())
		{
			// return the last key frame
			--iterKeyframeAfter;

			frame = *(*iterKeyframeAfter);
								
			return true;
		}

		// check if the time is before the first key frame
		if(iterKeyframeAfter == m_keyFrames.begin())
		{
			// return the first key frame
			frame = *(*iterKeyframeAfter);			

			return true;
		}

		// get the key frame before the requested one
		iterKeyframeBefore = iterKeyframeAfter;
		--iterKeyframeBefore;

		// get the two key frame
		KeyFrame& keyframeBefore = *(*iterKeyframeBefore);
		KeyFrame& keyframeAfter = *(*iterKeyframeAfter);

		// blend between the two key frames
		//Note: use interp type of compute time blend pct, and use linear for value interp, and should take care of rotation
		frame.interpolateValueFrom(keyframeBefore, keyframeAfter);
		
		return true;
	}

} //end namespace anim
} //end namespace flt