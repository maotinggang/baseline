#include "StdAfx.h"
#include "Business.h"
#include <process.h>

CBusiness::CBusiness(void) : m_pSolution(NULL), m_hStopProcess(NULL), m_bEncrypt(false)
{
}

CBusiness::~CBusiness(void)
{
}

bool CBusiness::LoadSolution()
{
	m_bEncrypt = true;

	if (m_pSolution == NULL)
	{
		m_pSolution = new ISolution;
		if (m_pSolution->Initialize() == false)
		{
			m_bEncrypt = false;
			return false;
		}
		if (m_bEncrypt)
		{
			m_pSolution->SetIsVerified(true);
		}
	}

	return ((m_pSolution != NULL) ? true : false);
}

void CBusiness::FreeSolution()
{
	if (m_pSolution)
	{
		delete m_pSolution;
		m_pSolution = NULL;
	}
}

ISolution* CBusiness::GetSolution()
{
	return m_pSolution;
}