#ifndef FLT_I_ANIMABLE_H
#define FLT_I_ANIMABLE_H

#include "common.h"

namespace flt
{
namespace anim
{

class IAnimable
{
public:
	virtual ~IAnimable(){}

	virtual const stringc& getName() const = 0;
	
	//virtual void onAnimate(f32 dt) = 0;

	//ͨ�����ֵõ���Ҫ������Ŀ�������ָ�룬���磺uibutton->getTarget("color");
	//uibutton��һ��IAnimable������Ӧ���ܷ���"color"��Ӧ�Ĳ�����ָ��
	virtual void* getTarget(const stringc& name) = 0;	

	virtual void notifyTargetModified() = 0;
};

class IAnimableProvider
{
public:
	virtual ~IAnimableProvider(){}
	virtual int getAnimableNum() = 0;
	virtual IAnimable* getAnimable(int index) = 0;
};

} //end namespace anim
} //end namespace flt

#endif //FLT_I_ANIMABLE_H