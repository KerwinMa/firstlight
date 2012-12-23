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

	//通过名字得到需要动画的目标参数的指针，例如：uibutton->getTarget("color");
	//uibutton是一个IAnimable，并且应该能返回"color"对应的参数的指针
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