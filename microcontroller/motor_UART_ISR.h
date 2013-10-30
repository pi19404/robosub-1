/*
 * File:   uart.h
 * Author: Ian
 *
 * Created on September 27, 2013, 10:34 AM
 */

#ifndef MOTOR_UART_ISR_H
#define	MOTOR_UART_ISR_H

/*************************************************************************
 System Includes
 ************************************************************************/
#include <peripheral/ports.h>
#include <peripheral/uart.h>

/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/
#define UART2QueueSize 32
/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct uart2_node
{
    uint8 uart_data[4];
//    uint8 sensor_id;
//    uint8 sensor_sub_id;
//    uint8 data_h;
//    uint8 data_l;
}UART2_NODE;

typedef struct uart2_queue
{
    UART2_NODE DataBank[UART2QueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}UART2_QUEUE;

/*************************************************************************
 Enums
 ************************************************************************/

/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void uart2_setup(void);
inline void uart2_begin(void);

/********************************************************
 *   Function Name: uart2_InitializeQueue(UART2_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart2_InitializeQueue(UART2_QUEUE* queue);

/********************************************************
 *   Function Name: uart2_addToQueue(UART2_QUEUE* queue, UART2_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart2_addToQueue(UART2_QUEUE* queue, UART2_NODE new_node);

/********************************************************
 *   Function Name: uart2_popNode(UART2_QUEUE* queue, UART2_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart2_popNode(UART2_QUEUE* queue, UART2_NODE* return_node);

 /********************************************************
 *   Function Name: uart2_SetNode( uint Byte1, uint Byte2, uint Byte3, uint Byte4 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart2_CreateNode( uint Byte1, uint Byte2, uint Byte3, uint Byte4 );

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 forward
 *
 *
 *********************************************************/
void Motor1_Forward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 backward
 *
 *
 *********************************************************/
void Motor1_Backward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 forward
 *
 *
 *********************************************************/
void Motor2_Forward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 backward
 *
 *
 *********************************************************/
void Motor2_Backward( uint address, uint speed );

#endif	/* MOTOR_UART_ISR_H */

