#pragma once

//NOTES
// COMMENT UNSAFE -> function / section could possible crash
// COMMENT WARNING -> function / section possibly can cause issues
// COMMENT BROKEN -> function / section not working as intended

//WARNING!
// _CRT_SECURE_NO_WARNINGS IS ENABLED (DEPRECATED FUNCTION WILL NOT CAUSE ERRORS)
// 
//GENERAL
#define INFINITE_SHOW_ERRORS //we show exceptions and errors
#define INFINITE_SHOW_CONSOLE //shows console debugging

//RENDERING
//#define MULTITHREAD_RENDER //allows for "RenderBranches" out of the main render thread //REMOVED FEATURE

//OPTIMIZATION
#define EXTERNALLY_LINK_CS_FUNCTIONS //externally links cs function based on memory pointer (results in slightly faster function if the cs function pointer is non changing
