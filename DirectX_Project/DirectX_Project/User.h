#include <math.h>
#include <D3D11_Framework.h>
#include <xnamath.h>

using namespace D3D11Framework;

class User : public InputListener
{
public:
	User(XMMATRIX *matr, int x = 5, int y = 5, int z = 5);

	static User* Get() { return m_this; }
	XMVECTOR GetTarget() { return m_Target; }

	// движение мыши
	bool MouseMove(const MouseEvent &arg);
	// кнопка нажата
	bool KeyPressed(const KeyEvent &arg);
	// кнопка отпущена
	bool KeyReleased(const KeyEvent &arg);

	void Refresh();

private:
	//int m_x;
	//int m_y;
	//float m_coord_x, m_coord_y, m_coord_z;
	static User *m_this;

	float m_speed;
	XMMATRIX *m_User_View;
	XMVECTOR m_Position;
	XMVECTOR m_Target;
	XMVECTOR m_Right_Vect;
	float m_Yaw, m_Pitch; // ”глы поворота и наклона
};