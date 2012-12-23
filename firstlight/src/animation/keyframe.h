#ifndef FLT_ANIMATION_KEY_FRAME_H
#define FLT_ANIMATION_KEY_FRAME_H

#include "common.h"
#include "core/vector2d.h"
#include "core/vector3d.h"

namespace flt
{
namespace anim
{

enum EInterpType
{
	IT_STEP,
	IT_LINEAR,
	IT_CUBIC,
	IT_EASEIN,
	IT_EASEOUT,
	IT_EASEINOUT,
	IT_NUM,
};

class KeyFrame
{
public:
	KeyFrame();
	virtual ~KeyFrame() {}

	void setTime(f32 time) { m_time = time; }

	f32 getTime() const { return m_time; }

	void setInterpType(EInterpType interpType) { m_interpType = interpType; }

	f32 getBlendPecent(const KeyFrame& frameBefore, const KeyFrame& frameAfter, EInterpType interpType);

	virtual u32 getDataElementNum()=0;

	virtual void loadValue(f32* &values, u32 kfIndex)=0;

	virtual void valueToString(char* str, bool lastValue)=0;

	virtual KeyFrame& operator=(const KeyFrame& rhs)=0;

	virtual void interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter)=0;

	virtual void applyToTarget(void* target)=0;

	virtual KeyFrame* clone() const=0;

	void setInterpTypeByString(const stringc& str);

	const char* getInterpTypeString(EInterpType interpType);

	void interpTypeToString(char* str, bool last);

protected:
	f32 m_time;
	EInterpType m_interpType;
};

class FloatKeyFrame : public KeyFrame
{
public:
	FloatKeyFrame();
	~FloatKeyFrame() {}

	virtual u32 getDataElementNum() { return 1; }

	virtual void loadValue(f32* &values, u32 kfIndex);

	virtual void valueToString(char* str, bool lastValue);

	virtual KeyFrame& operator=(const KeyFrame& rhs);

	virtual void interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter);

	virtual void applyToTarget(void* target);

	virtual KeyFrame* clone() const;

	f32 getValue() { return m_value; }

	void setValue(f32 value) { m_value = value; }

private:
	f32 m_value;
};

class Vector2dfKeyFrame : public KeyFrame
{
public:
	Vector2dfKeyFrame();
	~Vector2dfKeyFrame() {}

	virtual u32 getDataElementNum() { return 2; }

	virtual void loadValue(f32* &values, u32 kfIndex);

	virtual void valueToString(char* str, bool lastValue);

	virtual KeyFrame& operator=(const KeyFrame& rhs);

	virtual void interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter);

	virtual void applyToTarget(void* target);

	virtual KeyFrame* clone() const;

	const core::vector2df& getValue() const { return m_value; }

	void setValue(const core::vector2df& value) { m_value=value; }

private:
	core::vector2df m_value;
};

class Vector3dfKeyFrame: public KeyFrame
{
public:
	Vector3dfKeyFrame();
	~Vector3dfKeyFrame() {}

	virtual u32 getDataElementNum() { return 3; }

	virtual void loadValue(f32* &values, u32 kfIndex);

	virtual void valueToString(char* str, bool lastValue);

	virtual KeyFrame& operator=(const KeyFrame& rhs);

	virtual void interpolateValueFrom(const KeyFrame& frameBefore, const KeyFrame& frameAfter);

	virtual void applyToTarget(void* target);

	virtual KeyFrame* clone() const;

	const core::vector3df& getValue() const { return m_value; }

	void setValue(const core::vector3df& value) { m_value=value; }	

private:
	core::vector3df m_value;
};

} //end namespace anim
} //end namespace flt

#endif //FLT_ANIMATION_KEY_FRAME_H