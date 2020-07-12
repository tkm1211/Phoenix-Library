#pragma once


class AI
{
public:
	AI() {}
	virtual ~AI() {}

public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void GUI() = 0;
};