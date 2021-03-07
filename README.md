# particles
 Real time particle system using OpenGL 4 and C++
 
my goal is to build a particle simulation system that can be customizable by artists and designers, and easily extended by a graphics programmer. this is free software, without warranties.

# to-do list

- GPU particle support

- particle explosiveness factor

- GUI to change the particle texture

# maybe to-dos (these features are lesser priority and may or may not go to the list above)

- SPIR-V shaders (requires updating to GL 4.6)

- Bezier curves to determine particle path

# requirements for building

- opengl 4.5+ compatible device

- glm (>= 0.9.9)

- glfw3

- libdl

- meson (use a reasonably recent version)

- ninja (or some other meson backend, but i'm only using ninja)

- some C++ compiler

# how to build

- clone this repository and go to the directory;

- enter the command **meson setup build**;

- go to the created directory **cd build**;

- enter the command **ninja**. if everything works you'll have a **demo** executable in the build directory.

# usage

just run **demo** (from the build directory). there will be a GUI in the display so that you can set the particle parameters to whatever you like.

# contributing

i'm not accepting contributions right now, but maybe i will in the future.

open an issue if you have a problem, question or suggestion.

want me to work on this? give it a motivational star.

# special thanks

these libraries were used in the development and i'd like to thank their authors and contributors for offering these tools:

- imgui

- glad

- lodepng

- glm

- glfw
