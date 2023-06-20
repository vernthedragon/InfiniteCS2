#include "Features.h"

//BROKEN STRAFE
void Movement::DoStrafe(UserCmd* cmd) {

	static float yaw_add = 0.f;
	
	//	static const auto cl_forwardspeed = interfaces.cvars->FindVar("cl_forwardspeed");
	//	cl_sidespeed->SetValue(450 + 100);
	//	cl_forwardspeed->SetValue(450 + 100);
	static auto old_yaw = 0.0f;

	auto get_velocity_degree = [](float velocity)
	{
		auto tmp = RAD2DEGF(atan(30.0f / velocity));

		if (CheckIfNonValidNumber(tmp) || tmp > 90.0f)
			return 90.0f;

		else if (tmp < 0.0f)
			return 0.0f;
		else
			return tmp;
	};


	float side_speed = 1.f;
	auto velocity = GameHandler->Velocity;
	velocity.z = 0.0f;

	auto forwardmove = cmd->Base->forwardmove;
	auto sidemove = cmd->Base->sidemove;

	if (velocity.Length2D() < 5.0f && !forwardmove && !sidemove)
		return;

	static auto flip = false;
	flip = !flip;

	auto turn_direction_modifier = flip ? 1.0f : -1.0f;
	auto viewangles = cmd->Base->view->angles;

	if (forwardmove || sidemove)
	{
		cmd->Base->forwardmove = 0.0f;
		cmd->Base->sidemove = 0.0f;

		auto turn_angle = atan2(-sidemove, forwardmove);
		viewangles.y += turn_angle * 57.295779513082f;
	}
	else if (forwardmove) //-V550
		cmd->Base->forwardmove = 0.0f;

	auto strafe_angle = RAD2DEG(atan(15.0f / velocity.Length2D()));

	if (strafe_angle > 90.0f)
		strafe_angle = 90.0f;
	else if (strafe_angle < 0.0f)
		strafe_angle = 0.0f;

	auto temp = Vec3(0.0f, viewangles.y - old_yaw, 0.0f);
	temp.y = Math::NormalizeYaw(temp.y);

	auto yaw_delta = temp.y;
	old_yaw = viewangles.y;

	auto abs_yaw_delta = fabs(yaw_delta);

	if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 15.0f) // whatsapp turbo ativado
	{
		Vec3 velocity_angles = velocity;
		velocity_angles = velocity_angles.ToAngles();

		temp = Vec3(0.0f, viewangles.y - velocity_angles.y, 0.0f);
		temp.y = Math::NormalizeYaw(temp.y);

		auto velocityangle_yawdelta = temp.y;
		auto velocity_degree = get_velocity_degree(velocity.Length2D());

		if (velocityangle_yawdelta <= velocity_degree || velocity.Length2D() <= 15.0f)
		{
			if (-velocity_degree <= velocityangle_yawdelta || velocity.Length2D() <= 15.0f)
			{
				viewangles.y += strafe_angle * turn_direction_modifier;
				cmd->Base->sidemove = side_speed * turn_direction_modifier;
			}
			else
			{
				viewangles.y = velocity_angles.y - velocity_degree;
				cmd->Base->sidemove = side_speed;
			}
		}
		else
		{
			viewangles.y = velocity_angles.y + velocity_degree;
			cmd->Base->sidemove = -side_speed;
		}
	}
	else if (yaw_delta > 0.0f)
		cmd->Base->sidemove = -side_speed;
	else if (yaw_delta < 0.0f)
		cmd->Base->sidemove = side_speed;

	auto move = Vec3(cmd->Base->forwardmove, cmd->Base->sidemove, 0.0f);
	auto speed = move.Length();

	Vec3 angles_move = move;
	angles_move.ToAngles();

	auto normalized_x = fmod(cmd->Base->view->angles.x + 180.0f, 360.0f) - 180.0f;
	auto normalized_y = fmod(cmd->Base->view->angles.y + 180.0f, 360.0f) - 180.0f;

	auto yaw = DEG2RAD(normalized_y - viewangles.y + angles_move.y);

	if (normalized_x >= 90.0f || normalized_x <= -90.0f || cmd->Base->view->angles.x >= 90.0f && cmd->Base->view->angles.x <= 200.0f || cmd->Base->view->angles.x <= -90.0f && cmd->Base->view->angles.x <= 200.0f) //-V648
		cmd->Base->forwardmove = -cos(yaw) * speed;
	else
		cmd->Base->forwardmove = cos(yaw) * speed;

	cmd->Base->sidemove = sin(yaw) * speed;
}
void Movement::QuickStop(UserCmd* cmd) {
	


	bool pressed_move_key = cmd->Buttons.Button1 & in_moveforward || cmd->Buttons.Button1 & in_moveleft || cmd->Buttons.Button1 & in_moveright || cmd->Buttons.Button1 & in_moveback || cmd->Buttons.Button1 & in_jump;

	if (pressed_move_key)
		return;


	if (GameHandler->VelocityLength2D > 30.f) {
		//GameHandler->Log(std::to_string(GameHandler->cache.VelocityLength2D).c_str()); GameHandler->Log("\n");
		Vec3 direction = GameHandler->local->m_vecVelocity();
		direction.NormalizeInPlace();
		direction.ToAngles();
		direction.y = GameHandler->OriginalCmdViewAngles.y - direction.y;
		direction.Normalize();
		direction.ToVector();

		direction *= -1.f;

		cmd->Base->forwardmove = direction.x * GameHandler->VelocityLength2D;


		cmd->Base->sidemove = direction.y * GameHandler->VelocityLength2D;

	}
}
void Movement::MoveFix(UserCmd* cmd) {
	
	Vec3 Movement(cmd->Base->forwardmove, cmd->Base->sidemove, cmd->Base->upmove);
	float MoveSpeed = Movement.Length2D();

	Movement.ToAngles();

	float yaw = DEG2RADF(GameHandler->CmdViewAngle.y - GameHandler->ActiveViewAngle.y + Movement.y);

	cmd->Base->forwardmove = cos(yaw) * MoveSpeed;
	cmd->Base->sidemove = sin(yaw) * MoveSpeed;





	cmd->Buttons.Button1 &= ~in_moveforward;
	cmd->Buttons.Button1 &= ~in_moveback;
	cmd->Buttons.Button1 &= ~in_moveleft;
	cmd->Buttons.Button1 &= ~in_moveright;


	if (cmd->Base->forwardmove > 0.f)
		cmd->Buttons.Button1 |= in_moveforward;
	else if (cmd->Base->forwardmove < 0.f)
		cmd->Buttons.Button1 |= in_moveback;

	if (cmd->Base->sidemove > 0.f)
	{

		cmd->Buttons.Button1 |= in_moveright;
	}
	else if (cmd->Base->sidemove < 0.f)
	{

		cmd->Buttons.Button1 |= in_moveleft;
	}
}

void Movement::DoBunnyhop(UserCmd* cmd) {



	static bool bLastJumped = false;
	static bool bShouldFake = false;
	bool OnGround = GameHandler->local->m_fFlags() & FL_ONGROUND;
	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		cmd->Buttons.Button1 |= in_jump;
	}
	else if (cmd->Buttons.Button1 & in_jump)
	{
		if (OnGround)
		{
			bShouldFake = bLastJumped = true;
		}
		else
		{
			cmd->Buttons.Button1 &= ~in_jump;
			bLastJumped = false;
		}
	}
	else
	{
		bShouldFake = bLastJumped = false;
	}
	auto movetype = GameHandler->local->m_MoveType();
	if (movetype == MoveType_t::movetype_noclip || movetype == MoveType_t::movetype_ladder)
		return;

	if (/*cmd->Buttons.Button1 & IN_SPEED  IN_SPEED doesnt exist*/ GameHandler->VelocityLength2D < 1.f)
		return; 

	if (OnGround)
		return;

	Movement::DoStrafe(cmd);
}

void Movement::DoMovement(UserCmd* cmd) {
	if(Config->Movement.Bunnyhop)
		DoBunnyhop(cmd);

	if (Config->Movement.QuickStop)
		QuickStop(cmd);

	//MoveFix(cmd);
}