#pragma once
#ifndef MPC_H
#define MPC_H
#include "NN_Model.h"

typedef struct _mpc MPC;

struct _mpc
{
	NN_MODEL predict;		//Ԥ��ģ��
	NN_MODEL d_predict;		//���������ݶ���
	int iter;				//�ݶȵ�������
	int pre_horizon;		//Ԥ�ⲽ��
	double du;				//delta u���������ݶ���
	double lr;				//�ݶ��½�ѧϰ��
};

extern MPC myMPC;
double Mpc_Cal(MPC* p, double* xh_init, double* vol_init, double target);	//������MPC����
void MpcInit();			  //������MPC�㷨��ʼ��

#endif // !MPC_H

