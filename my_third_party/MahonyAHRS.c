
#include "MahonyAHRS.h"
#include <math.h>
#include <string.h> // memcpy
#include <stdint.h>
/* ??(????) */
#define Kp 2.00f
#define Ki 0.008f
#define halfT 0.005f   // = 0.5 * dt, ? dt=0.01s (100Hz) ? halfT=0.005

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.29577951308232f
#endif


float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;



float DCMgb[3][3]; 
/* ??? clamp */
static inline float clampf_local(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}


static inline float invSqrt(float x) {
    if (x <= 0.0f) return 0.0f;
    return 1.0f / sqrtf(x);
}


void IMUupdate(FLOAT_XYZ *Gyr_rad, FLOAT_XYZ *Acc_filt, FLOAT_ANGLE *Att_Angle)
{
    uint8_t i;
    float matrix[9];
    float ax = Acc_filt->X, ay = Acc_filt->Y, az = Acc_filt->Z;
    float gx = Gyr_rad->X, gy = Gyr_rad->Y, gz = Gyr_rad->Z;
    float vx, vy, vz;
    float ex, ey, ez;
    float norm;

    
    if ((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)) {
        return;
    }

  
    norm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= norm;
    ay *= norm;
    az *= norm;

  
    vx = 2.0f * (q1 * q3 - q0 * q2);
    vy = 2.0f * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

   
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    
    exInt += ex * Ki;
    eyInt += ey * Ki;
    ezInt += ez * Ki;

   
    gx += Kp * ex + exInt;
    gy += Kp * ey + eyInt;
    gz += Kp * ez + ezInt;

    
    float qa = q0, qb = q1, qc = q2, qd = q3;

    
    q0 = qa + (-qb * gx - qc * gy - qd * gz) * halfT;
    q1 = qb + (qa * gx + qc * gz - qd * gy) * halfT;
    q2 = qc + (qa * gy - qb * gz + qd * gx) * halfT;
    q3 = qd + (qa * gz + qb * gy - qc * gx) * halfT;

    
    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    if (norm > 0.0f) {
        q0 *= norm;
        q1 *= norm;
        q2 *= norm;
        q3 *= norm;
    } else {
       
        q0 = 1.0f; q1 = q2 = q3 = 0.0f;
    }

    
    float q0q0 = q0 * q0;
    float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;

    matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;
    matrix[1] = 2.0f * (q1 * q2 + q0 * q3);
    matrix[2] = 2.0f * (q1 * q3 - q0 * q2);
    matrix[3] = 2.0f * (q1 * q2 - q0 * q3);
    matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;
    matrix[5] = 2.0f * (q2 * q3 + q0 * q1);
    matrix[6] = 2.0f * (q1 * q3 + q0 * q2);
    matrix[7] = 2.0f * (q2 * q3 - q0 * q1);
    matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;

   
    memcpy(DCMgb, matrix, sizeof(matrix));

    
    float sinp = 2.0f * (q1 * q3 - q0 * q2);
    sinp = clampf_local(sinp, -1.0f, 1.0f);

    Att_Angle->yaw = atan2f(2.0f * (q1 * q2 + q0 * q3), q0q0 + q1q1 - q2q2 - q3q3) * RAD_TO_DEG;
    Att_Angle->pit = -asinf(sinp) * RAD_TO_DEG;
    Att_Angle->rol = atan2f(2.0f * (q0 * q1 + q2 * q3), q0q0 - q1q1 - q2q2 + q3q3) * RAD_TO_DEG;
}
