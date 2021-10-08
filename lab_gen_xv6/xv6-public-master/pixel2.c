#include "types.h"
#include "stat.h"
#include "user.h"


int
main(int argc, char *argv[])
{
  modeswitch(1);
  
  int j=0,i=0;
  for(j=0;j<320;++j)
    for(i=0; i<200; ++i)
      plotpixel(j,i,0);


for (int k = 0; k<100;++k){
  int j=0,i=0;

  for(j=145;j<185;++j)
    for(i=75; i<125; ++i)
      plotpixel(j+k,i+k,13);

  for(j=150;j<180;++j)
    for(i=80; i<120; ++i)
      plotpixel(j+k,i+k,7);
}
  exit();
}
