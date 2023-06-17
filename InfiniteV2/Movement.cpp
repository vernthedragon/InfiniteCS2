#include "Features.h"

//BROKEN STRAFE
void Movement::DoStrafe() {
	static auto StrafeDirection = 1.f;
	StrafeDirection = -StrafeDirection;

	auto velocity = Client->cache.Velocity;

	Vec3 wish_angle = Client->cmd->Base->view->angles;

	auto speed = velocity.Length2D();
	auto ideal_strafe = std::clamp(RAD2DEGF(atan(15.f / speed)), 0.f, 90.f);

	Client->cmd->Base->forwardmove = 0.f;


	static float old_yaw = 0.f;
	auto yaw_delta = std::remainderf(wish_angle.y - old_yaw, 360.f);
	auto abs_yaw_delta = abs(yaw_delta);
	old_yaw = wish_angle.y;



	if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f)
	{
		velocity.ToAngles();
		auto velocity_delta = std::remainderf(wish_angle.y - velocity.y, 360.0f);
		auto retrack = std::clamp(RAD2DEGF(atan(30.f / speed)), 0.f, 90.f) * 2.f;
		if (velocity_delta <= retrack || speed <= 15.f)
		{
			if (-retrack <= velocity_delta || speed <= 15.0f)
			{
				wish_angle.y += StrafeDirection * ideal_strafe;
				Client->cmd->Base->sidemove = StrafeDirection;
			}
			else
			{
				wish_angle.y = velocity.y - retrack;
				Client->cmd->Base->sidemove = 1.f;
			}
		}
		else
		{
			wish_angle.y = velocity.y + retrack;
			Client->cmd->Base->sidemove = -1.f;
		}


	}
	else if (yaw_delta > 0.f)
		Client->cmd->Base->sidemove = -1.f;
	else
		Client->cmd->Base->sidemove = 1.f;
}

void Movement::DoBunnyhop() {

	if (!Config->Movement.Bunnyhop)
		return;

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
	auto movetype = Client->local->m_MoveType();
	if (movetype == MoveType_t::movetype_noclip || movetype == MoveType_t::movetype_ladder)
		return;

	if (/*Client->cmd->Buttons.Button1 & IN_SPEED  IN_SPEED doesnt exist*/ Client->cache.VelocityLength2D < 1.f)
		return; 

	if (OnGround)
		return;

	Movement::DoStrafe();
}