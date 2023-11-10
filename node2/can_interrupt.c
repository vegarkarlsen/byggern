/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdint.h>
#include <stdio.h>
#include "sam.h"

#include "../uart_and_printf/printf-stdarg.h"

#include "can_controller.h"

#include "pwm_lib.h"
#include "can_handling.h"

#define DEBUG_INTERRUPT 0

uint8_t old_goals = 0;
/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);


		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

        if (message.id == 7){
            update_multiboard_vars(&message);
            // can_print(&message);
            // if (message.data_length == 8){
            //     *get_game_state_global() = message.data[7];
            // }
        } 
        //
        // else if (message.id == 1) { 
        //     update_game_state_vars(&message);    
        // }
        // can_print(&message);

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");

            CAN_MESSAGE sen_msg;
            // uint8_t *global_goals = get_goals_global();
            // if (*global_goals != old_goals){
            //     old_goals = *global_goals;
            // }
            // sen_msg.id = 2;
            // sen_msg.data_length = 1;
            // sen_msg.data[0] = 3;
            // can_send(&sen_msg, 0);
            // can_print(&sen_msg);

            // }

        // CAN_MESSAGE m = prepere_goals_package();
        // m.id = 2;
        // m.data_length = 1;
        // m.data[0] = 5;
        // CAN_MESSAGE m = prepere_goals_package();
        // can_print(&m);
        // can_send(&m, 0);
        // can_print(&m);
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
