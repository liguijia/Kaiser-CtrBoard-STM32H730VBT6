#include "vofa.h"
#include "usbd_cdc_if.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "math.h"
//#include "usart.h"

#define MAX_BUFFER_SIZE 1024
uint8_t send_buf[MAX_BUFFER_SIZE];
uint16_t cnt = 0;

/**
***********************************************************************
* @brief:      vofa_start(void)
* @param:		   void
* @retval:     void
* @details:    ???????????¦Ë??
***********************************************************************
**/
void vofa_start(void)
{
	vofa_demo();		// demo???
	
//	vofa_sendframetail();
}

/**
***********************************************************************
* @brief:      vofa_transmit(uint8_t* buf, uint16_t len)
* @param:		void
* @retval:     void
* @details:    ??????????USART????USB
***********************************************************************
**/
void vofa_transmit(uint8_t* buf, uint16_t len)
{
//	HAL_UART_Transmit(&huart3, (uint8_t *)buf, len, 0xFFFF);
	CDC_Transmit_HS((uint8_t *)buf, len);
}
/**
***********************************************************************
* @brief:      vofa_send_data(float data)
* @param[in]:  num: ?????? data: ???? 
* @retval:     void
* @details:    ?????????????????
***********************************************************************
**/
void vofa_send_data(uint8_t num, float data) 
{
	send_buf[cnt++] = byte0(data);
	send_buf[cnt++] = byte1(data);
	send_buf[cnt++] = byte2(data);
	send_buf[cnt++] = byte3(data);
}
/**
***********************************************************************
* @brief      vofa_sendframetail(void)
* @param      NULL 
* @retval     void
* @details:   ????????????¦Â
***********************************************************************
**/
void vofa_sendframetail(void) 
{
	send_buf[cnt++] = 0x00;
	send_buf[cnt++] = 0x00;
	send_buf[cnt++] = 0x80;
	send_buf[cnt++] = 0x7f;
	
	/* ????????¦Â??????? */
	vofa_transmit((uint8_t *)send_buf, cnt);
	cnt = 0;// ??¦Ç??????¦Â?????????
}
/**
***********************************************************************
* @brief      vofa_demo(void)
* @param      NULL 
* @retval     void
* @details:   demo???
***********************************************************************
**/
void vofa_demo(void) 
{
	static float scnt = 0.0f;

	scnt += 0.01f;

	if(scnt >= 360.0f)
		scnt = 0.0f;

	float v1 = scnt;
	float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	float v3 = sin((double)(scnt + 120) / 180 * 3.14159) * 180 + 180;
	float v4 = sin((double)(scnt + 240) / 180 * 3.14159) * 180 + 180;

	// Call the function to store the data in the buffer
	vofa_send_data(0, v1);
	vofa_send_data(1, v2);
	vofa_send_data(2, v3);
	vofa_send_data(3, v4);

	// Call the function to send the frame tail
	vofa_sendframetail();
}













