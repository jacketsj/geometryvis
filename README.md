# Geometry Vis
This is a library/application for visualizing geometry algorithms.
Current in development.

![](screenshots/line_intersection.png?raw=true "The smaller circles were created with the line intersection algorithm.")

## Usage

There are different tools available.
Each can currently be selected by pressing the corresponding key:
- "Q": Draw a point.
- "W": Draw a line segment.
- "E": Draw a circle.
- "U": Find the convex hull of all points and draw it as line segments.
- "I": Find all the pairwise intersections of line segments and draw them as circles.
- "O": Find the convex hull of all circles and draw it as line segments.
- "`": Display the console history.

The default colour of new geometric objects can be changed with the number keys.

## Dependencies
This repo relies on a [modified version of SIGIL](https://github.com/jacketsj/sigil) (Sound, Input, and Graphics Integration Library).
The modified version was developed in tandem with Geometry Vis.
The original version of SIGIL is available [here](http://www.libsigil.com/).

The installation instructions for SIGIL are quite outdated, so use these steps:
1. Install OpenAL. On Ubuntu (18.04), the following should suffice:
```
sudo apt install libalut-dev
```
2. Clone the [modified SIGIL source code](https://github.com/jacketsj/sigil). This should also be possible with a simple `git submodule update --init --recursive`.
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
