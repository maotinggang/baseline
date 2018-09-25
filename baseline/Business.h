///////////////////////////////////////////////////////////
//  Business.h
//  Implementation of the Class CBusiness
//  Created on:      2012-03
//  Original author: sxliu
///////////////////////////////////////////////////////////
#pragma once

#include "ISolution.h"

class CBusiness
{
public:
	CBusiness(void);
	virtual ~CBusiness(void);

	// 加载解算控制模块
	bool LoadSolution();

	// 卸载解算控制模块
	void FreeSolution();

	// 获取解算控制模块
	ISolution* GetSolution();

protected:
	// 解算控制模块实体对象
	ISolution* m_pSolution;

	// 线程停止事件句柄
	HANDLE m_hStopProcess;

	// 是否已经对软件狗加密
	bool m_bEncrypt;
};
