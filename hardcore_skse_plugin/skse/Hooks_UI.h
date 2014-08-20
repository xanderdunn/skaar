#pragma once

class UIDelegate
{
public:
	virtual void	Run(void)		= 0;
	virtual void	Dispose(void)	= 0;
};

void Hooks_UI_Commit(void);
