
# Install

## CMake

apt-get install cmake
Latest version (3.x): http://osdevlab.blogspot.fr/2015/12/how-to-install-latest-cmake-for.html
(long time needed)

## MySQL C Connector

Need to install MySQL C connector:
https://dev.mysql.com/downloads/connector/c/
(link: https://openclassrooms.com/courses/utiliser-l-api-mysql-dans-vos-programmes)
Linux: apt-get install libmysqlclient15-dev

## RF24 install

```
cd ./vendors/common/vendors/RF24
./configure
cmake .
make
sudo make install
```

# Support

## When compile, library mysql (dylib file) marked as "image not found"

Execute this command line:
sudo install_name_tool -id /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib/libmysqlclient.dylib /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib/libmysqlclient.dylib

Note: Can be usefull to create alias to /usr/local/mysql
(link: https://bugs.mysql.com/bug.php?id=61243)

## Check dependencies

Command: otool -L /Users/thibault/Library/Caches/CLion12/cmake/generated/c8c24237/c8c24237/Debug/homecenter_server

## Error 1

```
/usr/bin/ld: /usr/lib/arm-linux-gnueabihf//libmysqlclient.a(my_thr_init.c.o): undefined reference to symbol 'pthread_getspecific@@GLIBC_2.4'
//lib/arm-linux-gnueabihf/libpthread.so.0: error adding symbols: DSO missing from command line
```

Solution:

```
sudo ldconfig
```


# Build

```
#!/bin/bash

cd
sudo rm -Rf ~/bin
mkdir ~/bin
cd bin
sudo make ../homecenter-server/vendors/common/vendors/RF24
sudo cmake ../homecenter-server
sudo make
cd ../
sudo bin/homecenter_server
```