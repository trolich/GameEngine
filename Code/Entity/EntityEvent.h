#pragma once

enum class EntityEvent
{
	Spawned = 0, Despawned, Moving_Up, Moving_Down, Moving_Left, Moving_Right,
	Colliding_X, Colliding_Y, Elevation_Change, Began_Moving, Became_Idle, Began_Attacking
};