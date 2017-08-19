#include <D3D11_Framework.h>
#include "Structs.h"
using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init();
	bool Draw();
	void Close();
private:
	//StaticMesh *m_mesh;
	XMMATRIX m_view;
	ID3D11Buffer* IndexBuffer;
	ID3D11Buffer* VertBuffer;
	ID3D11Buffer* constMatrixBuffer;
	ID3D11Buffer* constLightBuffer;
	ID3D11Buffer* m_material;
	MaterialProperties pearlMaterial;
	LightProperties *light_prop;
	Light *light;
	Shader *shader;
};
