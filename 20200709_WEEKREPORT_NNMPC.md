# Neural network model predictive control numerical solution method  

WeifanWang  

School of Automation,HangZhouDianZi University  

1 The objective function  

$$
m i n~~~J=\sum_{k=1}^{n}(x_{r k}-\hat{x}_{k})^{2}
$$  

$x_{r k}$ and $\hat{x}_{k}$ is future reference and future predict,k is the time step.  

Based on the known information at sample time step k=0(To simplify the illustration, replace the current sampling time with k=0) and future input,the future state of the system is predicted:  

$$
\left\{\begin{array}{l l}{\hat{x}_{1}=f_{n e t}(X_{0},u_{0})=f_{0}(X_{0},u_{0})}\\ {\hat{x}_{2}=f_{n e t}(\hat{X}_{1},u_{1})=f_{1}(X_{0},u_{0},u_{1})}\\ {\hat{x}_{3}=f_{n e t}(\hat{X}_{2},u_{2})=f_{2}(X_{0},u_{0},u_{1},u_{2})}\\ {\quad\vdots}\\ {\hat{x}_{k}=f_{n e t}(\hat{X}_{k-1},u_{k-1})=f_{k-1}(X_{0},u_{0},u_{1},\cdots,u_{k-1})}\\ {\vdots}\end{array}\right.
$$  

$$
\begin{array}{r}{\left\{\begin{array}{l l}{X_{0}=[x_{0},x_{-1},\cdots\,,u_{-1},u_{-2},\cdots]}\\ {\hat{X}_{1}=[\hat{x}_{1},x_{0},\cdots\,,u_{0},u_{-1},\cdots]}\\ {\hat{X}_{2}=[\hat{x}_{2},\hat{x}_{1},\cdots\,,u_{1},u_{0},\cdots]}\\ {\;\;\;\vdots}\end{array}\right.}\end{array}
$$  

$f_{n e t}$ is the system model based on neural network, $X_{0}$ is the known information of the current sampling time, $\hat{X}_{k}$ is known information, but it contains estimated information. $u_{0},u_{1},\cdot\cdot\cdot\,,u_{k-1}$ is the future control input.  

2 Rewrite the objective function The objective function in section 1 can be write as follow using formula(1)(2)(3) :  

$$
\begin{array}{l}{\displaystyle\{m i n\quad J=\sum_{k=1}^{n}(x_{r k}-f_{k-1}(X_{0},u_{1},u_{2},\cdots,u_{i}))^{2}}\\ {\displaystyle i<k,i=0,i=1,i=2,\cdots}\end{array}
$$  

Because we know $x_{r k}$ and $X_{0}$ , we can rewrite our objective function as follow:  

$$
\begin{array}{r l}{m i n}&{{}J=\left(u_{0},u_{1},u_{2},\cdot\cdot\cdot\right)}\end{array}
$$  

Therefore, the solution of neural network MPC is a multivariable quadratic optimization problem  

3 Get the solution of Neural Network MPC using Gradient descent method (numerical solution)  

It is difficult to obtain analytic expressions in practical problems, so it is difficult to obtain analytic solutions  

So you can use the numerical method to solve for partial derivatives.The control input is updated using a method like gradient descent:  

$$
\left\{u_{0}=u_{0}-\alpha\frac{J(u_{0}+\Delta u,u_{1},\cdot\cdot\cdot,u_{n})-J(u_{0},u_{1},\cdot\cdot\cdot,u_{n})}{\Delta u}\right.
$$  

$u_{0}$ will be applied to the system as the control input for the current sampling moment.  

# 4 Future Work  

In the future, SEA position control simulation based on neural network MPC will adopt the above solution  