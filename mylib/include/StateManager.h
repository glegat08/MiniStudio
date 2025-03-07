#pragma once

class IState
{
public:
	virtual void update() = 0;
	virtual void handleInput() = 0;
	virtual void setTexture() = 0;
};

class Idle : public IState
{
public:

};

class Movement : public IState
{
public:

};

class Attack : public IState
{
public:

};

class Block : public IState
{
public:

};

class Hurt : public IState
{
public:

};

class Death : public IState
{
public:

};