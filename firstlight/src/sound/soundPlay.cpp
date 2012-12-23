#include "soundPlay.h"
#include "externs/CocosDenshion/include/SimpleAudioEngine.h"

using namespace CocosDenshion;

namespace flt
{
namespace sound
{
	void initSoundSystem()
	{
	}

	void shutDownSoundSystem()
	{
		//Note: call it will cause memory leak (reported by VLD), it's strange!
		//SimpleAudioEngine::end();
	}

	void preloadBackgroundMusic(const char* pszFilePath)
	{
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(pszFilePath);
	}

	void playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath, bLoop);
	}

	void stopBackgroundMusic(bool bReleaseData)
	{
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
	}

	void pauseBackgroundMusic()
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}

	void resumeBackgroundMusic()
	{
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	}

	bool isBackgroundMusicPlaying()
	{
		return SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
	}

	/**
	@brief The volume of the background music max value is 1.0,the min value is 0.0
	*/
	float getBackgroundMusicVolume()
	{
		return SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
	}

	/**
	@brief set the volume of background music
	@param volume must be in 0.0~1.0
	*/
	void setBackgroundMusicVolume(float volume)
	{
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume);
	}

	/**
	@brief The volume of the effects max value is 1.0,the min value is 0.0
	*/
	float getEffectsVolume()
	{
		return SimpleAudioEngine::sharedEngine()->getEffectsVolume();
	}

	/**
	@brief set the volume of sound effecs
	@param volume must be in 0.0~1.0
	*/
	void setEffectsVolume(float volume)
	{
		SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume);
	}

	// for sound effects
	/**
	@brief Play sound effect
	@param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
	*/
	unsigned int playEffect(const char* pszFilePath)
	{
		return SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath);
	}

	/**
	@brief Stop playing sound effect
	@param nSoundId The return value of function playEffect
	*/
	void stopEffect(unsigned int nSoundId)
	{
		SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
	}

	/**
	@brief  		preload a compressed audio file
	@details	    the compressed audio will be decode to wave, then write into an 
	internal buffer in SimpleaudioEngine
	*/
	void preloadEffect(const char* pszFilePath)
	{
		SimpleAudioEngine::sharedEngine()->preloadEffect(pszFilePath);
	}

	/**
	@brief  		unload the preloaded effect from internal buffer
	@param[in]		pszFilePath		The path of the effect file,or the FileName of T_SoundResInfo
	*/
	void unloadEffect(const char* pszFilePath)
	{
		SimpleAudioEngine::sharedEngine()->unloadEffect(pszFilePath);
	}

} //end namespace sound
} //end namespace flt


