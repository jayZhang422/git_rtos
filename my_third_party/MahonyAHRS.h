#ifndef __MAHONY_AHRS_H__
#define __MAHONY_AHRS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    float X;
    float Y;
    float Z;
} FLOAT_XYZ;


typedef struct {
    float rol;   // Roll
    float pit;   // Pitch
    float yaw;   // Yaw
} FLOAT_ANGLE;


extern float q0, q1, q2, q3;
extern float exInt, eyInt, ezInt;
extern float DCMgb[3][3];


 float invSqrt(float x);


void IMUupdate(FLOAT_XYZ *Gyr_rad, FLOAT_XYZ *Acc_filt, FLOAT_ANGLE *Att_Angle);

#ifdef __cplusplus
}
#endif

#endif // __MAHONY_AHRS_H__

