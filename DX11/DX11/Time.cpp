#include "stdafx.h"
#include "Time.h"


Time::Time()
{
	m_deltaTime = 1.0f / 60.0f;
}


Time::~Time()
{
}

void Time::Update() 
{
	m_frameTimeCount++;

	//delta time
	m_deltaTime = 1.0f / 60.0f;

	//current time
	m_Time += m_deltaTime;
}
