#ifndef KF_ANIM_H
#define KF_ANIM_H

#include "firstlight.h"
using namespace flt;

namespace kfAnim
{

class IAnimable
{
public:
	virtual ~IAnimable(){}

	virtual void setPos3D(const core::vector3df& pos) = 0;
	virtual void setRot3D(const core::quaternion& rot) = 0;
	virtual bool isMatch(const stringc& name) = 0;
};

enum TrackType
{
	TT_POS3D,
	TT_ROT3D,
	TT_UNKNOWN,
};

//enum InterpolationType
//{
//	IT_STEP,
//	IT_LINEAR,
//	IT_HERMITE,
//};
//now only use linear for pic dance

struct KeyFrame
{
	u32 TimeMs;

	core::vector3df Pos3D;
	core::quaternion Rot3D;	
};

class Track
{
public:

	friend class Anim;

	Track();
	~Track();

	void setTarget(IAnimable* target) { m_target = target; }

	int getKeyFrameCount() const;
	
	KeyFrame* getKeyFrame(u32 index);
	const KeyFrame* getKeyFrame(u32 index) const;

	void apply(u32 timeMs);

	bool getFrame(u32 timeMs, KeyFrame& frame) const;

	u32 getTotalTimeMs();

private:
	TrackType m_type;	
	std::vector<KeyFrame*> m_keyframes;
	IAnimable* m_target;
	flt::stringc m_targetName;

	std::vector<KeyFrame *>::const_iterator getUpperBound(u32 timeMs) const;
};

FLT_FORWARD_PTR(Anim)

class Anim: public BaseObject
{
public:
	Anim();
	~Anim();

	void load(flt::FileStream* file);


	void update(u32 dtMs);

	void start(bool loop)
	{
		m_bLoop = loop;
		m_bPaused=false;
		m_playTime = 0;
	}

	void pause()
	{
		m_bPaused=true;
	}
	
	void resume()
	{
		m_bPaused=false;
	}	
	
	bool isOver() 
	{ 
		return !m_bLoop && (m_playTime>=m_maxTime); 
	}

	void bindTracksTo(IAnimable* target);

private:	
	std::vector<Track*> m_tracks;

	u32 m_maxTime; //max time of all tracks
	bool m_bLoop; //loop when reach max time
	bool m_bPaused;

	u32 m_playTime;	
};

}//end namespace kfAnim

#endif //KF_ANIM_H