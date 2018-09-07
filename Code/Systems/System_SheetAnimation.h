#pragma once
#include "System.h"
#include "Component_State.h"
#include "Component_SpriteSheet.h"

class System_SheetAnimation : public System
{
public:
	System_SheetAnimation(SystemManager*);
	~System_SheetAnimation();
	void Update(float);
	void Notify(const Message&);
	void HandleEvents(const EntityID&, const EntityEvent&);

private:
	void ChangeAnimation(const EntityID&, const std::string&, bool play, bool loop);
};