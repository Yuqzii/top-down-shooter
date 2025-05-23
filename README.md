# Top-Down Shooter
(Temporary title)\
A top-down shooter written in C++ using SDL2 for graphics rendering.\
All feedback is very welcome :)

## Playing the game
There are precompiled binaries for the latest release that you can run. You can also [compile it yourself](#build) if you want.\
If you encounter any issues with the precompiled version I recommend trying to compile it. I would also greatly appreciate it if you open an issue if you encounter any issues.

## Build
This project uses CMake, so you can build it using whatever way you prefer with CMake.\
I build it using these commands from the project root:
```
mkdir build
cd build
cmake ..
cmake --build
```
If on Linux/Mac this should create an executable called "game". Simply run this to run the game.\
On windows the build process might be a bit different, but I have tested building it with Visual Studio which should work fine.

## Feature highlights
- KD-Tree structure for fast queries about object location.
- Destructible terrain.
- Automatic collision generation for terrain.
- Very simple UI system (only displays healthbars currently).
