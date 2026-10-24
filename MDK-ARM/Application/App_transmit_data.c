#include "App_transmit_data.h"

extern Remote_Data remote_data;
uint8_t transmit_buff[TX_PLOAD_WIDTH] = {0};

uint8_t post_buff[TX_PLOAD_WIDTH] = {0};

void App_transmit_data(void)
{

    uint32_t sum = 0;

    // Send Data
    transmit_buff[0] = FRAME_HEAD_CHECK_1;
    transmit_buff[1] = FRAME_HEAD_CHECK_2;
    transmit_buff[2] = FRAME_HEAD_CHECK_3;

    transmit_buff[3] = (remote_data.thr >> 8) & 0xFF;
    transmit_buff[4] = remote_data.thr & 0xFF;

    transmit_buff[5] = (remote_data.yaw >> 8) & 0xFF;
    transmit_buff[6] = remote_data.yaw & 0xFF;

    transmit_buff[7] = (remote_data.pit >> 8) & 0xFF;
    transmit_buff[8] = remote_data.pit & 0xFF;

    transmit_buff[9] = (remote_data.rol >> 8) & 0xFF;
    transmit_buff[10] = remote_data.rol & 0xFF;

    // The Shutdown and Fixed height only have 0 1
    taskENTER_CRITICAL();
    transmit_buff[11] = remote_data.shutdown; // Finished run in this place and changing tasks
    transmit_buff[12] = remote_data.fix_height;
    remote_data.fix_height = 0;
    taskEXIT_CRITICAL();

    for (uint8_t i = 0; i < 13; i++)
    {
        sum += transmit_buff[i];
    }

    transmit_buff[13] = (sum >> 24) & 0xFF;
    transmit_buff[14] = (sum >> 16) & 0xFF;
    transmit_buff[15] = (sum >> 8) &  0xFF;
    transmit_buff[16] = sum & 0xFF;

    // Changing Automatically To Send Mode
Int_SI24R1_TX_Mode();
Int_SI24R1_TxPacket(transmit_buff);
Int_SI24R1_RX_Mode();
}
