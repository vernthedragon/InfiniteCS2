#include "Interfaces.h"
#include "Client.h"
IClient* g_Client = nullptr;
IInput* g_Input = nullptr;
IEngineClient* g_Engine = nullptr;
IEntList* g_EntList = nullptr;
IInputSystem* g_InputSystem = nullptr;
IRenderer* g_Renderer = nullptr;
ISchemaSystem* g_SchemaSystem = nullptr;
ITrace* g_Trace = nullptr;
CSGlobalVars* g_GlobalVars = nullptr;
ICvar* g_Cvar = nullptr;
IVGuiPaintSurface* g_PaintSurface = nullptr;

template<typename T>
static T* GetGameInterface(const char* module_name, const char* interface_name)
{

	Client->Log("Setting up Interface: ");
	Client->Log(interface_name);
	Client->Log("\n");
	if (!module_name || !interface_name)
	{
		return nullptr;
	}

	HMODULE module = GetModuleHandleA(module_name);

	if (!module)
	{
		return nullptr;
	}

	std::uint8_t* create_interface = reinterpret_cast<std::uint8_t*>(GetProcAddress(module, "CreateInterface"));

	if (!create_interface)
	{
		return nullptr;
	}

	using interface_callback_fn = void* (__cdecl*)();

	typedef struct _interface_reg_t
	{
		interface_callback_fn callback;
		const char* name;
		_interface_reg_t* flink;
	} interface_reg_t;

	interface_reg_t* interface_list = *reinterpret_cast<interface_reg_t**>(Client->ResolveRIP(create_interface, 3, 7));

	if (!interface_list)
	{
		return nullptr;
	}

	for (interface_reg_t* it = interface_list; it; it = it->flink)
	{
		if (!strcmp(it->name, interface_name))
		{
			return reinterpret_cast<T*>(it->callback());
		}
	}

	return nullptr;
}

bool InitializeCSInterfaces() {
	g_Client = GetGameInterface<IClient>(("client.dll"), "Source2Client002");
	g_Input = *reinterpret_cast<IInput**>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), CSGO_INPUT), 3, 7));
	g_Engine = GetGameInterface<IEngineClient>(("engine2.dll"), "Source2EngineToClient001");
	g_EntList = *reinterpret_cast<IEntList**>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), ENTITY_LIST), 3, 7));
	g_InputSystem = GetGameInterface<IInputSystem>(("inputsystem.dll"), "InputSystemVersion001");
	g_Renderer = **reinterpret_cast<IRenderer***>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("rendersystemdx11.dll"), RENDERER), 4, 8));
	g_SchemaSystem = GetGameInterface<ISchemaSystem>(("schemasystem.dll"), "SchemaSystem_001");
	g_Trace = *reinterpret_cast<ITrace**>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), TRACE_MANAGER), 3, 7));
	g_GlobalVars = *reinterpret_cast<CSGlobalVars**>(Client->ResolveRIP(Client->FindPattern(GetModuleHandleA("client.dll"), GLOBAL_VARS), 3, 7));
	g_Cvar = GetGameInterface<ICvar>(("tier0.dll"), "VEngineCvar007");
	g_PaintSurface = GetGameInterface<IVGuiPaintSurface>("vgui2.dll", "VGUI_Panel010"); //ERROR, can't get the handle on vgui2.dll
	
	return true;
}