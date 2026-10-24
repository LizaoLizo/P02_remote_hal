#ifndef __INT_KEY__
#define __INT_KEY__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_RIGHT_X_LONG
}Key_type;

Key_type Int_key_get(void);


#endif // __INT_KEY__