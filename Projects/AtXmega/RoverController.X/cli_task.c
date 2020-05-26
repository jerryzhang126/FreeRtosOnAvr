#include "cli_task.h"

#include "adxl345.h"
#include "bmp085.h"
#include "mhc5883l.h"
#include "hardware.h"

extern HardwarePAL_t hardwarePAL;

void vTaskVTYusb(void *cliStatePtr)
{
  CliState_t *state = (CliState_t *)(cliStatePtr);

#if USE_XC8
  CMD_msg("Restart XC 8 compiller\r\n");
#else
  CMD_msg("Restart XC 8 AvrGss compiller\r\n");  
#endif
  //adxl345_init(&hardwarePAL.adxl, &hardwarePAL.twiSensors, ADXL345_RANGE_16G, state->myStdInOut);
  
  //bmp085_init(&hardwarePAL.bmp, &hardwarePAL.twiSensors,  state->myStdInOut, BMP085_ULTRAHIGHRES);

  //mhc5883l_init(&hardwarePAL.mhc, &hardwarePAL.twiSensors,  state->myStdInOut);
  
  cmdlineInputFunc('\r', state);

  char recSymbol;
  for( ;; )
  {
    if( xQueueReceive(xVtyRec, &recSymbol, portMAX_DELAY))
    {
      cmdlineInputFunc((char)recSymbol, state);
      cliMainLoop(state);
    }
  }
}

