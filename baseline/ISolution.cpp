#include "StdAfx.h"
#include "ISolution.h"
#include "compute.h"
#include <stdio.h>

#define	PI 3.14159265358979323846

#if _MSC_VER >= 1300    // for VC 7.0
// from ATL 7.0 sources
#ifndef _delayimp_h
extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif
#endif

CString GetCPUID(void);
CString UnEncrypt(const char* license);
ISolution::ISolution(void) : m_bVerified(false)
{
	::InitializeCriticalSection(&m_csLock); 
}

ISolution::~ISolution(void)
{
	::DeleteCriticalSection(&m_csLock);
}

bool ISolution::Initialize()
{
	CString strFile=_T(".\\Config");
	CString strField("SOLUTION");
	CString strKey("Freq");
	char szData[_MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	CString strValue;
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nFreq = atoi(strValue);

	strKey = _T("GPS");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nGPS = atoi(strValue);

	strKey = _T("Glonass");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nGlonass = atoi(strValue);

	strKey = _T("Beidou");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nBeidou = atoi(strValue);

	strKey = _T("EPSample");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nEpSample = atoi(strValue);

	strKey = _T("CutOff");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_nCutOff = atoi(strValue);

	strKey = _T("Ratio");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_dRatio = atof(strValue);

	strKey = _T("RMS");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_dRms = atof(strValue);

	strField = _T("BASE");
	strKey = _T("L");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_dBaseLon = atof(strValue);

	strKey = _T("B");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_dBaseLat = atof(strValue);

	strKey = _T("H");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();
	m_dBaseHgh = atof(strValue);

	strField = _T("KEY");
	strKey = _T("CODE");
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(strField, strKey, _T(""), szData, sizeof(szData), strFile);
	strValue.Format("%s", szData);
	strValue.Trim();

	// 注册码验证
	CString strKeyCode = UnEncrypt(strValue.GetString());
	strKeyCode.Trim();

	//获取本机的CPUID
	CString strCPUID(GetCPUID());
	strCPUID.Trim();

	if (strCPUID.CompareNoCase(strKeyCode.Mid(16)) != 0){//CPUID验证
		return false;
	}else{// 有效期验证
		CString strLimitTime=strKeyCode.Left(16);
		CString strLimitTimeDown=strLimitTime.Left(8);
		CString strLimitTimeUp=strLimitTime.Right(8);
		int year,month,day;
		sscanf(strLimitTimeDown,"%4d%2d%2d",&year,&month,&day);
		if (2000>year||2050<year||1>month||12<month||1>day||31<day){
			return false;
		}
		CTime LimitTimeDown(year,month,day,0,0,0);

		sscanf(strLimitTimeUp,"%4d%2d%2d",&year,&month,&day);
		if (2000>year||2050<year||1>month||12<month||1>day||31<day){
			return false;
		}
		CTime LimitTimeUp(year,month,day,0,0,0);
		CTime CurTime = CTime::GetCurrentTime();
		if ((CurTime<LimitTimeDown)||(CurTime>LimitTimeUp)){
			return false;
		} 
		else{
			return true;
		}
	}
}



void ISolution::SetIsVerified(bool bVerify)
{
	try
	{
		::EnterCriticalSection(&m_csLock);
		m_bVerified = bVerify;
		::LeaveCriticalSection(&m_csLock);
	}
	catch (...)
	{
		::LeaveCriticalSection(&m_csLock);
	}
}

void ISolution::SetBaseLBH(double lonDeg, double latDeg, double Hgh)
{
	m_dBaseLon = lonDeg;
	m_dBaseLat = latDeg;
	m_dBaseHgh = Hgh;
}

int ISolution::GPSDecord(char* sBaseFile, char* sRemoteFile)
{
	if (false == m_bVerified)
		return -101;

	CString strBaseFile(sBaseFile);
	CString strRemoteFile(sRemoteFile);

	int nPos = strBaseFile.ReverseFind('.');
	CString strType = strBaseFile.Mid(nPos + 1);
	strType.MakeUpper();
	if (strType.CompareNoCase("OBS") != 0)
	{
		CString strNewFile = strBaseFile.Left(nPos+1) + _T("obs");
		rename(strBaseFile, strNewFile);
		strBaseFile = strNewFile;
	}

	nPos = strRemoteFile.ReverseFind('.');
	strType = strRemoteFile.Mid(nPos + 1);
	strType.MakeUpper();
	if (strType.CompareNoCase("OBS") != 0)
	{
		CString strNewFile = strRemoteFile.Left(nPos+1) + _T("obs");
		rename(strRemoteFile, strNewFile);
		strRemoteFile = strNewFile;
	}

	BLINF p;

	strcpy(p.BaseName, strBaseFile);
	strcpy(p.RoveName, strRemoteFile);

	p.epSample=m_nEpSample;
	p.epBegin=0;
	p.epEnd=14400;
	p.CutOff=m_nCutOff;
	p.SolG=m_nGPS;
	p.SolN=m_nGlonass;
	p.SolB=m_nBeidou;
	p.Freq=m_nFreq;

	try
	{
		int nRet=ProBaseline(&p);
		if (nRet < 0)
		{
			return -102;
		}
	}
	catch (...)
	{
		return -103;
	}

	if (p.dis == 0 || p.dx == 0 || p.dy == 0 || p.dz == 0 || 
		fabs(p.px) > pow((float)10,(int)6) || fabs(p.py) > pow((float)10,(int)6) || fabs(p.pz) > pow((float)10,(int)6))
	{
		return -104;
	}

	if (p.ratio < m_dRatio || p.rms > 0.001*m_dRms)
	{
		return -105;
	}

	m_dX = p.dx;
	m_dY = p.dy;
	m_dZ = p.dz;
	m_dPX = p.px;
	m_dPY = p.py;
	m_dPZ = p.pz;
	
	m_sAll = p;
	return 0;
}

void ISolution::GetTransformValue(COORDSYSTEMTYPE emCoordSys, double& x, double& y, double& z)
{
	if (emCoordSys == WGS84_XYZ)
	{
		x = m_dX;
		y = m_dY;
		z = m_dZ;
	}

	if (emCoordSys == LLS_NEU)
	{
		double dBaseLonRadian = m_dBaseLon*PI/180.0f;
		double dBaseLatRadian = m_dBaseLat*PI/180.0f;

		x = -sin(dBaseLatRadian)*cos(dBaseLonRadian)*m_dX-sin(dBaseLatRadian)*sin(dBaseLonRadian)*m_dY+cos(dBaseLatRadian)*m_dZ;
		y = -sin(dBaseLonRadian)*m_dX+cos(dBaseLonRadian)*m_dY;
		z = cos(dBaseLatRadian)*cos(dBaseLonRadian)*m_dX+cos(dBaseLatRadian)*sin(dBaseLonRadian)*m_dY+sin(dBaseLatRadian)*m_dZ;
	}
}

void ISolution::GetTransformPrecision(COORDSYSTEMTYPE emCoordSys, double& px, double& py, double& pz)
{
	px = m_dPX;
	py = m_dPY;
	pz = m_dPZ;
}

void ISolution::GetAllResult(COORDSYSTEMTYPE emCoordSys, BLINF & allResult)
{
	allResult = m_sAll;
	if (emCoordSys == LLS_NEU)
	{
		double dBaseLonRadian = m_dBaseLon*PI / 180.0f;
		double dBaseLatRadian = m_dBaseLat*PI / 180.0f;

		allResult.dx = -sin(dBaseLatRadian)*cos(dBaseLonRadian)*m_dX - sin(dBaseLatRadian)*sin(dBaseLonRadian)*m_dY + cos(dBaseLatRadian)*m_dZ;
		allResult.dy = -sin(dBaseLonRadian)*m_dX + cos(dBaseLonRadian)*m_dY;
		allResult.dz = cos(dBaseLatRadian)*cos(dBaseLonRadian)*m_dX + cos(dBaseLatRadian)*sin(dBaseLonRadian)*m_dY + sin(dBaseLatRadian)*m_dZ;
	}
}

void ISolution::LBHtoXYZ(double lonDeg, double latDeg, double Hgh, double& X, double& Y, double& Z)
{
	double a = 6378137.0;//椭球的长轴
	double b = 6356752.3142;//椭球的短轴
	double e = (sqrt(a*a-b*b))/a;//椭球的第一偏心率
	double W = sqrt(1-e*e*sin(latDeg*PI/180.0)*sin(latDeg*PI/180.0));
	double N = a/W;

	X = (N+Hgh)*cos(latDeg*PI/180.0)*cos(lonDeg*PI/180.0);
	Y = (N+Hgh)*cos(latDeg*PI/180.0)*sin(lonDeg*PI/180.0);
	Z = (N*(1-e*e)+Hgh)*sin(latDeg*PI/180.0);
}

void ISolution::XYZtoLBH(double X, double Y, double Z, double& lonDeg, double& latDeg, double& Hgh)
{
	// WGS-84 系大地参数
	double earth_a = 6.378137e6;			// WGS84椭球长半轴,m
	double earth_b  = 6356752.314;	    // WGS-84大地坐标系半短轴const double earth_e = 6.6943799901e-3;

	// lst add the following if section to do the parameter checking, 2008-08-28
	double p;
	double n;
	double thet;
	double epsq;
	double esq;	  

	//LLH result;

	p = sqrt (X * X + Y * Y);  //求平方根，pos.x  pos.y  pos.z为ecef坐标系中的 x,y,z值，p为中间量

	thet = atan (Z * earth_a / (p * earth_b));  //thet为中间量	  
	epsq = (earth_a * earth_a) / (earth_b * earth_b) - 1.0;  //epsq为中间量
	esq = 1.0 - (earth_b * earth_b) / (earth_a * earth_a);  //esq为中间量，esq开平方之后是地球椭球的偏心率

	latDeg =  atan ((Z + epsq * earth_b * (sin (thet)*sin(thet)*sin(thet))) / (p -  esq * earth_a *  (cos (thet)*cos(thet)*cos(thet))));  //求纬度，CUBE立方
	lonDeg =  atan2 (Y, X);  //求经度

	n = earth_a * earth_a / sqrt (earth_a * earth_a * cos (latDeg) * cos (latDeg) +
		earth_b * earth_b * sin (latDeg) * sin (latDeg));  //n

	Hgh = p / cos (latDeg) - n;  //hae
	latDeg = latDeg * 180.0 /PI;
	lonDeg = lonDeg * 180.0 /PI;
}

void ISolution::WGS84toLLSNEU(double BaseLonDeg, double BaseLatDeg, double BaseHgh, double RemoteLonDeg, double RemoteLatDeg, double RemoteHgh, double& RX, double& RY, double& RZ)
{
	double BaseXYZ[3],RemoteXYZ[3];

	LBHtoXYZ(BaseLonDeg,BaseLatDeg,BaseHgh,BaseXYZ[0],BaseXYZ[1],BaseXYZ[2]);
	LBHtoXYZ(RemoteLonDeg,RemoteLatDeg,RemoteHgh,RemoteXYZ[0],RemoteXYZ[1],RemoteXYZ[2]);

	//用户站WGS84的XYZ，转换为当地坐标系北东天下的X,Y,Z 其中X 北 Y 东 Z 天
	RX = -sin(BaseLatDeg*(PI / 180.0))*cos(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[0]-BaseXYZ[0])-sin(BaseLatDeg*(PI / 180.0))*sin(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[1]-BaseXYZ[1])+cos(BaseLatDeg*(PI / 180.0))*(RemoteXYZ[2]-BaseXYZ[2]);
	RY = -sin(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[0]-BaseXYZ[0])+cos(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[1]-BaseXYZ[1]);
	RZ = cos(BaseLatDeg*(PI / 180.0))*cos(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[0]-BaseXYZ[0])+cos(BaseLatDeg*(PI / 180.0))*sin(BaseLonDeg*(PI / 180.0))*(RemoteXYZ[1]-BaseXYZ[1])+sin(BaseLatDeg*(PI / 180.0))*(RemoteXYZ[2]-BaseXYZ[2]);
}

CString GetCPUID(void)
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned long s1, s2;
	CString str1, str2;
	// 以下为获得CPU ID的汇编语言指令

	_asm    // 得到CPU ID的高32位 
	{
		mov eax, 01h
		cpuid
		mov s1, eax
	}
	str1.Format("%X", s1);

	_asm    // 得到CPU ID的低64位
	{
		mov eax, 03h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, ecx
	}
	str2.Format("%X%X", s1, s2);

	return (str1 + str2);
}

// 解析注册码
CString UnEncrypt(const char* license)
{
	CString strKey = _T("BDStarBXJCVersion2ByMg2017"); //密码所用的密钥

	CString strMiwen(license);
	strMiwen.MakeUpper();

	CString strMingwen;
	TCHAR szElem;
	TCHAR szMingwenChar;
	for (int nIndex = 0; nIndex < strMiwen.GetLength() - 1; nIndex += 2)
	{
		szElem = strMiwen.GetAt(nIndex);
		if (_T('0') <= szElem && szElem <= _T('9'))
		{
			szMingwenChar = 16 * (szElem - _T('0'));
		}
		else if (_T('A') <= szElem && szElem <= _T('Z'))
		{
			szMingwenChar = 16 * (10 + szElem - _T('A'));
		}

		szElem = strMiwen.GetAt(nIndex + 1);
		if (_T('0') <= szElem && szElem <= _T('9'))
		{
			szMingwenChar += szElem - _T('0');
		}
		else if (_T('A') <= szElem && szElem <= _T('Z'))
		{
			szMingwenChar += 10 + szElem - _T('A');
		}
		strMingwen.AppendChar(szMingwenChar);
	}

	//异或
	for (int nIndex = 0; nIndex < strMingwen.GetLength() && nIndex < strKey.GetLength(); ++nIndex)
	{
		strMingwen.SetAt(nIndex, strMingwen.GetAt(nIndex) ^ strKey.GetAt(nIndex));
	}
	strMingwen.MakeReverse();

	return strMingwen;
}