#include "pid.h"

void myPID::setTuning(float kp, float ki, float kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}
float myPID::normal_pid(float SP, float FB)
{
  float err=SP-FB;
   m_integral+=err;
   float out=0;
   out=m_kp* err+ m_ki*( m_integral)+ m_kd*( err- m_prev_error);
    m_prev_error= err;
   return out;
}
float myPID::integral_limit_pid(float SP,float FB){

    float err=SP-FB;
   float out=0;
   if(  m_ki!=0){
          m_integral+=  err;
   }
   else{
         m_integral=0;
   }
   if (  m_integral > 100)    m_integral = 100;
   if (  m_integral < -100)   m_integral = -100;
   out=  m_kp*  err+ m_ki*(  m_integral)+  m_kd*(  err-  m_prev_error);
     m_prev_error=  err;
   return out;



}
void myPID::setSampleTime(float sample_time_s)
{
      if (sample_time_s <= 0) return;
    
    // 当采样时间变化时，按比例调整Ki和Kd
    float ratio = sample_time_s / m_sample_time;
    m_ki *= ratio;
    m_kd /= ratio;
    m_sample_time = sample_time_s;
}
void myPID::setOutputLimits(float min, float max)
{
    if (min >= max) return;
    m_output_min = min;
    m_output_max = max;

}
void myPID::reset(){

      m_integral = 0.0f;
      m_prev_error = 0.0f;
      m_prev_feedback = 0.0f;

}
void myPID::enableIntegralSeparation(bool enable, float threshold)
{
    m_use_integral_separation = enable;
    m_integral_separation_threshold = threshold;
}
void myPID::enableDerivativeOnMeasurement(bool enable)
{
    m_use_derivative_on_measurement = enable;
}
myPID::myPID(float kp, float ki, float kd, float sample_time_s)
    : m_kp(kp), m_ki(ki), m_kd(kd),
      m_integral(0.0f), m_prev_error(0.0f), m_prev_feedback(0.0f),
      m_sample_time(sample_time_s), m_output_min(-100.0f), m_output_max(100.0f),
      m_integral_min(-100.0f), m_integral_max(100.0f),
      m_use_integral_separation(false), m_integral_separation_threshold(0.0f),
      m_use_derivative_on_measurement(false)
{
    if (m_sample_time <= 0) m_sample_time = 0.01f; // 默认采样时间
}
float myPID::compute(float setpoint, float feedback, float feedforward)
{
    float error = setpoint - feedback;

    // 2. 积分项
    if (!m_use_integral_separation || (fabs(error) < m_integral_separation_threshold))
    {
        m_integral += m_ki * error;
        // 积分限幅 (Anti-windup)
        if (m_integral > m_integral_max) m_integral = m_integral_max;
        else if (m_integral < m_integral_min) m_integral = m_integral_min;
    }

    // 3. 比例项
    float p_term = m_kp * error;

    // 4. 微分项
    float d_term = 0.0f;
    if (m_use_derivative_on_measurement)
        d_term = -m_kd * (feedback - m_prev_feedback); // 微分先行
    else
        d_term = m_kd * (error - m_prev_error);       // 标准微分

    // 5. 计算总输出
    float output = p_term + m_integral + d_term + feedforward;

    // 6. 输出限幅
    if (output > m_output_max) output = m_output_max;
    else if (output < m_output_min) output = m_output_min;

    // 7. 更新历史值
    m_prev_error = error;
    m_prev_feedback = feedback;

    return output;
}
