# Architect

Architect is a 64-bit Game engine for 2D games, still in its early stage.
For the time being OpenGl is supported, but in the future DirectX and Vulkan support will be added.
The main focus of this project is to provide an engine, API and editor for 2D games that is comprehensive and easy to handle 

# Feature Branch
It is not recommended to use the feature branch since it is a working branch with frequent changes which might not work or introduce bugs.

# Preprocessor statements used
_DEBUG<br />
_CONSOLE<br />
_CRT_SECURE_NO_WARNINGS<br />
GLEW_STATIC<br />
_USE_MATH_DEFINES<br />
GLM_FORCE_RADIANS<br />
PLATFORM_WINDOWS (only a very small piece of code relies on the windows header, which easily can be rewritten for MacOs and Linux)<br />

# Dependencies
The engine relies on the following liabrary dependencies and have to be placed in 
the specified folders (if you don't want to chnage dependency and library directories in the project settings).<br />
[library<br /> include folders<br /> library folder]<br /><br />
GLFW <br />&emsp;$(ProjectDir)/vendor/glfw/include/GLFW<br />&emsp;$(ProjectDir)/vendor/glfw/lib-vc2022<br />
GLEW <br />&emsp;$(ProjectDir)/vendor/glew/include/GL <br />&emsp;$(ProjectDir)/vendor/glew/lib/Release/x64<br />
glm  <br />&emsp;$(ProjectDir)/vendor/glm/glm<br />
ImGui <br /> &emsp;$(ProjectDir)/vendor/imgui<br />
OpenAl <br />&emsp;$(ProjectDir)/vendor/openal/include/AL <br />&emsp;$(ProjectDir)/vendor/openal/libs/Win64 <br />&emsp;$(ProjectDir)/vendor/openal/libs/Win64/EFX-Util_MT<br />
entt <br />&emsp;$(ProjectDir)/vendor/entt/include<br />
assimp<br />&emsp;$(ProjectDir)/vendor/assimp/include/assimp <br />&emsp;$(ProjectDir)/vendor/assimp/lib/x64<br />

Library file names<br />
opengl32.lib<br />
assimp.lib<br />
glew32s.lib<br />
glfw3.lib<br />
OpenAL32.lib<br />
EFX-Util.lib<br />

If you decide to link glew dynamicaly just remove the GLEW_STATIC preprocessor statement and replace glew32s.lib with glew32.lib

# Notes
Some assets that are harcoded by string are not available in the repository, thus the program will not compile.
Place your own assets in the corresponding folders in the src/assets directory and use them according to the
examples in the main.cpp file provided
Furthermore, the .wav files in the src/assets/audio directory might not work
