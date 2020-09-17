#pragma once


class AI
{
public:
	AI() {}
	virtual ~AI() {}

public:
	virtual void Construct() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void GUI() = 0;
};