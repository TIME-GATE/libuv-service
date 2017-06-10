#include <cassert>

#include "libuv.h"

int IIOService::listen(const char *host, int port) {
	printf("######## start ftp service HOST: %s PORT: %d ################### \n", host, port);  
	
	loop = uv_default_loop(); 
  uv_tcp_t server;  
  uv_tcp_init(loop, &server);  
  uv_ip4_addr(host, port, &addr); 
  uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
  
	int listen_status = uv_listen((uv_stream_t*) &server, default_backlog, IIOService::onConnect);   
	if (listen_status) {   
  	fprintf(stderr, "Listen error %s\n", uv_strerror(listen_status));  
  	return 1;  
  }   
  
	return uv_run(IIOService::loop, UV_RUN_DEFAULT);

}

void IIOService::send(uv_stream_t *client, ssize_t nread, const uv_buf_t *buff) {
  if(nread < 0) {
    if(nread != UV_EOF) fprintf(stderr, "read error %s \n", uv_err_name(nread));
    uv_close((uv_handle_t*) client, NULL);
  }

  printf("echo_read:%s\r\n", buff->base);
  if(buff->base) free(buff->base);

}

void IIOService::onConnect(uv_stream_t *server, int status) {
	if (status < 0) {  
  	fprintf(stderr, "one New connection error %s\n", uv_strerror(status));  
    return;  
  }  
      
  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));  
  uv_tcp_init(IIOService::loop, client);  
  
	if (uv_accept(server, (uv_stream_t*) client) == 0) {  
  	uv_read_start((uv_stream_t*) client, IIOService::read, IIOService::send);  
  } else {  
  	uv_close((uv_handle_t*) client, NULL);  
  }  
  
	printf("one new connection have been OK, status:%d\r\n", status);   

}

void IIOService::read(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char*) malloc(suggested_size);  
  buf->len = suggested_size; 
}
