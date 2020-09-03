# RodentsRevenge
Clone of the classic 1991 game written in C89

### Building
Windows and MacOS:
```shell script
mkdir build && cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build missing
cmake ..
```
Ubuntu Linux:
```shell script
sudo apt-get -y install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```