/**
 * @file       cbuffer.c
 * @copyright  
 * @license    
 * @version    
 * @date       2024-06-26
 * @author     Phat Nguyen
 *             
 * @brief      Circular Buffer
 *             This Circular Buffer is safe to use in IRQ with single reader,
 *             single writer. No need to disable any IRQ.
 *
 *             Capacity = <size> - 1
 * @note          
 * @example    cbuffer_t cb;
 *             uint8_t cb_buff[6];
 *             uint8_t a;
 *             void main(void)
 *             {
 *                 cb_init(&cb, cb_buff, 100);
 *                 cb_clear(&cb);
 *                 char a[] = {0, 1, 2, 3, 4};
 * 
 *                 int c = 5;
 *                 cb_write(&cb, a, 5);
 *                 char b[5];
 *                 cb_read(&cb, a, 5);
 *             }
 */

/* Includes ----------------------------------------------------------- */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cbuffer.h"

/* Private defines ---------------------------------------------------- */
#define CB_MAX_SIZE (0x00800000)

/* Private enumerate/structure ---------------------------------------- */
/*
typedef struct
{
    uint8_t *data;
    uint32_t size;
    uint32_t writer;
    uint32_t reader;
    uint32_t overflow;
    bool active;
} cbuffer_t;
*/

/* Private macros ----------------------------------------------------- */
/**
 * @brief  <macro description>
 *
 * @param[in]     <param_name>  <param_despcription>
 * @param[out]    <param_name>  <param_despcription>
 * @param[inout]  <param_name>  <param_despcription>
 *
 * @attention  <API attention note>
 *
 * @return  
 *  - 0: Success
 *  - 1: Error
 */
//#define PRIVATE_MACRO(a)  do_something_with(a)

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Public function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void cb_init(cbuffer_t *cb, void *buf, uint32_t size) 
{
    cb->data = (uint8_t *)buf;
    if (size <= CB_MAX_SIZE)
        cb->size = size;
    else 
        cb->size = CB_MAX_SIZE;
    cb->writer = 0;
    cb->reader = 0;
    cb->overflow = 0;
    cb->active = true;
}

void cb_clear(cbuffer_t *cb) 
{
    cb->writer = 0;
    cb->reader = 0;
    cb->overflow = 0;
}

uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes) 
{
    if (!cb->active) 
        return 1;

    uint32_t bytes_read = 0;
    for (uint32_t i = 0; i < nbytes; i++) 
    {
        if (cb->reader == cb->writer) 
        {
            printf("\nEmpty\n");
            break; // Empty 
        }
        ((uint8_t *)buf)[i] = cb->data[cb->reader];
        cb->data[cb->reader] = 0;
        cb->reader = (cb->reader + 1) % cb->size;
        bytes_read = bytes_read + 1;
    }
    return bytes_read;
}

uint32_t cb_write(cbuffer_t *cb, void *buf, uint32_t nbytes) 
{
    if (!cb->active) 
        return 1;

    uint32_t bytes_written = 0;
    for (uint32_t i = 0; i < nbytes; i++) 
    {
        uint32_t next = (cb->writer + 1) % cb->size;
        if (next == cb->reader) 
        {
            printf("\nOverflow\n");
            cb->overflow++;
            break;
        }
        cb->data[cb->writer] = ((uint8_t *)buf)[i];
        cb->writer = next;
        bytes_written = bytes_written + 1;
    }
    return bytes_written;
}

/* ----------------------------------------------- */


/* End of file -------------------------------------------------------- */
