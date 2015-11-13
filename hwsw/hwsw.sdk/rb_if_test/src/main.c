/* Simple DMA example */

#include <stdio.h>
#include <stdlib.h>
#include <sleep.h>
#include <time.h>

#include "platform.h"
#include "xparameters.h"
#include "xuartps_hw.h"
#include "xtime_l.h"
#include "xil_cache.h"



double XTime_ToDouble(XTime * t)
{
   return ((double) (*t)) / COUNTS_PER_SECOND;
}

double XTime_GetDiff_Double(XTime * oldtime, XTime * newtime)
{
   return ((double) (*newtime - *oldtime)) / COUNTS_PER_SECOND;
}

double XTime_DiffCurrReal_Double(XTime * oldtime)
{
   XTime newtime;
   XTime_GetTime(&newtime);
   return XTime_GetDiff_Double(oldtime, &newtime);
}

/* IP base address */
volatile unsigned *myip_regs = (void *) 0x43C00000;

void myip_print_regs(void)
{
   for (unsigned i = 0; i < 8; i++)
      printf(" %08x", myip_regs[i]);
   fflush(stdout);
   print("\n");
}

int main(void)
{
   init_platform();
   sleep(3);
   print("Hello World\r\n");
   print("Waiting for 3s to let user connect UART...");
   sleep(3);
   print(" done\r\n");

   print("Explicitely enabling UART Rx and Tx, no parity, no interrupts...\r\n");
   // Explicitely enable Rx ad Tx
   XUartPs_WriteReg(XPAR_PS7_UART_1_BASEADDR, XUARTPS_CR_OFFSET, XUARTPS_CR_RX_EN | XUARTPS_CR_TX_EN);
   // No parity
   XUartPs_WriteReg(XPAR_PS7_UART_1_BASEADDR, XUARTPS_MR_OFFSET, XUARTPS_MR_CHMODE_NORM | XUARTPS_MR_PARITY_NONE);
   // Disable interrupts
   XUartPs_WriteReg(XPAR_PS7_UART_1_BASEADDR, XUARTPS_IDR_OFFSET, XUARTPS_IXR_MASK);

   extern unsigned int _end;
   printf("Adresse de fin de la ddr utilisée par le ldscript 0x%08x", (unsigned int)&_end);
   fflush(stdout);
   print("\r\n");

#define ARRAY_SIZE ((1<<10) * 16)
   // Try to access memory
   volatile unsigned int *tx_addr = (volatile unsigned int *)0x43C00000;
   volatile unsigned *arr = &_end;
   unsigned total;

   printf("Array to be filled: addr=%p, sz in words=%d\r\n", arr, ARRAY_SIZE);

   /* Configure TX */
   *(tx_addr + 11) = (unsigned int)arr;
   *(tx_addr + 13) = ARRAY_SIZE/16;

   printf("Configuration: tx_addr %08x, wbs %d\n\r", *(tx_addr + 11), *(tx_addr + 13));

   *tx_addr = 0; // Trigger the start of the transfert

   //printf("Master FSM STATE 0x%02x\r\n", (*(tx_addr + 1))>>24);
   //printf("Master Error     0x%02x\r\n", (*(tx_addr + 1))&0x3);

   /* Busy waiting for the transfert to be done */
   unsigned int r;
   while ((r = *(tx_addr +  0)) == 1);

   print("Transfer is over, checking integrity, ...\r\n");

   total = 0;
   for (unsigned i = 0; i < ARRAY_SIZE; i++) {
      //printf("Index %d -> value %d\r\n", i, arr[i]);
      total += arr[i];
   }
   printf("Total: %u\n\r", total);

   print("Finish: making leds blink...\r\n");
   unsigned *ptrled = (void *) XPAR_AXI_GPIO_0_BASEADDR;
   unsigned sh = 0;
   int dir = 1;
   *ptrled = 0x01 << sh;
   do {
      usleep(500000);
      if ((sh == 0 && dir < 0) || (sh == 3 && dir > 0))
         dir = -dir;
      sh += dir;
      *ptrled = 0x01 << sh;
      //print(".");
   } while (1);

   cleanup_platform();
   return 0;
}
