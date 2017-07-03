#include <D3D11_Framework.h>

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	bool Init(HWND nwnd);
	bool Draw();
	void Close();
};
