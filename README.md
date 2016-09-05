
# Install

Need to install MySQL C connector:
https://dev.mysql.com/downloads/connector/c/
(link: https://openclassrooms.com/courses/utiliser-l-api-mysql-dans-vos-programmes)

## When compile, library mysql (dylib file) marked as "iamge not found"

Execute this command line:
sudo install_name_tool -id /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib/libmysqlclient.dylib /usr/local/mysql-connector-c-6.1.6-osx10.8-x86_64/lib/libmysqlclient.dylib

Note: Can be usefull to create alias to /usr/local/mysql
(link: https://bugs.mysql.com/bug.php?id=61243)

## Check dependencies

Command: otool -L /Users/thibault/Library/Caches/CLion12/cmake/generated/c8c24237/c8c24237/Debug/homecenter_server

