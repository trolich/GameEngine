#pragma once

enum class EntityMessageType {
	Move = 0, Is_Moving, Frame_Change, Switch_State, State_Change, Direction_Change,
	Attack, Being_Attacked, Hurt, Die, Respawn, Activate, Break, Removed 
};