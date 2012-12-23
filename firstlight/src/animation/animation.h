#ifndef FLT_ANIMATION_H
#define FLT_ANIMATION_H

#include "common.h"
#include "BaseObject.h"
#include "animation/animationTrack.h"
#include "animation/animationChannel.h"
#include "externs/tinyxml/tinyxml.h"

//#include "core/LiteArray.h"
//#include "core/LiteString.h"

namespace flt
{
namespace anim
{

/*
//data type for track (keyframe datas), determine the size of one key frame data
enum EDataType
{
	DT_BYTE,
	DT_FLOAT,		// 1 float
	DT_FLOAT4X4,	//16 floats
	DT_RGB,			//3 bytes, converted to 3 floats //关键帧数据用bytes存储减少内存，计算时使用float
	DT_ARGB,		//4 bytes, converted to 4 floats
	DT_STRING,
};

struct SKeyFrameData
{	
	union
	{
		core::LiteArray<core::LiteString> m_sData;
		core::LiteArray<void *> m_pData;
		core::LiteArray<f32> m_fData;
		core::LiteArray<s32> m_iData;
		core::LiteArray<u8> m_bData;
	};
};

enum EInterpolType
{
	IT_STEP,
	IT_LINEAR,
	IT_HERMIT,
};

struct STrack
{
	stringc m_name;	

	s32 m_dataType;	

	core::LiteArray<s32> m_times;
	core::LiteArray<SKeyFrameData> m_keyFrameDatas;	
	core::LiteArray<u8> m_interpolTypes;
};

namespace trackType
{
	enum
	{
		Track_ColorRGB = 0,
		Track_ColorAlpha,
		Track_Pos2D,

		Track_Built_In_TrackType_Count
	};
};

struct SChannel
{	
	stringc m_animableName;
	stringc m_targetName;	
	stringc m_trackName;	
};

struct SAnimation
{
	stringc m_name;
	bool m_isLoop;

	core::LiteArray<STrack> m_tracks;
	core::LiteArray<SChannel> m_channels;
};

*/

//////////////////////////////////////////////////////////

class Animation: public BaseObject
{
public:

	Animation();
	~Animation();

	bool loadFromXML(TiXmlElement *xmlNodeAnim);

	void saveToXML(TiXmlElement *xmlNodeAnim);

	void addChannel(AnimationChannel *channel) { m_channels.push_back(channel); }

	void addTrack(AnimationTrack *track) { m_tracks.push_back(track); }

	int getChannelNum() { return (int)m_channels.size(); }

	AnimationChannel* getChannel(int index) { return m_channels[index]; }

	int getTrackNum() { return (int)m_tracks.size(); }

	bool isLoop() { return m_isLoop; }

	void bindChannels();

	void bindTrackToChannel(AnimationChannel* channel);	

private:
	array_t<AnimationChannel*> m_channels;
	array_t<AnimationTrack*> m_tracks;

	bool m_isLoop;

	void clearAll();

};


} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMATION_H