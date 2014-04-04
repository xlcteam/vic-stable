
#include <vic.h>


void setup ()
{
  vic_init(115200);
  Serial.begin(115200);

}

void loop ()
{
  if(Serial.available()){
    vic_process(Serial.read());
  }

}
