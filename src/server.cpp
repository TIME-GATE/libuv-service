#include "./io/libuv.h"

int main(int argc, char ** argv){
  
  printf("start service ...");
  IIOService server;
  server.listen("0.0.0.0", 7000);

}
