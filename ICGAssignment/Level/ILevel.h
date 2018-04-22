#pragma once


class ILevel
{
public:
	virtual ~ILevel() {}
	virtual void InitLevel() = 0;
	virtual void DisplayLevel() = 0;
	virtual void CloseLevel() = 0;
	virtual void UpdateMainLight(float i_pitch, float i_yaw) = 0;
	virtual void HandleNormalKey(char i_char) = 0;

private:
};

