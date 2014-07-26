/***********************************************
* Filename:  		AudioSource.cpp
* Date:      		7/18/2014
* Mod. Date: 		7/18/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		An object used to control fmod channels.
************************************************/
#include "AudioSource.h"
#include "AudioManager.h"
#ifndef SERVER_BUILD
CAudioSource::CAudioSource(unsigned int nNumOfChannels): m_nNumOfChannels(nNumOfChannels), m_nWorkingSlot(0)
{
	m_vChannels.reserve(nNumOfChannels);
	m_vIndexes.reserve(nNumOfChannels);
	for(unsigned int nCounter = 0; nCounter < nNumOfChannels; nCounter++)
	{
		m_vChannels.push_back(nullptr);
		m_vIndexes.push_back(0);
	}

}

CAudioSource::~CAudioSource()
{
	for(unsigned int nIndex = 0; nIndex < m_nNumOfChannels; nIndex++)
	{
		if(m_vChannels[nIndex] == nullptr)
		{
			continue;
		}

		m_vChannels[nIndex]->stop();
	}
}

bool CAudioSource::LoadSlot(const char * szShortName, bool bIsMusic, unsigned int nSlot)
{
	if(nSlot > m_nNumOfChannels)
	{
		printf("ERROR: Out of bounds for the slot.\n");
		return false;
	}
	int nIndex = 0;
	CAudioManager * manager = CAudioManager::GetInstance();
	if(bIsMusic)
	{
		nIndex = manager->GetMusic(szShortName);
	}
	else
	{
		nIndex = manager->GetSound(szShortName);
	}

	if(nIndex >= 0)
	{
		if(m_vChannels[nSlot] != nullptr)
		{
			m_vChannels[nSlot]->stop();
			m_vChannels[nSlot] = nullptr;
		}
		m_vIndexes[nSlot] = nIndex;
		manager->CreateChannel(nIndex, false, &m_vChannels[nSlot]);

		m_nWorkingSlot = nSlot;
		return true;
	}
	return false;
}

void CAudioSource::RestoreChannel(bool bAllSlots)
{
	if(bAllSlots)
	{
		for(unsigned int nIndex = 0; nIndex < m_nNumOfChannels; nIndex++)
		{
			if(m_vChannels[nIndex] == nullptr)
			{
				continue;
			}

			RestoreChannel(nIndex);

		}
	}
	else
	{
		RestoreChannel(m_nWorkingSlot);
	}
}

void CAudioSource::RestoreChannel(unsigned int nSlot)
{
	FMOD::Sound * pcTemp;
	FMOD_RESULT eResult = m_vChannels[nSlot]->getCurrentSound(&pcTemp);
	if(eResult == FMOD_ERR_INVALID_HANDLE)
	{
		CAudioManager::GetInstance()->CreateChannel(m_vIndexes[nSlot], false, &m_vChannels[nSlot]);
	}
}

void CAudioSource::Play(bool bAllSlots)
{
	if(bAllSlots)
	{
		for(unsigned int nIndex = 0; nIndex < m_nNumOfChannels; nIndex++)
		{
			if(m_vChannels[nIndex] == nullptr)
			{
				continue;
			}

			m_vChannels[nIndex]->setPaused(false);
		}
	}
	else
	{
		m_vChannels[m_nWorkingSlot]->setPaused(false);
	}
}

void CAudioSource::Play(unsigned int nSlot)
{
	if(nSlot > m_nNumOfChannels)
	{
		printf("ERROR: Out of bounds for the slot.\n");
		return;
	}
	m_nWorkingSlot = nSlot;
	m_vChannels[m_nWorkingSlot]->setPaused(false);
}

void CAudioSource::Pause(bool bAllSlots)
{
	if(bAllSlots)
	{
		for(unsigned int nIndex = 0; nIndex < m_nNumOfChannels; nIndex++)
		{
			if(m_vChannels[nIndex] == nullptr)
			{
				continue;
			}

			m_vChannels[nIndex]->setPaused(true);
		}
	}
	else
	{
		m_vChannels[m_nWorkingSlot]->setPaused(true);
	}
}

void CAudioSource::Pause(unsigned int nSlot)
{
	if(nSlot > m_nNumOfChannels)
	{
		printf("ERROR: Out of bounds for the slot.\n");
		return;
	}
	m_nWorkingSlot = nSlot;
	m_vChannels[m_nWorkingSlot]->setPaused(true);
}

void CAudioSource::SetVolume(float fVolume)
{
	m_vChannels[m_nWorkingSlot]->setVolume(fVolume);
}

void CAudioSource::SetLooping(int nLoopCount)
{
	m_vChannels[m_nWorkingSlot]->setLoopCount(nLoopCount);
}

void CAudioSource::SetPosition(float fPosition)
{
	unsigned int nTime = (unsigned int)(1000.0f * fPosition);
	m_vChannels[m_nWorkingSlot]->setPosition(nTime, FMOD_TIMEUNIT_MS);
}

float CAudioSource::GetPosition(int nSlot)
{
	unsigned int nTime = 0;
	if(nSlot == WORKING_SLOT)
	{
		m_vChannels[m_nWorkingSlot]->getPosition(&nTime, FMOD_TIMEUNIT_MS);
	}
	else
	{
		m_vChannels[nSlot]->getPosition(&nTime, FMOD_TIMEUNIT_MS);
	}
	return (float)(nTime) * 0.001f;
}

bool CAudioSource::GetSongStatus(int nSlot, bool * pbIsEnded)
{
	bool bIsPaused = true;
	if(nSlot == WORKING_SLOT)
	{
		FMOD::Sound * soundReference = nullptr;
		FMOD_RESULT eResult = m_vChannels[m_nWorkingSlot]->getCurrentSound(&soundReference);
		if(soundReference != nullptr)
		{
			m_vChannels[m_nWorkingSlot]->getPaused(&bIsPaused);
			if(pbIsEnded)
			{
				*pbIsEnded = false;
			}
		}
		else if(pbIsEnded != nullptr)
		{
			*pbIsEnded = true;
		}
		
	}
	else
	{
		FMOD::Sound * soundReference = nullptr;
		FMOD_RESULT eResult = m_vChannels[nSlot]->getCurrentSound(&soundReference);
		if(soundReference != nullptr)
		{
			m_vChannels[nSlot]->getPaused(&bIsPaused);
			if(pbIsEnded)
			{
				*pbIsEnded = false;
			}
		}
		else if(pbIsEnded != nullptr)
		{
			*pbIsEnded = true;
		}
		
	}
	return true;
}

unsigned int CAudioSource::GetWorkingSlot() const
{
	return m_nWorkingSlot;
}

unsigned int CAudioSource::GetNumberOfSlots() const
{
	return m_nNumOfChannels;
}

void CAudioSource::SetWorkingSlot(unsigned int nSlot)
{
	if(nSlot > m_nNumOfChannels)
	{
		printf("ERROR: Out of bounds for the slot.\n");
		return;
	}

	m_nWorkingSlot = nSlot;
}
#endif