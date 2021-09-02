# RealTime FLuid Dynamics

A little pet project to acquire intuition of fluid dynamics and needed data structures for performant processing.

Only tested on Windows 10 x64 architecture.

## Introduction
Several littles projects to play with fluid dynamics, based on differents academic papers or, I hope so, novel concepts such as Entity Component System.


### Basic
The simpliest implementation based on  [Real Time Fluid Dynamics for Games](http://graphics.cs.cmu.edu/nsp/course/15-464/Spring11/papers/StamFluidforGames.pdf) paper published by Jos STAM in 2003.

source: [basic](src/basic)


## Dependencies

This project has only [xtensor](https://github.com/xtensor-stack/xtensor) and [SDL2](https://www.libsdl.org/download-2.0.php) as dependencies.

### Easy way
Please intall [vcpkg](https://github.com/microsoft/vcpkg), it is way more simple than anything else and portable.

#### Visual studio 2019
Edit **CMakeSettings.json** and change **cmakeToolchain** value to <vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake.

```ps
vcpkg install sdl2:x64-windows
vcpkg install xtensor:x64-windows
```

### Other ways

Please follow installation processes of each dependencies.

## Compiling

### Outside an IDE

If you have installed dependencies with [vcpkg](https://github.com/microsoft/vcpkg):

```ps
cmake -B out/build -S . -DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake
cmake --build out/build
```

Otherwise please refer to dependencies documentations.



### With Visual Studio 2019

Open the folder with Visual Studio.

Building: click on menu **Build / Build All**.

Running: select a startup item by clicking on drop down menu list near **Run** button and choose **basic.exe** for example and hit F5 or press **Run** button.
