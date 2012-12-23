//! A wrapper of sound engine to provide a common interface to play sound

#ifndef FLT_SOUND_PLAY_H
#define FLT_SOUND_PLAY_H

namespace flt
{
namespace sound
{
	void initSoundSystem();

	void shutDownSoundSystem();

	void preloadBackgroundMusic(const char* pszFilePath);

	void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);
	
	void stopBackgroundMusic(bool bReleaseData = false);

	void pauseBackgroundMusic();

	void resumeBackgroundMusic();

	bool isBackgroundMusicPlaying();

	/**
	@brief The volume of the background music max value is 1.0,the min value is 0.0
	*/
	float getBackgroundMusicVolume();

	/**
	@brief set the volume of background music
	@param volume must be in 0.0~1.0
	*/
	void setBackgroundMusicVolume(float volume);

	/**
	@brief The volume of the effects max value is 1.0,the min value is 0.0
	*/
	float getEffectsVolume();

	/**
	@brief set the volume of sound effecs
	@param volume must be in 0.0~1.0
	*/
	void setEffectsVolume(float volume);

	// for sound effects
	/**
	@brief Play sound effect
	@param pszFilePath The path of the effect file,or the FileName of T_SoundResInfo
	*/
	unsigned int playEffect(const char* pszFilePath);

	/**
	@brief Stop playing sound effect
	@param nSoundId The return value of function playEffect
	*/
	void stopEffect(unsigned int nSoundId);

	/**
	@brief  		preload a compressed audio file
	@details	    the compressed audio will be decode to wave, then write into an 
	internal buffer in SimpleaudioEngine
	*/
	void preloadEffect(const char* pszFilePath);

	/**
	@brief  		unload the preloaded effect from internal buffer
	@param[in]		pszFilePath		The path of the effect file,or the FileName of T_SoundResInfo
	*/
	void unloadEffect(const char* pszFilePath);

} //end namespace sound
} //end namespace flt


#endif //FLT_SOUND_PLAY_H
