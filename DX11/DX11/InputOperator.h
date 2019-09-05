#pragma once
class InputOperator
{
public:
	InputOperator();
	~InputOperator();

	virtual void Dispatch() = 0;
};

