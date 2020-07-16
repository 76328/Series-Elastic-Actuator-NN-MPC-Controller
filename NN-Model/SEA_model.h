#ifndef MODEL_H
#define MODEL_H


#define INNUM 10
#define HNUM 30

typedef struct _nn_model NN_MODEL;

struct _nn_model
{
	float input[10];
	float h[30];
	float out;
	float IW[30][10];
	float LW2[30];
	float B1[30];
	float B2;
	float haux[30];
	
	void (*FF_GetNextOutput)(NN_MODEL *p);
};

extern NN_MODEL PredictModel;
void module_model_init(void);

#endif
