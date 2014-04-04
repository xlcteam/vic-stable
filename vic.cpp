

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #if defined(ARDUINO)
    #include "WProgram.h"
  #endif
#endif

#include "src/vic.c"
#include "src/vic-funcs.c"
#include "src/vic-serial.c"
#include "src/vic-tasks.c"
#include "src/vic-var.c"

