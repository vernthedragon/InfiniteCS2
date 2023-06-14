#include "Features.h"

void Movement::DoBunnyhop() {

	static bool bLastJumped = false;
	static bool bShouldFake = false;
	bool OnGround = Client->local->m_fFlags() & FL_ONGROUND;
	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		Client->cmd->Buttons.Button1 |= IN_JUMP;
	}
	else if (Client->cmd->Buttons.Button1 & IN_JUMP)
	{
		if (OnGround)
		{
			bShouldFake = bLastJumped = true;
		}
		else
		{
			Client->cmd->Buttons.Button1 &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
	else
	{
		bShouldFake = bLastJumped = false;
	}


}