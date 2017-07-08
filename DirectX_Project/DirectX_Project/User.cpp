#include "User.h"


User *User::m_this = nullptr;


User::User(XMMATRIX *matr, int x, int y, int z)
{
	m_Position = XMVectorSet(x, y, z, 0.0f);
	/*m_coord_x = x;
	m_coord_y = y;
	m_coord_z = z;
	m_x = 0;
	m_y = 0;*/
	m_Yaw = 0;
	m_Pitch = 0;
	m_User_View = matr;
	m_this = this;

	m_speed = 0.4;

	this->Refresh();
}

// движение мыши
bool User::MouseMove(const MouseEvent &arg)
{
	/*if (arg.x > 0 && arg.x < 50 && arg.y > 0 && arg.y < 50)
		Log::Get()->Debug("Yes\n");*/
	m_Yaw += (arg.x - Window::Get()->GetWidth() / 2)/4;
	fmod(m_Yaw, 360);
	m_Pitch += (arg.y - Window::Get()->GetHeight() / 2)/4;
	fmod(m_Pitch, 360);
	/*if (m_Pitch > 180)
		m_Pitch = 180;
	if (m_Pitch < 0)
		m_Pitch = 0;*/

	//Log::Get()->Debug("%d %d\n", m_x, m_y);
	SetCursorPos(Window::Get()->GetWidth() / 2 + Window::Get()->GetLeft(), Window::Get()->GetHeight() / 2 + Window::Get()->GetTop());
	this->Refresh();

	return false;
}
// кнопка нажата
bool User::KeyPressed(const KeyEvent &arg)
{
	switch (arg.code)
	{
	case KEY_ESCAPE:
		Window::Get()->Close();
		break;
	case KEY_W:
		m_Position += (m_Target - m_Position)*m_speed;//+= XMVectorSet(m_speed*cosf(pitchRadian)*sinf(yawRadian), -m_speed*sinf(pitchRadian), m_speed*cosf(pitchRadian)*cosf(yawRadian), 0.0f);
		this->Refresh();
		break;
	case KEY_S:
		m_Position -= (m_Target - m_Position)*m_speed;//-= XMVectorSet(m_speed*cosf(pitchRadian)*sinf(yawRadian), -m_speed*sinf(pitchRadian), m_speed*cosf(pitchRadian)*cosf(yawRadian), 0.0f);
		this->Refresh();
		break;
	case KEY_D:
		m_Position += m_Right_Vect*m_speed;
		this->Refresh();
		break;
	case KEY_A:
		m_Position -= m_Right_Vect*m_speed;
		this->Refresh();
		break;
	default:
		break;
	}
		
	return false;
}
// кнопка отпущена
bool User::KeyReleased(const KeyEvent &arg)
{
	return false;
}

void User::Refresh()
{
	/*float vr = 1;
	float vtx = (sin(m_x*PI / 180.)*cos(m_y*PI / 180.))*vr + m_coord_x;
	float vty = (sin(m_x*PI / 180.)*sin(m_y*PI / 180.))*vr + m_coord_y;
	float vtz = (cos(m_x*PI / 180.))*vr + m_coord_z;*/
	XMMATRIX Rotation = XMMatrixRotationY(m_Yaw / 180 * XM_PI);
	XMMATRIX Rotation_Right_Vector = XMMatrixRotationY(m_Yaw / 180 * XM_PI + XM_PIDIV2);
	m_Right_Vect = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	m_Right_Vect = XMVector3Transform(m_Right_Vect, Rotation_Right_Vector);

	Rotation *= XMMatrixRotationAxis(m_Right_Vect, m_Pitch / 180 * XM_PI);

	//XMMATRIX mtxRotation = XMMatrixRotationRollPitchYaw(m_Pitch / 180 * XM_PI, m_Yaw / 180 * XM_PI, 0.0f);
	/*XMMATRIX rotation;
	XMMatrixRotationAxis(&rotation, &m_up, radians);
	XMVector3TransformNormal
	XVec3T(&m_right, &m_right, &rotation);
	D3DXVec3TransformNormal(&m_look, &m_look, &rotation);*/
	
	/*float vr = (float)(sqrt(pow(m_x, 2) + pow(m_y, 2) + pow(1, 2)));
	float vtx = (float)(sin(m_x)*sin(m_x))*vr;
	float vty = (float)(cos(m_x))*vr;
	float vtz = (float)(sin(m_x)*cos(m_x))*vr;*/
	//Log::Get()->Debug("%f %f %f\n", vtx, vty, vtz);
	XMVECTOR forward = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), Rotation);
	m_Target = m_Position + forward;
	//Log::Get()->Debug("Target: %f %f %f\n", XMVectorGetByIndex(m_Target, 0), XMVectorGetByIndex(m_Target, 1), XMVectorGetByIndex(m_Target, 2));
	
	XMVECTOR vecUp = XMVector3Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), Rotation);
	//Log::Get()->Debug("%f %f %f\n", vtx, vty, vtz);
	//XMVECTOR Eye = XMVectorSet(m_coord_x, m_coord_y, m_coord_z, 0.0f);
	//XMVECTOR At = XMVectorSet(vtx, vty, vtz, 0.0f);
	//XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	*m_User_View = XMMatrixLookAtLH(m_Position, m_Target, vecUp);
}
