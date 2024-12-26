#ifndef _GRC_DEFS_H_
#define _GRC_DEFS_H_

#include "sdkconfig.h"

#define GRC_DEMO_AD4FAN_NAME           "AD4fan"
#define GRC_DEMO_AD4FAN_RESULT_MODE_1  "MODE1"
#define GRC_DEMO_AD4FAN_RESULT_MODE_2  "MODE2"
#define GRC_DEMO_AD4FAN_RESULT_MODE_3  "MODE3"
#define GRC_DEMO_AD4FAN_RESULT_MODE_4  "MODE4"
#define GRC_DEMO_AD4FAN_RESULT_ANOMALY "ANOMALY"

#define GRC_DEMO_AI_RHYTHM_KEY_NAME             "AIRhythmKey"
#define GRC_DEMO_AI_RHYTHM_KEY_RESULT_CORRECT   "correct"
#define GRC_DEMO_AI_RHYTHM_KEY_RESULT_INCORRECT "incorrect"
#define GRC_DEMO_AI_RHYTHM_KEY_RESULT_LOCKED    "locked"
#define GRC_DEMO_AI_RHYTHM_KEY_RESULT_UNLOCKED  "unlocked"

#define GRC_DEMO_AI_TEACHER_NAME             "AIteacher"
#define GRC_DEMO_AI_TEACHER_RESULT_CORRECT   "correct"
#define GRC_DEMO_AI_TEACHER_RESULT_INCORRECT "incorrect"

#define GRC_DEMO_ROBOT_CONTROL_NAME "RobotControl"

#define GRC_DEMO_VOICE_PIN_NAME             "VoicePIN"
#define GRC_DEMO_VOICE_PIN_RESULT_CORRECT   "correct"
#define GRC_DEMO_VOICE_PIN_RESULT_INCORRECT "incorrect"
#define GRC_DEMO_VOICE_PIN_RESULT_LOCKED    "locked"
#define GRC_DEMO_VOICE_PIN_RESULT_UNLOCKED  "unlocked"

#define GRC_BUTTON_0_NAME "USER"
#define GRC_BUTTON_1_NAME "SW3"
#define GRC_BUTTON_2_NAME "SW4"

#define GRC_BUTTON_NAMES_STR                                                   \
  ("<" GRC_BUTTON_0_NAME "|" GRC_BUTTON_1_NAME "|" GRC_BUTTON_2_NAME ">")

#define GRC_DEMO_NAMES_STR                                                     \
  ("<" GRC_DEMO_ROBOT_CONTROL_NAME "|" GRC_DEMO_AI_TEACHER_NAME                \
   "|" GRC_DEMO_VOICE_PIN_NAME "|" GRC_DEMO_AD4FAN_NAME                        \
   "|" GRC_DEMO_AI_RHYTHM_KEY_NAME ">")

#endif // _GRC_DEFS_H_
