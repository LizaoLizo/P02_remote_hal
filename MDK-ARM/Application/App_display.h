#ifndef __APP_DISPLAY__
#define __APP_DISPLAY__

#include "Inf_OLED.h"
#include "Int_SI24R1.h"
#include "App_process_data.h"

#define LINE1_BEGIN 52
#define LINE2_BEGIN 5
#define LINE3_BEGIN 5
#define LINE3_BEGIN2 65
#define LINE4_BEGIN 5
#define LINE4_BEGIN2 65
#define BAR1_BEGIN 35
#define BAR2_BEGIN 47
#define BAR1_BEGIN2 95
#define BAR2_BEGIN2 107

#define Y0 0
#define Y1 14
#define Y2 26
#define Y3 38


void App_display_init(void);

void App_display_show(void);

#endif 