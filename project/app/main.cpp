#include "engine.hpp"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	winapp app(800, 450);
	graphic_d3d12 d3d12(app);

	while(app.isloop())
	{
		app.update();
		d3d12.render();
	}

	return 0;
}