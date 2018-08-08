###  项目依赖安装 LINUX环境

#### 1 LIBUV

* unix
```text
sudo apt-get install automake

sudo apt-get install libtool

git clone https://github.com/libuv/libuv.git && cd libuv

sh autogen.sh

./configure

make && make check

sudo make install

```

* OS

```
brew install --HEAD libuv

g++ -luv server.cpp
```

#### 2 PROTOBUFF

```text

git clone https://github.com/google/protobuf.git

cd protobuf && ./configure

make && make check

sudo make install && sudo ldconfig

```

#### 3 MYSQL

```text
略
```

#### 4 LEVELDB

```text
略
```

#### 5 test

```
g++ -luv src/server.cpp src/io/libuv.cpp src/utils/timeutils.cc src/io/response.cpp -o server.o

./server.o
```
