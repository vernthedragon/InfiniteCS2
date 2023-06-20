#include "Hooks.h"
#include "Features.h"


void __fastcall Hooks::OnAddEntity(IEntList* This, IEntityInstance* Instance, CHandle Handle) {
	EntListVMTHook->GetOriginal(oOnAddEntity, EntListVTable::ONADDENTITY);

	oOnAddEntity(This, Instance, Handle);

	if (Instance)
		GameHandler->AddEntity(Instance, Handle);
}


void __fastcall Hooks::OnRemoveEntity(IEntList* This, IEntityInstance* Instance, CHandle Handle) {
	EntListVMTHook->GetOriginal(oOnRemoveEntity, EntListVTable::ONREMOVEENTITY);

	if (Instance)
		GameHandler->RemoveEntity(Instance, Handle);

	oOnRemoveEntity(This, Instance, Handle);
}