#pragma once
#ifndef MPC_H
#define MPC_H
#include "NN_Model.h"

typedef struct _mpc MPC;

struct _mpc
{
	NN_MODEL predict;		//预测模型
	NN_MODEL d_predict;		//辅助计算梯度用
	int iter;				//梯度迭代步数
	int pre_horizon;		//预测步长
	double du;				//delta u辅助计算梯度用
	double lr;				//梯度下降学习率
};

extern MPC myMPC;
double Mpc_Cal(MPC* p, double* xh_init, double* vol_init, double target);	//神经网络MPC计算
void MpcInit();			  //神经网络MPC算法初始化

#endif // !MPC_H

