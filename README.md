# RealTime FLuid Dynamics

A little pet project to acquire intuition of fluid dynamics and needed data structures for performant processing.

## Introduction
Several littles projects to play with fluid dynamics, based on differents academic papers or, I hope so, novel concepts such as Entity Component System.


### Basic
The simpliest implementation based on  [Real Time Fluid Dynamics for Games](http://graphics.cs.cmu.edu/nsp/course/15-464/Spring11/papers/StamFluidforGames.pdf) paper published by Jos STAM in 2003.

source: [basic](src/basic)




## Dependencies

This project has only [xtensor](https://github.com/xtensor-stack/xtensor) and [SDL2](https://www.libsdl.org/download-2.0.php) as dependencies.

### Easy way
Please intall [vcpkg](https://github.com/microsoft/vcpkg), it is way more simple than anything else and portable.
<!--  -->
Edit **CMakeSettings.json** and change **cmakeToolchain** value to <vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake.

```ps
vcpkg install SDL2::x64-windows
vcpkg install xtensor::x64-windows
```

### Other ways

Please follow installation processes of each dependencies.


