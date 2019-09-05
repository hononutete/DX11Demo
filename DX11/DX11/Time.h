#pragma once
class Time
{
public:
	UINT m_frameTimeCount = 0;
	double m_Time = 0.0;
	float m_deltaTime = 0.0f;

	Time();
	~Time();

	void Update();

};

