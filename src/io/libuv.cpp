#include <cassert>
#include <uv.h>

#include "response.h"
#include "libuv.h"
#include "string.h"

using namespace std;

uv_loop_t * Response::loop = uv_default_loop();
uv_tcp_t Response::server;
struct sockaddr_in Response::addr;

int IIOService::listen(const char * host, int port) {
	
  printf("######## start futures service HOST: %s PORT: %d ######### \n", host, port);  

  uv_tcp_init(Response::loop, &Response::server);
  uv_ip4_addr(host, port, &Response::addr);
  uv_tcp_bind(&Response::server, (const struct sockaddr*)&Response::addr, 0);
  
	int listen_status = uv_listen((uv_stream_t*)&Response::server, Response::default_backlog, Response::onConnect);   
	
  if(listen_status) {   
  	fprintf(stderr, "Listen error %s\n", uv_strerror(listen_status));
  	return 1;
  } 
  
	return uv_run(Response::loop, UV_RUN_DEFAULT);
}

int IIOService::connect(const char * host, uint16_t port, int32_t seconds) {
  // TODO: 
	return 0;
}

void IIOService::shutdown() {
  // TODO
}

