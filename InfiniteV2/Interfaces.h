#pragma once

#include "IClient.h"
#include "IInput.h"
#include "IEngineClient.h"
#include "IEntList.h"
#include "IInputSystem.h"
#include "IRenderer.h"
#include "ISchemaSystem.h"
#include "ITrace.h"
#include "CSGlobalVars.h"
#include "ICvar.h"
#include "IVGuiPaintSurface.h"

extern IClient* g_Client;
extern IInput* g_Input;
extern IEngineClient* g_Engine;
extern IEntList* g_EntList;
extern IInputSystem* g_InputSystem;
extern IRenderer* g_Renderer;
extern ISchemaSystem* g_SchemaSystem;
extern ITrace* g_Trace;
extern CSGlobalVars* g_GlobalVars;
extern ICvar* g_Cvar;
extern IVGuiPaintSurface* g_PaintSurface;

bool InitializeCSInterfaces();
