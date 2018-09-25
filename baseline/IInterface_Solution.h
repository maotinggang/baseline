///////////////////////////////////////////////////////////
//  IInterface_Solution.h
//  Implementation of the Class IInterface_Solution
//  Created on:      2012-03
//  Original author: sxliu
///////////////////////////////////////////////////////////
#pragma once
#include "compute.h"

class IInterface_Solution
{
public:
	/// <summary>
	/// 坐标参考系
	/// </summary>
	typedef enum COORDSYSTEMTYPE
	{
		// WGS84地心坐标系
		WGS84_XYZ,
		// 北东天局部坐标系
		LLS_NEU
	}COORDSYSTEMTYPE;

public:
	/// <summary>
	/// 初始化解算模块
	/// </summary>
	virtual bool Initialize() = 0;

	/// <summary>
	/// 解算数据
	/// </summary>
	/// <param name="sBaseFile">基准点数据文件名</param>
	/// <param name="sRemoteFile">监测点数据文件名</param>
	virtual int GPSDecord(char* sBaseFile, char* sRemoteFile) = 0;

	/// <summary>
	/// 获得解算后的基线坐标值
	/// </summary>
	/// <param name="emCoordSys">坐标参考系类型</param>
	/// <param name="x">监测点基线坐标x值</param>
	/// <param name="y">监测点基线坐标y值</param>
	/// <param name="z">监测点基线坐标z值</param>
	virtual void GetTransformValue(COORDSYSTEMTYPE emCoordSys, double& x, double& y, double& z) = 0;

	/// <summary>
	/// 获得解算后的位移精度
	/// </summary>
	/// <param name="emCoordSys">坐标参考系类型</param>
	/// <param name="px">监测点x方向位移精度</param>
	/// <param name="py">监测点y方向位移精度</param>
	/// <param name="pz">监测点z方向位移精度</param>
	virtual void GetTransformPrecision(COORDSYSTEMTYPE emCoordSys, double& px, double& py, double& pz) = 0;

	/// <summary>
	/// 获得所有解算结果
	/// </summary>
	/// <param name="emCoordSys">坐标参考系类型</param>
	/// <param name="allResult">所有解算结果</param>
	virtual void GetAllResult(COORDSYSTEMTYPE emCoordSys, BLINF& allResult) = 0;

	/// <summary>
	/// 设置基准站固定初始位置坐标值
	/// </summary>
	/// <param name="lonDeg">经度 单位 度</param>
	/// <param name="latDeg">纬度 单位 度</param>
	/// <param name="Hgh">高</param>
	virtual void SetBaseLBH(double lonDeg, double latDeg, double Hgh) = 0;

	/// <summary>
	/// WGS84经纬度转换为WGS84XYZ坐标
	/// </summary>
	/// <param name="lonDeg">经度 单位 度</param>
	/// <param name="latDeg">纬度 单位 度</param>
	/// <param name="Hgh">高</param>
	/// <param name="X">输出WGS84的X坐标</param>
	/// <param name="Y">输出WGS84的Y坐标</param>
	/// <param name="Z">输出WGS84的Z坐标</param>
	virtual void LBHtoXYZ(double lonDeg, double latDeg, double Hgh, double& X, double& Y, double& Z) = 0;

	/// <summary>
	/// WGS84XYZ转换为WGS84经纬度坐标
	/// </summary>
	/// <param name="X">WGS84X坐标</param>
	/// <param name="Y">WGS84Y坐标</param>
	/// <param name="Z">WGS84Z坐标</param>
	/// <param name="lonDeg">输出经度 单位 度</param>
	/// <param name="latDeg">输出纬度 单位 度</param>
	/// <param name="Hgh">输出高</param>
	virtual void XYZtoLBH(double X, double Y, double Z, double& lonDeg, double& latDeg, double& Hgh) = 0;

	/// <summary>
	/// WGS84经纬度转换为地平系下的坐标
	///坐标系定义为：北东天，其中X轴为北，Y轴为东，Z轴为天
	/// </summary>
	/// <param name="BaseLonDeg">地平系坐标原点的经度</param>
	/// <param name="BaseLatDeg">地平系坐标原点的纬度</param>
	/// <param name="BaseHgh">地平系坐标原点的高</param>
	/// <param name="RemoteLonDeg">待转换点的经度</param>
	/// <param name="RemoteLatDeg">待转换点的纬度</param>
	/// <param name="RemoteHgh">待转换点的高</param>
	/// <param name="RX">北向坐标</param>
	/// <param name="RY">东向坐标</param>
	/// <param name="RZ">天向坐标</param>
	virtual void WGS84toLLSNEU(double BaseLonDeg, double BaseLatDeg, double BaseHgh, 
		double RemoteLonDeg, double RemoteLatDeg, double RemoteHgh, double& RX, double& RY, double& RZ) = 0;
};

//创建解算控制模块
_declspec(dllexport) bool CreateSolutionModule();

//释放解算控制模块
_declspec(dllexport) void ReleaseSolutionModule();

//解算数据 返回值为0,表明解算成功,否则,表明解算失败
/// <param name="sBaseFile">基准点数据文件名</param>
/// <param name="sRemoteFile">监测点数据文件名</param>
/// <param name="x">监测点基线坐标x值</param>
/// <param name="y">监测点基线坐标y值</param>
/// <param name="z">监测点基线坐标z值</param>
/// <param name="px">监测点x方向位移精度</param>
/// <param name="py">监测点y方向位移精度</param>
/// <param name="pz">监测点z方向位移精度</param>
/// <param name="coordSysType">坐标参考系类型 0-WGS84地心坐标系 1-北东天局部坐标系(默认)</param>
_declspec(dllexport) int BaseLine(char* sBaseFile, char* sRemoteFile,
								   double& x, double& y, double& z,
								   double& px, double& py, double& pz, int coordSysType = 1);

//所有解算数据 返回值为0,表明解算成功,否则,表明解算失败
/// <param name="sBaseFile">基准点数据文件名</param>
/// <param name="sRemoteFile">监测点数据文件名</param>
/// <param name="allResult">所有解算结果</param>
/// <param name="coordSysType">坐标参考系类型 0-WGS84地心坐标系 1-北东天局部坐标系(默认)</param>
_declspec(dllexport) int AllResult(char* sBaseFile, char* sRemoteFile,
	BLINF& allResult, int coordSysType = 1);

/// <summary>
/// 设置基准站固定初始位置坐标值
/// 可通过修改配置文件的方式，也可在程序中调用该接口进行重新设置
/// </summary>
/// <param name="lonDeg">经度 单位 度</param>
/// <param name="latDeg">纬度 单位 度</param>
/// <param name="Hgh">高</param>
_declspec(dllexport) void SetBaseLBH(double lonDeg, double latDeg, double Hgh);


//////////////////////////////////////////////////////////////////////////
// 以下为常见的几种坐标转换接口函数，用户可根据实际情况调用相关接口

/// <summary>
/// WGS84经纬度转换为WGS84XYZ坐标
/// </summary>
/// <param name="lonDeg">经度 单位 度</param>
/// <param name="latDeg">纬度 单位 度</param>
/// <param name="Hgh">高</param>
/// <param name="X">输出WGS84的X坐标</param>
/// <param name="Y">输出WGS84的Y坐标</param>
/// <param name="Z">输出WGS84的Z坐标</param>
_declspec(dllexport) void LBHtoXYZ(double lonDeg, double latDeg, double Hgh, double& X, double& Y, double& Z);

/// <summary>
/// WGS84XYZ转换为WGS84经纬度坐标
/// </summary>
/// <param name="X">WGS84X坐标</param>
/// <param name="Y">WGS84Y坐标</param>
/// <param name="Z">WGS84Z坐标</param>
/// <param name="lonDeg">输出经度 单位 度</param>
/// <param name="latDeg">输出纬度 单位 度</param>
/// <param name="Hgh">输出高</param>
_declspec(dllexport) void XYZtoLBH(double X, double Y, double Z, double& lonDeg, double& latDeg, double& Hgh);

/// <summary>
/// WGS84经纬度转换为地平系下的坐标
///坐标系定义为：北东天，其中X轴为北，Y轴为东，Z轴为天
/// </summary>
/// <param name="BaseLonDeg">地平系坐标原点的经度</param>
/// <param name="BaseLatDeg">地平系坐标原点的纬度</param>
/// <param name="BaseHgh">地平系坐标原点的高</param>
/// <param name="RemoteLonDeg">待转换点的经度</param>
/// <param name="RemoteLatDeg">待转换点的纬度</param>
/// <param name="RemoteHgh">待转换点的高</param>
/// <param name="RX">北向坐标</param>
/// <param name="RY">东向坐标</param>
/// <param name="RZ">天向坐标</param>
_declspec(dllexport) void WGS84toLLSNEU(double BaseLonDeg, double BaseLatDeg, double BaseHgh, 
						   double RemoteLonDeg, double RemoteLatDeg, double RemoteHgh, double& RX, double& RY, double& RZ);