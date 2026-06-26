This is a fork of [CleanQuake](https://github.com/klaussilveira/clean-quake).

(INDEV)

Primary Goal:
To modernize the Quake engine by aggressively stripping away legacy cruft and updating the codebase to a highly readable foundation with modern C++ language features and stricter compile-time type checking.

Secondary Goal:
To upgrade the engine's core dependencies from SDL2 to SDL3, replacing the software renderer with a modern, cross-platform hardware-accelerated graphics pipeline using SDL_GPU. This ensures native support for modern graphics APIs (Vulkan, Metal, Direct3D) without massively increasing codebase bloat.
