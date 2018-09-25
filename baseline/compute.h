#ifndef LIB_H
#define LIB_H


typedef struct BlInf 
{
	//输入参数
	char BaseName[120];                //输入文件名
	char RoveName[120];                //输出文件名
	int epBegin;		               //基线解算起始历元	
	int epEnd;                         //基线解算终止历元
	int epSample;                      //历元采样间隔 
	int CutOff;                        //截止角(度) 
	int SolG;                          //GPS解算     1: 解算 0:不解算 
	int SolN;                          //Glonass解算 1: 解算 0:不解算 
	int SolB;                          //Bd解算      1: 解算 0:不解算 
	int Freq;                          //多频解算    0: L1(B1) 1:双频 2:三频 
	
	//输入输出参数
	double x0;double y0; double z0;    //固定站坐标	
    double dx; double dy; double dz;   //基线坐标
									   //对ExProBaseline()为输入参数
	
	//输出参数
	double dis;						   //基线长      
	double px; double py; double pz;   //x,y,z方向的解算精度
	double ratio;                      //ratio
	double rms;                        //rms
	int epoch;                         //历元数 
	int mear;                          //观测值数
	int reject;                        //剔除观测值数
	double rdop;                       //RDOP
	double SecBegin;                   //起始时间 (GPS second)       
	double SecEnd;                     //终止时间 (GPS second)
} BLINF;

extern "C"  int __declspec(dllexport) ProBaseline(BLINF *);//基线解算
extern "C"  int __declspec(dllexport) ExProBaseline(BLINF *);//附加信息基线解算

//返回值
// -1    无法打开数据文件
// -2    数据文件没有Gnss观测数据
// -3    数据质量坏,无法解算
//  1    解算成功
 
#endif 