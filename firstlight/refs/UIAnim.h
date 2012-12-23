#ifndef __UI_ANIM_H__
#define __UI_ANIM_H__

#include <vector>

class IUIObject;

enum EUITrackType
{
	TT_POS2D,
	TT_SCALE,
	TT_ROTATION,
};

enum EUITrackInterpolationType
{
	IT_STEP,
	IT_LINEAR,
	IT_HERMITE,
};

struct SPos2D
{
	int x,y;
};


struct UIKeyFrame
{

	float time;

	union
	{
		SPos2D pos;
		float scale;
		float rotation;
	};	
};

class UITrack
{
public:
	UITrack();
	~UITrack();

	void SetTarget(IUIObject* target) { m_target = target; }

	int GetKeyFrameCount() const;
	UIKeyFrame* GetKeyFrame(int idx);
	const UIKeyFrame* GetKeyFrame(int idx) const;

	void Apply(float time);

	bool GetFrame(float time, UIKeyFrame& frame) const;	

private:
	EUITrackType	m_type;
	EUITrackInterpolationType m_interpolation;
	std::vector<UIKeyFrame*> m_keyframes;
	IUIObject* m_target;

	std::vector<UIKeyFrame *>::const_iterator GetUpperBound(float time) const;

};

class UIAnim
{
public:
	UIAnim();
	~UIAnim();

	bool Load(const char* file);

	void Reset();
	void Update(float dt);
	void Pause(){m_bPaused=true;}
	void Resume(){m_bPaused=false;}	
	void SetLoop(bool loop) { m_bLoop = loop; }	
	bool IsOver() { return !m_bLoop && (m_time>=m_maxTime); }


private:
	std::string m_name;
	std::vector<UITrack*> m_tracks;

	float m_maxTime; //max time of all tracks
	bool m_bLoop; //loop when reach max time
	bool m_bPaused;

	float m_time;	
};

#endif //__UI_ANIM_H__