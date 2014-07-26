/***********************************************
* Filename:  		AudioSource.h
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		An object used to control fmod channels.
************************************************/

#ifndef AUDIOSOURCE_HEADER
#define AUDIOSOURCE_HEADER
#ifndef SERVER_BUILD

#define ALL_SLOTS		true
#define WORKING_SLOT	-1



#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>
#include <vector>
class CAudioSource
{
	
	unsigned int m_nNumOfChannels;
	unsigned int m_nWorkingSlot;
	std::vector<FMOD::Channel *> m_vChannels;
	std::vector<int> m_vIndexes;
public:
	CAudioSource(unsigned int nNumOfChannels = 1);
	~CAudioSource();

	/**********************************
	*	LoadSlot:			Add details here
	*
	*	In:					const char * szShortName - The short name for the file
	*						bool bIsMusic - Determines if what your calling for is a song or a sound effect
	*						unsigned int nSlot - The slot to load the sound into
	*
	*	Return:				bool
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	bool LoadSlot(const char * szShortName, bool bIsMusic, unsigned int nSlot);

	/**********************************
	*  RestoreChannel:			Checks the channels for a sound. If it doesn't have one, it will
	*							reaquire a channel with the sound.
	*
	*	In:						bool bAllChannels - Determines if all the channels should attempt to be restored.
	*
	*  Return:					void
	*
	*  Mod. Date:				7/21/2014
	*  Mod. Initials:			MB
	**********************************/
	void RestoreChannel(bool bAllChannels = false);

	/**********************************
	*	RestoreChannel:			Checks the channels for a sound. If it doesn't have one, it will
	*							reaquire a channel with the sound.
	*
	*	In:						unsigned int nSlot - The 
	*
	*	Return:					void
	*
	*	Mod. Date:				7/21/2014
	*	Mod. Initials:			MB
	**********************************/
	void RestoreChannel(unsigned int nSlot);

	/**********************************
	*	Play:				Causes the working slot or all the slots to play.
	*
	*	In:					bool bAllSlots - Determines if all the slots play.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void Play(bool bAllSlots = false);

	/**********************************
	*	Play:				Causes a slot to play.
	*
	*	In:					unsigned int nSlot - The slot to play.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void Play(unsigned int nSlot);

	/**********************************
	*	Pause:				Causes the working slot or all the slots to pause.
	*
	*	In:					bool bAllSlots - Determines if all the slots are paused.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void Pause(bool bAllSlots = false);

	/**********************************
	*	Pause:				Causes a slot to pause. Will change the working slot to the
	*						passed slot.
	*
	*	In:					unsigned int nSlot - The slot to pause.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void Pause(unsigned int nSlot);

	/**********************************
	*	SetVolume:			Sets the volume for the current working slot.
	*
	*	In:					float fVolume - The volume to set the slot to.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void SetVolume(float fVolume);

	/**********************************
	*	SetLooping:			Sets how many times the song loops. Send -1 for never stops.
	*
	*	In:					int nLoopCount - The number of times to loop.
	*
	*	Return:				void
	*
	*	Mod. Date:			7/18/2014
	*	Mod. Initials:		MB
	**********************************/
	void SetLooping(int nLoopCount);

	/**********************************
	*	SetPosition:			Sets the position of the channel.
	*
	*	In:						float fPosition - The position to set the track to.
	*
	*	Return:					void
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	void SetPosition(float fPosition);

	/**********************************
	*	GetPosition:			Gets the position of the channel.
	*
	*	In:						int nSlot - The slot to check.
	*
	*	Return:					float
	*
	*	Mod. Date:				7/18/2014
	*	Mod. Initials:			MB
	**********************************/
	float GetPosition(int nSlot = WORKING_SLOT);

	/**********************************
	*	GetSongStatus:		Checks if the song is playing, paused, or ended.
	*
	*	In:					int nSlot - The slot to check.
	*
	*	Out:                bool * pbIsEnded - Returns if the song is at the end.
	*
	*	Return:				bool
	*
	*	Mod. Date:			7/21/2014
	*	Mod. Initials:		MB
	**********************************/
	bool GetSongStatus(int nSlot = WORKING_SLOT, bool * pbIsEnded = nullptr);

	/////////////
	//Accessors//
	/////////////
	unsigned int GetWorkingSlot() const;
	unsigned int GetNumberOfSlots() const;

	/////////////
	//Mutators //
	/////////////
	void SetWorkingSlot(unsigned int nSlot);
};
#endif
#endif