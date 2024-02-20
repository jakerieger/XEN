# GLEngine

Just another game engine project based on OpenGL. Provides a `Core` static library that can
be linked against to build games and other 3D tools.

To see GLEngine in action, you can browse and build the projects provided in the [Examples](Examples) directory.

## Third Party Dependencies

See [DEPENDENCIES](DEPENDENCIES.md) for information regarding GLEngine's use of 3rd party libraries.

## Creating Projects

GLEngine provides a [Python script](Scripts/CreateNewProject.py) for generating a new project. Projects must be added
to [Projects.cmake](Projects.cmake)
and the GLEngine project reconfigured before the project can be compiled.

## License

GLEngine is licensed under the [ISC License](LICENSE). Third party files includes are all licensed
under their respective licenses.