#include "MyRender.h"

int main()
{
	Framework framework;

	MyRender *render = new MyRender();

	
	FrameworkDesc myDesk;
	myDesk.render = render;
	myDesk.wnd.height = 1000;
	myDesk.wnd.width = 1400;

	framework.Init(myDesk);

	framework.Run();

	framework.Close();

	return 0;
}
