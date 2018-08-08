
```
g++ -luv src/server.cc src/io/libuv.cc src/utils/timeutils.cc src/io/response.cc -o server.o

./server.o

node ./net_client.js
```