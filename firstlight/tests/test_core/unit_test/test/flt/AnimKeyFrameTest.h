#ifndef TEST_FLT_ANIM_KEY_FRAME_H
#define TEST_FLT_ANIM_KEY_FRAME_H

#include "common.h"
#include "animation/animation.h"
#include "animation/keyframe.h"
#include <gtest/gtest.h> 

namespace flt_test
{
	using namespace flt;	

	class AnimKeyFrameTest : public testing::Test
	{
	protected:   
		AnimKeyFrameTest() {   
			// You can do set-up work for each test here.   
		}   
		virtual ~AnimKeyFrameTest() {   
			// You can do clean-up work that doesn't throw exceptions here.   
		}   

		virtual void SetUp() {   
			// Code here will be called immediately after the constructor (right   
			// before each test).   
		}   
		virtual void TearDown() {   
			// Code here will be called immediately after each test (right   
			// before the destructor).   
		}   


	};

	void testFindKeyFrameAfterTime(const anim::AnimationTrack &track, float time, float exceptedTime)
	{
		array_t<anim::KeyFrame *>::const_iterator iter;
		anim::FloatKeyFrame* kfFind=0;

		iter = track.getUpperBound(time);
		kfFind = (anim::FloatKeyFrame*)(*iter);
		EXPECT_EQ(exceptedTime, kfFind->getTime());
		printf("Find key frame after time %f: kfTime=%f, kfValue=%f ExpTime: %f\n", time, kfFind->getTime(), kfFind->getValue(), exceptedTime);
	}

	TEST_F(AnimKeyFrameTest, BasicTest) 
	{
		anim::AnimationTrack track;
		
		anim::FloatKeyFrame* kf = new anim::FloatKeyFrame();
		kf->setTime(0);
		kf->setValue(10);
		track.addKeyFrame(kf);

		kf = new anim::FloatKeyFrame();
		kf->setTime(5);
		kf->setValue(50);
		track.addKeyFrame(kf);

		kf = new anim::FloatKeyFrame();
		kf->setTime(10);
		kf->setValue(100);
		track.addKeyFrame(kf);

		kf = new anim::FloatKeyFrame();
		kf->setTime(20);
		kf->setValue(160);
		track.addKeyFrame(kf);

		testFindKeyFrameAfterTime(track, -10, 0);
		testFindKeyFrameAfterTime(track, 0, 5);
		testFindKeyFrameAfterTime(track, 3, 5);
		testFindKeyFrameAfterTime(track, 5, 10);
		testFindKeyFrameAfterTime(track, 7, 10);
		testFindKeyFrameAfterTime(track, 10, 20);
		testFindKeyFrameAfterTime(track, 15, 20);
		testFindKeyFrameAfterTime(track, 20, 20);
		testFindKeyFrameAfterTime(track, 30, 20);


	}   

}
#endif //TEST_FLT_ANIM_KEY_FRAME_H
