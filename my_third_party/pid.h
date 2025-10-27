#ifndef _PID_H
#define _PID_H

#include <stdint.h>
#include "math.h"
class myPID
{

private:
    float m_kp;
    float m_ki;
    float m_kd;

    // 状态变量
    float m_integral;
    float m_prev_error;
    float m_prev_feedback;

    // 配置
    float m_sample_time; // 采样时间 (s)
    float m_output_min;
    float m_output_max;
    float m_integral_min;
    float m_integral_max;
    
    // 功能开关
    bool  m_use_integral_separation;
    float m_integral_separation_threshold;
    bool  m_use_derivative_on_measurement;
public:
    myPID(float kp = 0.0f, float ki = 0.0f, float kd = 0.0f, float sample_time_s = 0.01f);
    void setTuning(float kp, float ki, float kd);
    float normal_pid(float SP,float FB);//最常规即单环pid
    float integral_limit_pid(float SP,float FB);
    void setSampleTime(float sample_time_s);//设置采样时间
    void setOutputLimits(float min, float max);//设置输出限幅
    void reset();//重置PID计算器
    // 开启/关闭积分分离功能
    void enableIntegralSeparation(bool enable, float threshold = 0.0f);
    // 开启/关闭微分先行 (Derivative on Measurement)
    void enableDerivativeOnMeasurement(bool enable);
    float compute(float setpoint, float feedback, float feedforward = 0.0f);
};


#endif
