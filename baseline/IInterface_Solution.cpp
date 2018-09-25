#include "StdAfx.h"
#include "IInterface_Solution.h"
#include "Business.h"

extern CBusiness* g_pBusiness = NULL;

bool CreateSolutionModule()
{
	if (g_pBusiness == NULL)
	{
		g_pBusiness = new CBusiness();
	}

	ASSERT(g_pBusiness != NULL);

	if (false == g_pBusiness->LoadSolution())
	{
		return false;
	}

	return true;
}

void ReleaseSolutionModule()
{
	if (g_pBusiness != NULL)
	{
		g_pBusiness->FreeSolution();

		delete g_pBusiness;
		g_pBusiness = NULL;
	}
}

void SetBaseLBH(double lonDeg, double latDeg, double Hgh)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution != NULL)
		{
			pSolution->SetBaseLBH(lonDeg, latDeg, Hgh);
		}
	}
}

int BaseLine(char* sBaseFile, char* sRemoteFile, double& x, double& y, double& z, double& px, double& py, double& pz, int coordSysType)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution == NULL)
			return -1;

		if (0 == pSolution->GPSDecord(sBaseFile, sRemoteFile))
		{
			pSolution->GetTransformValue((IInterface_Solution::COORDSYSTEMTYPE)coordSysType, x, y, z);
			pSolution->GetTransformPrecision((IInterface_Solution::COORDSYSTEMTYPE)coordSysType, px, py, pz);
			return 0;
		}
	}

	return -1;
}

int AllResult(char * sBaseFile, char * sRemoteFile, BLINF & allResult, int coordSysType)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution == NULL)
			return -1;

		if (0 == pSolution->GPSDecord(sBaseFile, sRemoteFile))
		{
			pSolution->GetAllResult((IInterface_Solution::COORDSYSTEMTYPE)coordSysType, allResult);
			return 0;
		}
	}

	return -1;
}

void LBHtoXYZ(double lonDeg, double latDeg, double Hgh, double& X, double& Y, double& Z)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution != NULL)
		{
			pSolution->LBHtoXYZ(lonDeg, latDeg, Hgh, X, Y, Z);
		}
	}
}

void XYZtoLBH(double X, double Y, double Z, double& lonDeg, double& latDeg, double& Hgh)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution != NULL)
		{
			pSolution->XYZtoLBH(X, Y, Z, lonDeg, latDeg, Hgh);
		}
	}
}

void WGS84toLLSNEU(double BaseLonDeg, double BaseLatDeg, double BaseHgh, double RemoteLonDeg, double RemoteLatDeg, double RemoteHgh, double& RX, double& RY, double& RZ)
{
	if (g_pBusiness != NULL)
	{
		IInterface_Solution* pSolution = g_pBusiness->GetSolution();
		if (pSolution != NULL)
		{
			pSolution->WGS84toLLSNEU(BaseLonDeg, BaseLatDeg, BaseHgh, RemoteLonDeg, RemoteLatDeg, RemoteHgh, RX, RY, RZ);
		}
	}
}