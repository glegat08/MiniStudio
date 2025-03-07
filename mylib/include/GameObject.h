#pragma once

class IGameObject
{
public:
	virtual ~IGameObject() = default;

	virtual int getHp() = 0;
	virtual void getStat() = 0;
};