#pragma once

//GENERAL
#define INFINITE_SHOW_CONSOLE //shows console debugging

//RENDERING
//#define MULTITHREAD_RENDER //allows for "RenderBranches" out of the main render thread //REMOVED FEATURE

//OPTIMIZATION
#define EXTERNALLY_LINK_CS_FUNCTIONS //externally links cs function based on memory pointer (results in slightly faster function if the cs function pointer is non changing
