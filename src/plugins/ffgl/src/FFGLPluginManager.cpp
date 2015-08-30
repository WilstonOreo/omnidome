//
// Copyright (c) 2004 - InfoMus Lab - DIST - University of Genova
//
// InfoMus Lab (Laboratorio di Informatica Musicale)
// DIST - University of Genova 
//
// http://www.infomus.dist.unige.it
// news://infomus.dist.unige.it
// mailto:staff@infomus.dist.unige.it
//
// Developer: Gualtiero Volpe
// mailto:volpe@infomus.dist.unige.it
//
// Last modified: Oct 25 2006 by Trey Harrison
// email:trey@harrisondigitalmedia.com

#include "FFGLPluginManager.h"
#include "FFGLPluginSDK.h"

#include <stdlib.h> 
#include <memory.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFFGLPluginManager constructor and destructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFFGLPluginManager::CFFGLPluginManager()
{
	m_iMinInputs = 0;
	m_iMaxInputs = 0;
  m_timeSupported = 0;

	m_NParams = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
}

CFFGLPluginManager::~CFFGLPluginManager()
{
	if (m_pFirst != NULL)
  {
		ParamInfo* pCurr = m_pFirst;
		ParamInfo* pNext = m_pFirst;

		while (pCurr != NULL)
    {
			pNext = pCurr->pNext;

			if ( (pCurr->dwType == FF_TYPE_TEXT) &&
				 (pCurr->StrDefaultValue != NULL) )
			{
				free(pCurr->StrDefaultValue);
			}

			delete pCurr;
			pCurr = pNext;
		}
	}

	m_pFirst = NULL;
	m_pLast = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFFGLPluginManager methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFFGLPluginManager::SetMinInputs(int iMinInputs)
{
	m_iMinInputs = iMinInputs;
}

void CFFGLPluginManager::SetMaxInputs(int iMaxInputs)
{
	m_iMaxInputs = iMaxInputs;
}

void CFFGLPluginManager::SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, float fDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = dwIndex;
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = dwType;
	if (fDefaultValue > 1.0) fDefaultValue = 1.0;
	if (fDefaultValue < 0.0) fDefaultValue = 0.0;
	pInfo->DefaultValue = fDefaultValue;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, bool bDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = dwIndex;
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}
	
	pInfo->dwType = dwType;
	pInfo->DefaultValue = bDefaultValue ? 1.0f : 0.0f;
	pInfo->StrDefaultValue = NULL;
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetParamInfo(DWORD dwIndex, const char* pchName, DWORD dwType, const char* pchDefaultValue)
{
	ParamInfo* pInfo = new ParamInfo;
	pInfo->ID = dwIndex;
	
	bool bEndFound = false;
	for (int i = 0; i < 16; ++i) {
		if (pchName[i] == 0) bEndFound = true;
		pInfo->Name[i] = (bEndFound) ?  0 : pchName[i];
	}

	pInfo->dwType = dwType;
	pInfo->DefaultValue = 0;
	pInfo->StrDefaultValue = strdup(pchDefaultValue);
	pInfo->pNext = NULL;
	if (m_pFirst == NULL) m_pFirst = pInfo; 
	if (m_pLast != NULL) m_pLast->pNext = pInfo;
	m_pLast = pInfo;
	m_NParams++;
}

void CFFGLPluginManager::SetTimeSupported(bool supported)
{
  m_timeSupported = supported;
}

char* CFFGLPluginManager::GetParamName(DWORD dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->Name;
	return NULL;
}
	
DWORD CFFGLPluginManager::GetParamType(DWORD dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) return pCurr->dwType;
	return FF_FAIL;
}

void* CFFGLPluginManager::GetParamDefault(DWORD dwIndex) const
{
	ParamInfo* pCurr = m_pFirst;
	bool bFound = false;
	while (pCurr != NULL) {
		if (pCurr->ID == dwIndex) {
			bFound = true;
			break;
		}
		pCurr = pCurr->pNext;
	}
	if (bFound) {
		if (GetParamType(dwIndex) == FF_TYPE_TEXT)
			return (void*)pCurr->StrDefaultValue;
		else
			return (void*) &pCurr->DefaultValue;
	}
	return NULL;
}

bool CFFGLPluginManager::GetTimeSupported() const
{
  return m_timeSupported;
}
