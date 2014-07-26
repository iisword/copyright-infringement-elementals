/***********************************************
* Filename:  		AudioManager.h
* Date:      		7/15/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		The audio manager for the game. It loads and unloads sounds into memory
*					and handles playing music and sound effects. 
************************************************/


#ifndef AUDIOMANAGER_HEADER
#define AUDIOMANAGER_HEADER

#ifndef SERVER_BUILD
#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>
#include <vector>
#include <string>

#define FMOD_SOFTWARE_BUFFER_SIZE	1024
#define FMOD_SAMPLES_OF_BUFFER		10
#define FMOD_MAX_CHANNELS			100

typedef unsigned char AudioFileType;

#define SOUND_ASSEST_FOLDER		"assets\\sounds\\sfx\\"
#define MUSIC_ASSEST_FOLDER		"assets\\sounds\\music\\"

#define AUDIOFILETYPE_SOUND		0
#define AUDIOFILETYPE_STREAM	1

#define AMERROR_NOT_FOUND		-1		//The resource could not be found
#define AMERROR_WRONG_TYPE		-2		//The resource is loaded as a sound instead of a music and vice versa

class CAudioManager
{
	FMOD::System * m_cFmodSystem;
	int m_nNumOfDrivers;
	FMOD_CAPS m_nCaps;
	FMOD_SPEAKERMODE m_eSpeakerMode;

	FMOD::ChannelGroup * m_cMusicGroup;
	FMOD::ChannelGroup * m_cSoundEffectGroup;

	struct TAudioFile
	{
		std::string m_szFilePath;
		std::string m_szShortName;
		FMOD::Sound * m_cSound;
		AudioFileType m_chType;
	};

	std::vector<TAudioFile *> m_vFiles;

	/**********************************
	*	CheckFMODError:			Checks if the FMOD_Result is valid and displays an error if it's not.
	*
	*	In:						FMOD_RESULT eResult - The result to check
	*
	*	Return:					bool
	*
	*	Mod. Date:				7/15/2014
	*	Mod. Initials:			MB
	**********************************/
	static bool CheckFMODError(FMOD_RESULT eResult);

	CAudioManager();
	CAudioManager(CAudioManager const & obj);
	~CAudioManager();
	CAudioManager & operator=(CAudioManager const & obj);
public:
	/**********************************
	*	GetInstance:			Gets the instance of the audio manager.
	*
	*	Return:					CAudioManager *
	*
	*	Mod. Date:				7/16/2014
	*	Mod. Initials:			MB
	**********************************/
	static CAudioManager * GetInstance();

	/**********************************
	*	Initialize:			Initializes the audio system. Will return false if it can't
	*						start the audio system.
	*
	*	Return:             bool
	*
	*	Mod. Date:          7/15/2014
	*	Mod. Initials:      MB
	**********************************/
	bool Initialize();

	/**********************************
	*	Shutdown:			Shutdowns FMOD and clears memory.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/15/2014
	*	Mod. Initials:		MB
	**********************************/
	void Shutdown();

	/**********************************
	*	Update:				Updates the fmod system.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/15/2014
	*	Mod. Initials:		MB
	**********************************/
	void Update();

	/**********************************
	*	LoadSound:			Loads a sound effect into memory.
	*
	*	In:					const char * szShortName - The short name for the file. 
	*
	*	Return:				bool
	*
	*	Mod. Date:          7/16/2014
	*	Mod. Initials:      MB
	**********************************/
	bool LoadSound(const char * szShortName, int * nIndex = nullptr);

	/**********************************
	*	LoadMusic:			Loads a stream into memory.
	*
	*	In:					const char * szShortname - The short name for the file.
	*
	*	Return:             bool
	*
	*	Mod. Date:          7/16/2014
	*	Mod. Initials:		MB
	**********************************/
	bool LoadMusic(const char * szShortName, int * nIndex = nullptr);

	/**********************************
	*  LoadXML:				Loads an XML file with the name of resources.
	*
	*  In:					const char * szFile - The XML file to parse.
	*
	*  Return:				bool
	*
	*  Mod. Date:			7/21/2014
	*  Mod. Initials:		MB
	**********************************/
	bool LoadXML(const char * szFile);

	/**********************************
	*  GetSound:	       Gets a channel for the sound. 
	*
	*  In:                 Add parameters here
	*
	*  Return:             int
	*
	*  Mod. Date:          7/16/2014
	*  Mod. Initials:      MB
	**********************************/
	int GetSound(const char * szShortName);

	/**********************************
	*	GetMusic:			Add details here
	*
	*	In:					
	*
	*	Return:				int
	*
	*	Mod. Date:			7/16/2014
	*	Mod. Initials:		MB
	**********************************/
	int GetMusic(const char * szShortName);

	/**********************************
	*	CreateChannel:			Creates a channel for the sound. Channels control the sound.
	*
	*	In:						int nIndex - the audio to store in the channel.
	*							bool bImmediatePlay - determines if the sound starts playing.
	*
	*	Out:					FMOD::Channel ** cChannel - The channel created by the system.
	*
	*	Return:					void
	*
	*	Mod. Date:				7/16/2014
	*	Mod. Initials:			MB
	**********************************/
	void CreateChannel(int nIndex, bool bImmediatePlay, FMOD::Channel ** cChannel);

	/**********************************
	*	SetSFXVolume:		Sets the volume for all sound effects.
	*
	*	In:					float fVolume - The volume to set the effects to.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void SetSFXVolume(float fVolume);

	/**********************************
	*	SetMusicVolume:		Add details here
	*
	*	In:					float fVolume - The volume to set the music to.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void SetMusicVolume(float fVolume);

	/**********************************
	*	GetSFXVolume:		Gets the volume for the sound effects.
	*
	*	Return:				float
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	float GetSFXVolume() const;

	/**********************************
	*	GetMusicVolume:		Gets the volume for the sound effects.
	*
	*	Return:				float
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	float GetMusicVolume() const;
};
#endif
#endif