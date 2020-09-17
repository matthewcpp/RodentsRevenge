# RodentsRevenge
Clone of the classic 1991 game written in C89

### Controls
The game can be controlled using a Controller or the keyboard

```
Start / Space: Start or Pause Game
D-Pad / Arrow Keys: Move the mouse
```

### Building

Begin by cloning the repo and initializing the submodules:
```shell script
git clone --recurse-submodules https://github.com/matthewcpp/RodentsRevenge.git
```
##### Windows:
```shell script
mkdir build && cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install .. --build sdl2_ttf --build sdl2_image
cmake ..
```
##### Ubuntu Linux:
```shell script
sudo apt-get -y install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

##### MacOS:
```shell script
brew install sdl2 sdl2_image sdl2_ttf
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

##### Xbox:

Clone the [nxdk](https://github.com/XboxDev/nxdk) repo from github and follow the instructions for configuring the SDK.
Note that on Ubuntu 20.04 you may need to append to your path directory for the `llvm-lib` command to work during the build process:
```
PATH=$PATH:/usr/lib/llvm-10/bin
```
Generate your xbe
```shell script
make -f Makefile.xbox NXDK_DIR=/path/to/nxdk
```