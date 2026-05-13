#ifndef COMMOM_H 
#define COMMOM_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <string>
#include <valarray>                   // for valarray
#include <cmath>                      // for sqrt() and pow()
#include <sstream>
#include <cstring>
#include <cstdlib>


using namespace std;

#define  MAX_BUF_SIZE 512             //�ַ���󻺴���
#define	 MAXDIMENSION	20              //�������ά��
#define	 MAXCLUSTER		10              //��������
#define  MAXLABEL  5                  //����ǩ��
#define  CLUSTER_PRECISION  0.1       //���ྫ������Ϊ10%
#define  MAXLEVEL  8                  //���ľ������
#define  INTERLEVEL  3                //���ľ������

/********************** ��¼�Ľṹ *************************/
struct strMyRecord
{
	float fProtocolType;                //02 Э������
	float fService;                     //03 ��������
	float fStatusFlag;                  //04 ״̬��־
	float fSrcBytes;                    //05 Դ��Ŀ���ֽ���
	float fDestBytes;                   //06 Ŀ�ĵ�Դ�ֽ���
	float fFailedLogins;                //11 ��¼ʧ�ܴ���
	float fNumofRoot;                   //16 root�û�Ȩ�޴�ȡ����
	float fCount;                       //23 2����������ͬ������Ŀ
	float fSrvCount;                    //24 2����������ͬ�˿���Ŀ
	float fRerrorRate;                  //27 "REJ"���������������
	float fSameSrvRate;                 //29 ���ӵ���ͬ�˿�������
	float fDiffSrvRate;                 //30 ���ӵ���ͬ�˿�������
	float fDstHostSrvCount;             //33 ��ͬĿ�ĵ���ͬ�˿�������
	float fDstHostSameSrvRate;          //34 ��ͬĿ�ĵ���ͬ�˿�����������
	float fDstHostDiffSrvRate;          //35 ��ͬĿ�ĵز�ͬ�˿�����������
	float fDstHostSameSrcPortRate;      //36 ��ͬĿ�ĵ���ͬԴ�˿����ӱ���
	float fDstHostSrvDiffHostRate;      //37 ��ͬ����������ͬ�˿ڱ���
	float fDstHostSrvSerrorRate;        //39 ���ӵ�ǰ������S0����ı���
	int   iLabel;                       //42 ���ͱ�ǩ
};

#endif
