# Geometry Vis
This is a library/application for visualizing geometry.

## Dependencies
This repo relies on [SIGIL](https://libsigil.com) (Sound, Input, and Graphics Integration Library),
and [GLFW3](https://github.com/glfw/glfw).
The installation instructions for SIGIL are quite outdated, so try these steps:
1. Install OpenAL. On Ubuntu (18.04), the following should suffice:
```
sudo apt install libalut-dev
```
2. Clone the [SIGIL source code](https://gitlab.com/geoff-nagy/sigil/tree/master). This should also be possible with a simple `git submodule update`.
3. Build SIGIL according loosely according to the instructions from [this thread](https://openeuphoria.org/forum/133840.wc) (saved in wayback machine april 13 2020), which are as follows:
```
sudo apt install libfreetype6-dev libglew-dev libxcursor-dev libxi-dev libxinerama-dev libxrandr-dev 
git clone https://gitlab.com/geoff-nagy/sigil.git sigil 
cd sigil/deps/ 
git clone https://github.com/glfw/glfw.git glfw 
mkdir ../build-linux-gcc 
cd ..
cmake .
make 
sudo make install
```
3. Now also install GLFW:
```
cd sigil/deps/glfw/
cmake .
make 
sudo make install
```
3. To setup the shared library for the first time (actually allowing programs to run):
```
echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```