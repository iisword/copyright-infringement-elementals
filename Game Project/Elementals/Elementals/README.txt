----------Elementals Compiler Setup and Notes----------

In order to setup the compiler if the there is a problem:

<Create server builds>
1. Open configuration manager
2. Create a copy of the Debug config and save it as ServerDebug
3. Create a copy of the Release config and save it as ServerRelease
4. Open the project properties
5. Go to C++/C->Preprocessor
6. Add "SERVER_BUILD" to the Preprocessor Definitions of ServerDebug and ServerRelease config

<Includes>
Under VC++ Directories in the project properties:
Add the "Third Party\include" to the include directories(suggested: $(SolutionDir)Third Party\include)
Add the "Third Party\lib" to the library directories(suggested: $(SolutionDir)Third Party\lib)

<Linker Input>
Under Linker->Input in the project properties:
Add "ws2_32.lib" to all builds.
Add "Raknet_debug.lib" to all debug builds.
Add "Raknet.lib" to all release builds.
Add "fmodex_vc.lib" to all client builds.

<Starting multiple builds>
Go to Build->Batch Build
Check all configs you want to build
Hit Build/Rebuild/Clean

<Separating client and server code>
The "SERVER_BUILD" preprocessor definition is used to help within the code.
By using #ifdef/#ifndef SERVER_BUILD, the project will ignore parts of the code that is specific to a build.
Whole files can be removed from the build by right-clicking the file, clicking Properties, then excluding it from the build.