#include "cli_task.h"

void vTaskVTYusb(void *cliStatePtr)
{
  CmdState_t *state = (CmdState_t *)(cliStatePtr);
  fprintf_P(state->myStdInOut, PSTR("Restart\r\n"));
  cmdlineInputFunc('\r', state);
  
  char znak;
  for( ;; )
  {
    if( xQueueReceive(xVtyRec, &znak, portMAX_DELAY))
    {
      cmdlineInputFunc((char)znak, state);
      cmdlineMainLoop(state);
    }  
  }
}

void vTaskVTYsocket(void *cliStatePtr)
{
  CmdState_t *state = (CmdState_t *)(cliStatePtr);
  
  char znak;
  for( ;; )
  {
    znak = 0;
    znak = fgetc(state->myStdInOut);
    cmdlineInputFunc((char)znak, state);
    cmdlineMainLoop(state);
  }
}
