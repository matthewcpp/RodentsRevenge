# RodentsRevenge
Clone of the classic 1991 game written in C89

### Building
```shell script
mkdir build && cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build missing
cmake ..
```