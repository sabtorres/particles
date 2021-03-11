# particles
Real time particle system using OpenGL 4 and C++

![Example 1](showcase/particle_example1.webp)
 
my goal is to build a particle simulation system that can be customizable by artists and designers, and easily extended by a graphics programmer. this is free software, without warranties.

# features

- GPU compute support for particle dynamics

- user is able to put a custom texture

- able to customize particle size, explosiveness, emission radius, initial velocity, acceleration, cycle time and randomness factors

- able to simulate up to 9999 particles (tested on a Ryzen 7 2700 GPU, 3.2 Ghz, and an AMD RX 460 GPU, with no notable variation on seconds per frame. notice that very large point sizes can cause delay due to some reason, i may attempt to fix this in the future)

# to-do list

- camera input and grid for better 3D view

# maybe to-dos (these features are lesser priority and may or may not go to the list above)

- support for multiple textures

- SPIR-V shaders (requires updating to GL 4.6)

- Bezier curves to determine particle path

# requirements for building

- opengl 4.5+ compatible device

- glm (>= 0.9.9)

- glfw3

- libdl

- meson (use a reasonably recent version)

- ninja (or some other meson backend, but i'm only using ninja)

- some C++ compiler (stdc++11 support)

# how to build

- clone this repository and go to the directory;

- enter the command **meson setup build**;

- go to the created directory **cd build**;

- enter the command **ninja**. if everything works you'll have a **demo** executable in the build directory.

# usage

just run **demo** (from the build directory). there will be a GUI in the display so that you can set the particle parameters to whatever you like.

# contributing

i'm not accepting PRs right now, but maybe i will in the future.

open an issue if you have a problem, question or suggestion.

want me to work on this? give it a motivational star.

# special thanks

these libraries were used in the development and i'd like to thank their authors and contributors for offering these tools:

- imgui

- imgui file dialog (by aiekick)

- glad

- lodepng

- glm

- glfw
