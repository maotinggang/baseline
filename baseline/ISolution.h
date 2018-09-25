///////////////////////////////////////////////////////////
//  ISolution.h
//  Implementation of the Class ISolution
//  Created on:      2012-03
//  Original author: sxliu
///////////////////////////////////////////////////////////
#pragma once
#include "IInterface_Solution.h"
#include <afxstr.h>
#include "compute.h"


class ISolution : public IInterface_Solution
{
public:
	ISolution(void);
	virtual ~ISolution(void);

	// 初始化解算模型
	bool Initialize();

	// 解算数据
	int GPSDecord(char* sBaseFile, char* sRemoteFile);

	// 获得解算后的基线坐标值
	void GetTransformValue(COORDSYSTEMTYPE emCoordSys, double& x, double& y, double& z);

	// 获得解算后的位移精度
	void GetTransformPrecision(COORDSYSTEMTYPE emCoordSys, double& px, double& py, double& pz);

	//获得所有解算结果
	void GetAllResult(COORDSYSTEMTYPE emCoordSys, BLINF& allResult);

	// 设置固定站初始地理位置坐标
	void SetBaseLBH(double lonDeg, double latDeg, double Hgh);

	// 设置验证状态
	void SetIsVerified(bool bVerify);

	// 地理坐标向地心坐标转换
	void LBHtoXYZ(double lonDeg, double latDeg, double Hgh, double& X, double& Y, double& Z);

	// 地心坐标向地理坐标转换
	void XYZtoLBH(double X, double Y, double Z, double& lonDeg, double& latDeg, double& Hgh);

	// 局部坐标系定义:指定任意点为原点,该点正北方向为X,正东为Y,正上方为Z
	// 地理坐标向指定原点的地面局部坐标转换,BaseLonDeg为参考原点的坐标
	void WGS84toLLSNEU(double BaseLonDeg, double BaseLatDeg, double BaseHgh,
		double RemoteLonDeg, double RemoteLatDeg, double RemoteHgh, double& RX, double& RY, double& RZ);
	
private:
	// 信号互斥量
	CRITICAL_SECTION m_csLock;

	// 验证是否通过
	bool m_bVerified;

protected:
	// 解算参数
	int m_nGPS;
	int m_nGlonass;
	int m_nBeidou;
	int m_nFreq;
	int m_nEpSample;
	int m_nCutOff;
	double m_dRatio;
	double m_dRms;

	// 固定站初始地理位置坐标值
	double m_dBaseLon;
	double m_dBaseLat;
	double m_dBaseHgh;

	// 当前解算后的基线坐标值
	double m_dX;
	double m_dY;
	double m_dZ;

	// 当前解算后的位移精度
	double m_dPX;
	double m_dPY;
	double m_dPZ;

	//当前所有解算结果
	BLINF m_sAll;
};
