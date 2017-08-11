#include <D3D11_Framework.h>

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init(HWND hwnd);
	bool Draw();
	void Close();
private:
	Image *m_img;
};
