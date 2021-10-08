#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

  printf(1, "Cantidad de tiempo reloj: %d\n", uptime());
  exit();
}
