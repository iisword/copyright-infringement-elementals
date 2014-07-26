/***********************************************
* Filename:  		AudioManager.cpp
* Date:      		7/15/2014
* Mod. Date: 		7/15/2014
* Mod. Initials:	MB
* Author:    		Michael Boodle
* Purpose:   		The audio manager for the game. It loads and unloads sounds into memory
*					and handles playing music and sound effects. 
************************************************/

#include "AudioManager.h"
#include "..\Utilities\tinyxml.h"
#include "..\Utilities\tinystr.h"
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <Windows.h>
#ifndef SERVER_BUILD


CAudioManager::CAudioManager(): m_cFmodSystem(nullptr), m_cMusicGroup(nullptr), m_cSoundEffectGroup(nullptr)
{

}

CAudioManager::~CAudioManager()
{

}

CAudioManager * CAudioManager::GetInstance()
{
	static CAudioManager cSharedManager;
	return &cSharedManager;
}

bool CAudioManager::CheckFMODError(FMOD_RESULT eResult)
{
	if(eResult != FMOD_OK)
	{
		printf("ERROR:FMOD: An error occured with result: %d", eResult);
		return false;
	}
	return true;
}

bool CAudioManager::Initialize()
{
	FMOD_RESULT eResult = FMOD::System_Create(&m_cFmodSystem);
	if(CheckFMODError(eResult) == false)
	{
		return false;
	}

	eResult = m_cFmodSystem->getNumDrivers(&m_nNumOfDrivers);
	if(CheckFMODError(eResult) == false)
	{
		return false;
	}

	if(m_nNumOfDrivers == 0)
	{
		eResult = m_cFmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		return false;
	}
	else
	{
		eResult = m_cFmodSystem->getDriverCaps(0, &m_nCaps, 0, &m_eSpeakerMode);
		if(CheckFMODError(eResult) == false)
		{
			return false;
		}

		eResult = m_cFmodSystem->setSpeakerMode(m_eSpeakerMode);
		if(CheckFMODError(eResult) == false)
		{
			return false;
		}

		eResult = m_cFmodSystem->setDSPBufferSize(FMOD_SOFTWARE_BUFFER_SIZE, FMOD_SAMPLES_OF_BUFFER);
		CheckFMODError(eResult);
	}

	eResult = m_cFmodSystem->init(FMOD_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
	if(CheckFMODError(eResult) == false)
	{
		if(eResult != FMOD_ERR_OUTPUT_CREATEBUFFER)
		{
			return false;
		}

		m_eSpeakerMode = FMOD_SPEAKERMODE_STEREO;
		eResult = m_cFmodSystem->setSpeakerMode(m_eSpeakerMode);
		
		eResult = m_cFmodSystem->init(FMOD_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
		if(CheckFMODError(eResult) == false)
		{
			return false;
		}
	}

	eResult = m_cFmodSystem->createChannelGroup(nullptr, &m_cMusicGroup);
	CheckFMODError(eResult);
	eResult = m_cFmodSystem->createChannelGroup(nullptr, &m_cSoundEffectGroup);
	CheckFMODError(eResult);


	return true;
}

void CAudioManager::Shutdown()
{
	for(unsigned int nIndex = 0; nIndex < m_vFiles.size(); nIndex++)
	{
		m_vFiles[nIndex]->m_cSound->release();
		delete m_vFiles[nIndex];
	}

	if(m_cMusicGroup)
	{
		m_cMusicGroup->release();
		m_cMusicGroup = nullptr;
	}
	
	if(m_cSoundEffectGroup)
	{
		m_cSoundEffectGroup->release();
		m_cSoundEffectGroup = nullptr;
	}

	if(m_cFmodSystem)
	{
		m_cFmodSystem->release();
		m_cFmodSystem = nullptr;
	}
	m_vFiles.clear();
}

void CAudioManager::Update()
{
	m_cFmodSystem->update();
}

bool CAudioManager::LoadSound(const char * szShortName, int * nIndex)
{
	int nFound = GetSound(szShortName);
	if(nFound == AMERROR_NOT_FOUND)
	{
		TAudioFile * tNewFile = new TAudioFile();
		tNewFile->m_chType = AUDIOFILETYPE_SOUND;
		tNewFile->m_szFilePath = SOUND_ASSEST_FOLDER;
		tNewFile->m_szFilePath += szShortName;
		tNewFile->m_szShortName = szShortName;
		FMOD_RESULT eResult = m_cFmodSystem->createSound(tNewFile->m_szFilePath.c_str(), FMOD_DEFAULT, nullptr, &tNewFile->m_cSound);

		if(CheckFMODError(eResult))
		{
			std::cout << "Loaded sound file: " << tNewFile->m_szShortName << "\n";
			nFound = (int)m_vFiles.size();
			m_vFiles.push_back(tNewFile);
		}
		else
		{
			delete tNewFile;
			return false;
		}

	}
	else if(nFound == AMERROR_WRONG_TYPE)
	{
		return false;
	}

	if(nIndex)
	{
		*nIndex = nFound;
	}

	return true;
}

bool CAudioManager::LoadMusic(const char * szShortName, int * nIndex)
{
	int nFound = GetSound(szShortName);
	if(nFound == AMERROR_NOT_FOUND)
	{
		TAudioFile * tNewFile = new TAudioFile();
		tNewFile->m_chType = AUDIOFILETYPE_STREAM;
		tNewFile->m_szFilePath = MUSIC_ASSEST_FOLDER;
		tNewFile->m_szFilePath += szShortName;
		tNewFile->m_szShortName = szShortName;
		FMOD_RESULT eResult = m_cFmodSystem->createStream(tNewFile->m_szFilePath.c_str(), FMOD_DEFAULT, nullptr, &tNewFile->m_cSound);

		if(CheckFMODError(eResult))
		{

			std::cout << "Loaded sound file: " << tNewFile->m_szShortName << "\n";
			nFound = (int)m_vFiles.size();
			m_vFiles.push_back(tNewFile);
		}
		else
		{
			delete tNewFile;
			return false;
		}

	}
	else if(nFound == AMERROR_WRONG_TYPE)
	{
		return false;
	}

	if(nIndex)
	{
		*nIndex = nFound;
	}

	return true;
}

bool CAudioManager::LoadXML(const char * szFile)
{
	TiXmlDocument cDocument(szFile);
	if(cDocument.LoadFile() == false)
	{
		return false;
	}

	TiXmlElement * pRoot = cDocument.RootElement();

	TiXmlElement * pSFX = pRoot->FirstChildElement("SFX");
	TiXmlElement * pMusic = pRoot->FirstChildElement("music");

	if(pSFX == nullptr && pMusic == nullptr)
	{
		printf("ERROR: XML File is there, but there is no data.");
		return false;
	}

	TiXmlElement * pFile;

	if(pSFX)
	{
		pFile = pSFX->FirstChildElement("file");
		while(pFile)
		{
			TAudioFile * tNewFile = new TAudioFile();
			tNewFile->m_chType = AUDIOFILETYPE_SOUND;
			tNewFile->m_szFilePath = SOUND_ASSEST_FOLDER;
			tNewFile->m_szFilePath += pFile->Attribute("fileName");
			if(pFile->Attribute("shortName") != nullptr)
			{
				tNewFile->m_szShortName = pFile->Attribute("shortName");
			}
			else
			{
				tNewFile->m_szShortName = pFile->Attribute("fileName");
			}
			FMOD_RESULT eResult = m_cFmodSystem->createSound(tNewFile->m_szFilePath.c_str(), FMOD_DEFAULT, nullptr, &tNewFile->m_cSound);

			if(CheckFMODError(eResult))
			{
				std::cout << "Loaded sound file: " << tNewFile->m_szShortName << "\n";
				m_vFiles.push_back(tNewFile);
			}
			else
			{
				printf("Failed to load: %s.\n", tNewFile->m_szFilePath.c_str());
				return false;
			}
			pFile = pFile->NextSiblingElement("file");
		}
	}

	if(pMusic)
	{
		pFile = pMusic->FirstChildElement("file");
		while(pFile)
		{
			TAudioFile * tNewFile = new TAudioFile;
			tNewFile->m_chType = AUDIOFILETYPE_STREAM;
			tNewFile->m_szFilePath = MUSIC_ASSEST_FOLDER;
			tNewFile->m_szFilePath += pFile->Attribute("fileName");
			if(pFile->Attribute("shortName") != nullptr)
			{
				tNewFile->m_szShortName = pFile->Attribute("shortName");
			}
			else
			{
				tNewFile->m_szShortName = pFile->Attribute("fileName");
			}

			FMOD_RESULT eResult = m_cFmodSystem->createStream(tNewFile->m_szFilePath.c_str(), FMOD_DEFAULT, nullptr, &tNewFile->m_cSound);

			if(CheckFMODError(eResult))
			{

				std::cout << "Loaded sound file: " << tNewFile->m_szShortName << "\n";
				m_vFiles.push_back(tNewFile);
			}
			else
			{
				printf("Failed to load: %s.\n", tNewFile->m_szFilePath.c_str());
				delete tNewFile;
			}
			pFile = pFile->NextSiblingElement("file");
		}
	}

	return true;
}

int CAudioManager::GetSound(const char * szShortName)
{
	for(int nIndex = 0; nIndex < (int)m_vFiles.size(); nIndex++)
	{
		if(m_vFiles[nIndex]->m_szShortName == szShortName)
		{
			if(m_vFiles[nIndex]->m_chType == AUDIOFILETYPE_SOUND)
			{
				return nIndex;
			}
			else
			{
				return AMERROR_WRONG_TYPE;
			}
		}
	}
	return AMERROR_NOT_FOUND;
}

int CAudioManager::GetMusic(const char * szShortName)
{
	for(int nIndex = 0; nIndex < (int)m_vFiles.size(); nIndex++)
	{
		if(m_vFiles[nIndex]->m_szShortName == szShortName)
		{
			if(m_vFiles[nIndex]->m_chType == AUDIOFILETYPE_STREAM)
			{
				return nIndex;
			}
			else
			{
				return AMERROR_WRONG_TYPE;
			}
		}
	}
	return AMERROR_NOT_FOUND;
}

void CAudioManager::CreateChannel(int nIndex, bool bImmediatePlay, FMOD::Channel ** cChannel)
{
	if(cChannel == nullptr)
	{
		std::cout << "ERROR:AUDIO: You didn't pass in a channel.\n";
		return;
	}

	if(nIndex < 0 || nIndex > (int)m_vFiles.size())
	{
		std::cout << "ERROR:AUDIO: Index out of bounds.";
		return;
	}
	
	FMOD_RESULT eError = m_cFmodSystem->playSound(FMOD_CHANNEL_FREE, m_vFiles[nIndex]->m_cSound, !bImmediatePlay, cChannel);
	if(CheckFMODError(eError) == false)
	{
		return;
	}

	if(m_vFiles[nIndex]->m_chType == AUDIOFILETYPE_SOUND)
	{
		(*cChannel)->setChannelGroup(m_cSoundEffectGroup);
	}
	else
	{
		(*cChannel)->setChannelGroup(m_cMusicGroup);
	}
}

void CAudioManager::SetSFXVolume(float fVolume)
{
	if(m_cSoundEffectGroup)
	{
		m_cSoundEffectGroup->setVolume(fVolume);
	}
}

float CAudioManager::GetSFXVolume() const
{
	float fReturnFloat = 0.0f;
	if(m_cSoundEffectGroup)
	{
		m_cSoundEffectGroup->getVolume(&fReturnFloat);
	}
	return fReturnFloat;
}

void CAudioManager::SetMusicVolume(float fVolume)
{
	if(m_cMusicGroup)
	{
		m_cMusicGroup->setVolume(fVolume);
	}
}

float CAudioManager::GetMusicVolume() const
{
	float fReturnFloat = 0.0f;
	if(m_cMusicGroup)
	{
		m_cMusicGroup->getVolume(&fReturnFloat);
	}
	return fReturnFloat;
}
#endif