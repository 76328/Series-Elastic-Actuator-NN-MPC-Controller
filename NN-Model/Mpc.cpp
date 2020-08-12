#include "Mpc.h"
#include "math.h"
#include "common.h"
#include <iostream>
#include "Opt.h"

#define USE_ADAM
using namespace std;
MPC myMPC;

double Mpc_Cal(MPC *p,double *xh_init,double *vol_init,double target)
{
	int iteraux=1;		 //IT=1防止ADAM 迭代项无穷大
	double u[10];
	double u2[10];
	double du[10];
	double inputxh[5];
	double inputvol[5];
	double inputxh2[5];
	double inputvol2[5];

	double upd[10];

	double optsum=0;
	double doptsum=0;
	double ref=target;
	double err,err_pow;
	for (int i = 0; i < 5; i++)
	{
		inputxh[i] = xh_init[i];
		inputvol[i] = vol_init[i];
		inputxh2[i] = xh_init[i];
		inputvol2[i] = vol_init[i];

		//cout << "Input_init" << "xh" << inputxh[i] << "vol" << inputvol[i] << endl;
	}	
	for (int i=0;i<10;i++)
	{
		u[i] = vol_init[4];
		u2[i] = vol_init[4];
		du[i] = 0;		
	}
	while (iteraux < p->iter)	 //开始梯度迭代
	{
		for (int pre_du = 0; pre_du < p->pre_horizon; pre_du++)
		{
			//Get u(1) u(pre_du)+du u(3)...
			for (int update=0; update <p->pre_horizon; update++)
			{
				u2[update] = u[update];
			}
			u2[pre_du] = u[pre_du] + p->du;
			for (int i = 0; i < 5; i++)
			{
				inputxh[i] = xh_init[i];
				inputvol[i] = vol_init[i];
				inputxh2[i] = xh_init[i];
				inputvol2[i] = vol_init[i];
			}


			//使用u(1) u(2) u(3)预测
			for (int pre = 0; pre < p->pre_horizon; pre++)
			{
				inputvol[4] = u[pre];

				for (int i = 0; i < 5; i++)
				{
					p->predict.input[i] = inputxh[i];
					p->predict.input[i+5] = inputvol[i];
				}
				p->predict.FF_GetNextOutput(&p->predict);		 //神经网络前向计算f(u) 
				
				err = ref - (p->predict.out);
				err_pow = pow(err, 2);						  //神经网络前向计算J(u)
				optsum = optsum + err_pow;
				for (int i = 0; i < 4; i++)
				{
					inputxh[i] = inputxh[i + 1];
				}
				inputxh[4] = p->predict.out;

				for (int i = 0; i < 4; i++)
				{
					inputvol[i] = inputvol[i + 1];
				}
			}
			//使用u(1) u(pre)+delta u u(3)预测
			for (int pre=0;pre<p->pre_horizon;pre++)
			{
				inputvol2[4] = u2[pre];
				for (int i=0;i<5;i++)
				{
					p->d_predict.input[i] = inputxh2[i];
					p->d_predict.input[i + 5] = inputvol2[i];
				}
				p->d_predict.FF_GetNextOutput(&p->d_predict);	   //神经网络前向计算f(u+delta u)
				
				err = ref - (p->d_predict.out);
				err_pow = pow(err,2);
				doptsum = doptsum + err_pow;					//神经网络前向计算J(u+delta u)	 
				for (int i=0;i<4;i++)
				{
					inputxh2[i] = inputxh2[i + 1];
				}
				inputxh2[4] = p->d_predict.out;
				for (int i=0;i<4;i++)
				{
					inputvol2[i] = inputvol2[i + 1];
				}

			}
			//获取u(k) u(k+1)....的梯度
			du[pre_du] = (doptsum - optsum)/myMPC.du;			 //J(u+delta u)-J(u)  ....
			optsum = 0;
			doptsum = 0;
		}
		//基于负梯度更新 u(k) u(k+1)...
#ifdef USE_ADAM
		Gradient2Update(du, 10, upd, &mpcsolver, iteraux);		//Use ADAM_SOLVER Get Update
#endif
		for (int update_u=0;update_u<p->pre_horizon;update_u++)	 //控制量更新
		{
			//u[update_u] = u[update_u] - p->lr * du[update_u];
			//
			//cout <<"UPD:"<<upd[update_u] << endl;
#ifdef USE_ADAM
			u[update_u] = u[update_u] + upd[update_u];
#endif
#ifdef USE_GRD
			u[update_u] = u[update_u] - p->lr * du[update_u];
#endif
			if (u[update_u] > 24)	 //控制约束
			{
				u[update_u] = 24;
			}
			if (u[update_u] < -24)
			{
				u[update_u] = -24;
			}
		}
		iteraux++;
	}
	OptInit();
	return u[0];	   //返回控制量u[0]
}

void MpcInit()
{
	int k = 0;
	double inputxh[5] = {0,0,0,0,0};  
	NNModelModuleRegister();	//神经网络模型初始化
	OptInit();
	myMPC.d_predict = PredictModel;	   //该项用来辅助计算偏导
	myMPC.predict = PredictModel;		//预测模型
	myMPC.pre_horizon = 10;				//预测步长
	myMPC.iter = 10;					//梯度下降迭代步数
	myMPC.du = 0.05;					//delta u用来计算偏导
	myMPC.lr =400;						//梯度下降率（学习率）
}
