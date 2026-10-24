#ifndef __APP_TRANSMIT_DATA__
#define __APP_TRANSMIT_DATA__

#include "Int_SI24R1.h"
#include "App_process_data.h"
#include "FreeRTOS.h"
#include "task.h"

#define FRAME_HEAD_CHECK_1 's'
#define FRAME_HEAD_CHECK_2 'g'
#define FRAME_HEAD_CHECK_3 'g'



void App_transmit_data(void);

#endif // __APP_TRANSMIT_DATA__