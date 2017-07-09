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
	friend class Image;

	Image *m_img;

	XMMATRIX m_Ortho;
};
