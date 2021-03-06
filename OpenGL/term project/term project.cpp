#include "stdafx.h"

#include "VECTOR.h"
#include "Matrix.h"
#include "Face.h"
#include "Mesh.h"
#include "Init.h"

#include <stdlib.h>
#include <gl\glut.h>
#include <gl\GLU.h>
#include <math.h>

#define PI 3.14159265358979
#define deg_to_rad(deg) (((deg)/360)*2*PI)

using namespace std;

bool DepthTest = true;

int object = 0;
int translation = 1;
int rotation = 0;
int drag_mouse = 0;
int last_mx, last_my;
int coordinate = 1;
int b = 0;

float px = -5;
float py = 0;
float pz = 0;
float p1x = 5;
float p1y = 0;
float p1z = 0;
float ax = 0;
float ay = 0;
float az = 0;
float a1x = 0;
float a1y = 0;
float a1z = 0;

double zoom = 45;
double deg = 0;



Mesh mesh1;
Mesh mesh2;
Mesh mesh3;

Mesh mesh4;
Mesh mesh5;
Mesh mesh6;
Mesh mesh7;
Mesh mesh8;
Mesh mesh9;
Mesh mesh10;


float VECTOR3D::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

float VECTOR3D::InnerProduct(VECTOR3D v)
{
	return (x * v.x + y * v.y + z * v.z);
}

VECTOR3D VECTOR3D::CrossProduct(VECTOR3D v)
{
	VECTOR3D result;
	result.x = y * v.z - z * v.y;
	result.y = z * v.x - x * v.z;
	result.z = x * v.y - y * v.x;

	return result;
}

void VECTOR3D::Normalize()
{
	float w = Magnitude();
	if (w < 0.00001) return;
	x /= w;
	y /= w;
	z /= w;
}

VECTOR3D VECTOR3D::operator+(VECTOR3D v)
{
	VECTOR3D result = (*this);
	result.x += v.x;
	result.y += v.y;
	result.z += v.z;

	return result;
}

VECTOR3D& VECTOR3D::operator+=(VECTOR3D v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

VECTOR3D VECTOR3D::operator-(VECTOR3D v)
{
	VECTOR3D result = (*this);
	result.x -= v.x;
	result.y -= v.y;
	result.z -= v.z;

	return result;
}

VECTOR3D VECTOR3D::operator*(float val)
{
	VECTOR3D result = (*this);
	result.x *= val;
	result.y *= val;
	result.z *= val;

	return result;
}

VECTOR3D VECTOR3D::operator/(float val)
{
	VECTOR3D result = (*this);
	result.x /= val;
	result.y /= val;
	result.z /= val;

	return result;
}

VECTOR3D VECTOR3D::operator/=(float val)
{
	this->x /= val;
	this->y /= val;
	this->z /= val;

	return *this;
}

VECTOR3D operator*(float val, VECTOR3D v)
{
	return v * val;
}

void MeshLoad()
{
	//To do

	mesh1.LoadMesh("batman2.obj");
	
	mesh3.LoadMesh("barrel_test.obj");

	mesh2.LoadMesh("sword2_test.obj");
	mesh4.LoadMesh("sword2_test.obj");
	mesh5.LoadMesh("sword2_test.obj");
	mesh6.LoadMesh("sword2_test.obj");
	mesh7.LoadMesh("sword2_test.obj");
	mesh8.LoadMesh("sword2_test.obj");
	mesh9.LoadMesh("sword2_test.obj");
	mesh10.LoadMesh("sword2_test.obj");
}

void ComputeNormal()
{
	//To do

	mesh1.ComputeFaceNormal();
	mesh1.FindNeighborFaceArray();
	mesh1.ComputeVertexNormal();

	mesh2.ComputeFaceNormal();
	mesh2.FindNeighborFaceArray();
	mesh2.ComputeVertexNormal();

	mesh3.ComputeFaceNormal();
	mesh3.FindNeighborFaceArray();
	mesh3.ComputeVertexNormal();

	mesh4.ComputeFaceNormal();
	mesh4.FindNeighborFaceArray();
	mesh4.ComputeVertexNormal();


	mesh5.ComputeFaceNormal();
	mesh5.FindNeighborFaceArray();
	mesh5.ComputeVertexNormal();

	mesh6.ComputeFaceNormal();
	mesh6.FindNeighborFaceArray();
	mesh6.ComputeVertexNormal();

	mesh7.ComputeFaceNormal();
	mesh7.FindNeighborFaceArray();
	mesh7.ComputeVertexNormal();

	mesh8.ComputeFaceNormal();
	mesh8.FindNeighborFaceArray();
	mesh8.ComputeVertexNormal();

	mesh9.ComputeFaceNormal();
	mesh9.FindNeighborFaceArray();
	mesh9.ComputeVertexNormal();

	mesh10.ComputeFaceNormal();
	mesh10.FindNeighborFaceArray();
	mesh10.ComputeVertexNormal();


}

void Mesh::LoadMesh(string str)
{
	ifstream file(str);
	string inputString;
	while (!file.eof())
	{
		getline(file, inputString);
		if (inputString[0] == '#')
			continue;
		string delimiter = " ";
		string tokens[4];
		size_t pos = 0;
		int index = 0;
		string token;
		while ((pos = inputString.find(delimiter)) != std::string::npos)
		{
			token = inputString.substr(0, pos);
			tokens[index] = token;
			inputString.erase(0, pos + delimiter.length());
			index++;
		}
		tokens[3] = inputString;

		if (tokens[0] == "v")
		{
			Vertex v;
			v.position = VECTOR3D(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
			vertexArray.push_back(v);
		}

		else if (tokens[0] == "f")
		{
			Face f;
			f.vertex0 = stoi(tokens[1]);
			f.vertex1 = stoi(tokens[2]);
			f.vertex2 = stoi(tokens[3]);
			f.vertex0--;
			f.vertex1--;
			f.vertex2--;
			faceArray.push_back(f);
		}

	}
	file.close();
}

void Mesh::ComputeFaceNormal()
{
	for (int i = 0; i < faceArray.size(); i++)
	{
		Face &f = faceArray[i];

		VECTOR3D v0 = vertexArray[f.vertex0].position;
		VECTOR3D v1 = vertexArray[f.vertex1].position;
		VECTOR3D v2 = vertexArray[f.vertex2].position;

		VECTOR3D va = v1 - v0;
		VECTOR3D vb = v2 - v0;
		VECTOR3D vc = va.CrossProduct(vb);
		vc.Normalize();
		f.normal = vc;
	}
}

void Mesh::FindNeighborFaceArray()
{
	for (int i = 0; i < vertexArray.size(); i++)
	{
		for (int j = 0; j < faceArray.size(); j++)
		{
			if (faceArray[j].vertex0 == i || faceArray[j].vertex1 == i || faceArray[j].vertex2 == i)
			{
				vertexArray[i].neighborFaces.push_back(j);
			}
		}
	}
}

void Mesh::ComputeVertexNormal()
{
	for (int i = 0; i < vertexArray.size(); i++)
	{
		Vertex& v = vertexArray[i];
		for (int j = 0; j < vertexArray[i].neighborFaces.size(); j++)
		{
			v.normal += faceArray[vertexArray[i].neighborFaces[j]].normal;
		}
		v.normal /= vertexArray[i].neighborFaces.size();
	}
}

void Mouse(int mouse_event, int state, int x, int y)
{
	//To Do
	if (mouse_event == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		drag_mouse = 1;


	if (mouse_event == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		drag_mouse = 0;

	last_mx = x;
	last_my = y;


	glutPostRedisplay();
}

void Motion(int x, int y)
{


	//To Do
	if (object == 1) {
		if (translation == 1) {
			if (coordinate == 1) {
				if (drag_mouse == 1)
					px = px + (x - last_mx) * 0.1;
			}

			else if (coordinate == 2) {
				if (drag_mouse == 1)
					py = py + (last_my - y) * 0.1;
			}

			else if (coordinate == 3) {
				if (drag_mouse == 1)
					pz = pz + (y - last_my) * 0.1;
			}
		}

		else if (rotation == 1) {
			if (coordinate == 1) {
				if (drag_mouse == 1) {
					ax = ax + (y - last_my) * 1;
				}
			}

			else if (coordinate == 2) {
				if (drag_mouse == 1) {
					ay = ay + (last_mx - x) * 1;

				}
			}

			else if (coordinate == 3) {
				if (drag_mouse == 1) {
					az = az + (x - last_mx) * 1;

				}
			}
		}
	}
	
	else if (object == 2) {
		if (translation == 1) {
			if (coordinate == 1) {
				if (drag_mouse == 1)
					p1x = p1x + (x - last_mx) * 0.1;
				printf("p1x = %d\n", p1x);
			}

			else if (coordinate == 2) {
				if (drag_mouse == 1)
					p1y = p1y + (last_my - y) * 0.1;
				printf("p1y = %d\n", p1y);
			}

			else if (coordinate == 3) {
				if (drag_mouse == 1)
					p1z = p1z + (y - last_my) * 0.1;
				printf("p1z = %d\n", p1z);
			}
		}

		else if (rotation == 1) {
			if (coordinate == 1) {
				if (drag_mouse == 1)
					a1x = a1x + (y - last_my) * 1;
				printf("a1x = %d\n", a1x);
			}

			else if (coordinate == 2) {
				if (drag_mouse == 1)
					a1y = a1y + (last_mx - x) * 1;
				printf("a1y = %d\n", a1y);
			}

			else if (coordinate == 3) {
				if (drag_mouse == 1)
					a1z = a1z + (x - last_mx) * 1;
				printf("a1z = %d\n", a1z);
			}
		}
	}
	

	last_mx = x;
	last_my = y;




	glutPostRedisplay();
}

void DrawBatman(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh1.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh1.faceArray[i].normal.x, mesh1.faceArray[i].normal.y, mesh1.faceArray[i].normal.z);
		Vertex v0 = mesh1.vertexArray[mesh1.faceArray[i].vertex0];
		Vertex v1 = mesh1.vertexArray[mesh1.faceArray[i].vertex1];
		Vertex v2 = mesh1.vertexArray[mesh1.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh2.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh2.vertexArray[mesh2.faceArray[i].vertex0];
		Vertex v1 = mesh2.vertexArray[mesh2.faceArray[i].vertex1];
		Vertex v2 = mesh2.vertexArray[mesh2.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword1(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh4.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh4.vertexArray[mesh4.faceArray[i].vertex0];
		Vertex v1 = mesh4.vertexArray[mesh4.faceArray[i].vertex1];
		Vertex v2 = mesh4.vertexArray[mesh4.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword2(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh5.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh5.vertexArray[mesh5.faceArray[i].vertex0];
		Vertex v1 = mesh5.vertexArray[mesh5.faceArray[i].vertex1];
		Vertex v2 = mesh5.vertexArray[mesh5.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword3(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh6.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh6.vertexArray[mesh6.faceArray[i].vertex0];
		Vertex v1 = mesh6.vertexArray[mesh6.faceArray[i].vertex1];
		Vertex v2 = mesh6.vertexArray[mesh6.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword4(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh7.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh7.vertexArray[mesh7.faceArray[i].vertex0];
		Vertex v1 = mesh7.vertexArray[mesh7.faceArray[i].vertex1];
		Vertex v2 = mesh7.vertexArray[mesh7.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword5(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh8.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh8.vertexArray[mesh8.faceArray[i].vertex0];
		Vertex v1 = mesh8.vertexArray[mesh8.faceArray[i].vertex1];
		Vertex v2 = mesh8.vertexArray[mesh8.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword6(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh9.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh9.vertexArray[mesh9.faceArray[i].vertex0];
		Vertex v1 = mesh9.vertexArray[mesh9.faceArray[i].vertex1];
		Vertex v2 = mesh9.vertexArray[mesh9.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawSword7(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh10.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh10.vertexArray[mesh10.faceArray[i].vertex0];
		Vertex v1 = mesh10.vertexArray[mesh10.faceArray[i].vertex1];
		Vertex v2 = mesh10.vertexArray[mesh10.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}

void DrawBarrel(void)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh3.faceArray.size(); i++)
	{
		//face normal
		//glNormal3f(mesh2.faceArray[i].normal.x, mesh2.faceArray[i].normal.y, mesh2.faceArray[i].normal.z);
		Vertex v0 = mesh3.vertexArray[mesh3.faceArray[i].vertex0];
		Vertex v1 = mesh3.vertexArray[mesh3.faceArray[i].vertex1];
		Vertex v2 = mesh3.vertexArray[mesh3.faceArray[i].vertex2];

		//vertex normal
		glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
		glVertex3f(v0.position.x, v0.position.y, v0.position.z);
		glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
		glVertex3f(v1.position.x, v1.position.y, v1.position.z);
		glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
		glVertex3f(v2.position.x, v2.position.y, v2.position.z);
	}
	glEnd();
}
void RenderPlane()
{
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.8, 0.8);
	glNormal3f(0, 1, 0);
	glVertex3f(-10.0f, -1.0f, 10.0f);
	glVertex3f(-10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, -10.0f);
	glVertex3f(10.0f, -1.0f, 10.0f);
	glEnd();
}


void Rendering(void)
{
	// 화면 버퍼 클리어
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(zoom, 1, 0.1, 500);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 화면을 제대로 바라보기 위해 카메라를 회전 후 이동
	//gluLookAt(30 * sin(deg_to_rad(deg)), 20.0f, 30 * cos(deg_to_rad(deg)), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword1
	gluLookAt(0, 35.0f, 35, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	//sword2
	//gluLookAt(35, 35.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword3
	//gluLookAt(0, 35.0f, -35, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword4
	//gluLookAt(-35, 35.0f, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword5
	//gluLookAt(35/sqrt(2), 35, 35/sqrt(2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword6
	//gluLookAt(-35/sqrt(2), 35, 35/sqrt(2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword7
	//gluLookAt(35/sqrt(2), 35, -35/sqrt(2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	//sword8
	//gluLookAt(-35/sqrt(2), 35, -35/sqrt(2), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f);
	//glNormal3f(0, 1, 0);
	glVertex3f(-20.0f, 20.0f, 0);
	glVertex3f(-20.0f, -20.0f, 0);
	glVertex3f(20.0f, -20.0f, 0);
	glVertex3f(20.0f, 20.0f, 0);
	glEnd();
	

	glDisable(GL_CULL_FACE);
	RenderPlane();

	glPushMatrix();
	/*
	glTranslatef(px, py, pz);
	glRotatef(ax, 1.0, 0.0, 0.0);
	glRotatef(ay, 0.0, 1.0, 0.0);
	glRotatef(az, 0.0, 0.0, 1.0);

	if (object == 1)
		glColor3f(1.0, 0.0, 0.0);
	else
		glColor3f(1.0, 1.0, 1.0);
		*/
	glTranslatef(0, 10, 0);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	/*
	matAmb[0] = 0;
	matAmb[1] = 0;
	matAmb[2] = 0;
	matAmb[3] = 0;
	matDif[0] = 0;
	matDif[1] = 0;
	matDif[2] = 0;
	matDif[3] = 0;
	matSpec[0] = 0;
	matSpec[1] = 0;
	matSpec[2] = 0;
	matSpec[3] = 0;
	
	matShininess = 200;
	
	matAmb[0] = 0;
	matAmb[1] = 0;
	matAmb[2] = 0;
	matAmb[3] = 0;
	matDif[0] = 0;
	matDif[1] = 0;
	matDif[2] = 0;
	matDif[3] = 0;
	matSpec[0] = 0;
	matSpec[1] = 0;
	matSpec[2] = 0;
	matSpec[3] = 0;
	matShininess = 200;

	*/
	//bronze
	float noMat[] = { 0, 0, 0, 1 };
	float matAmb[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
	float matDif[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
	float matSpec[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
	float matShininess = 25.6f;

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	DrawBatman();

	glPopMatrix();
	glPushMatrix();
	/*
	glTranslatef(p1x, p1y, p1z);

	glRotatef(a1x, 1.0, 0.0, 0.0);
	glRotatef(a1y, 0.0, 1.0, 0.0);
	glRotatef(a1z, 0.0, 0.0, 1.0);

	if (object == 2)
		glColor3f(1.0, 0.0, 0.0);
	else
		glColor3f(1.0, 1.0, 1.0);
		*/
	glTranslatef(1, 5, 15);
	glRotatef(90, 1.0, 0.0, 0.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	//gold
	matAmb[0] = 0.24725f;
	matAmb[1] = 0.1995f;
	matAmb[2] = 0.0745f;
	matAmb[3] = 1;
	matDif[0] = 0.75164f;
	matDif[1] = 0.60648f;
	matDif[2] = 0.22648f;
	matDif[3] = 1;
	matSpec[0] = 0.628281f;
	matSpec[1] = 0.555802f;
	matSpec[2] = 0.366065f;
	matSpec[3] = 1;
	matShininess = 51.2f;

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	DrawSword();


	
	glPopMatrix();
	glPushMatrix();

	glTranslatef(15, 5, -1);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword1();


	glPopMatrix();
	glPushMatrix();

	glTranslatef(1, 5, -15);
	glRotatef(-90, 1.0, 0.0, 0.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword2();

	
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-15, 5, 1);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword3();

	glPopMatrix();
	glPushMatrix();

	glTranslatef(15/sqrt(2), 5, 15 / sqrt(2));
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-45, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword4();

	glPopMatrix();
	glPushMatrix();

	glTranslatef(-15 / sqrt(2), 5, 15 / sqrt(2));
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword5();

	glPopMatrix();
	glPushMatrix();

	glTranslatef(15 / sqrt(2), 5, -15 / sqrt(2));
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-135, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword6();

	glPopMatrix();
	glPushMatrix();

	glTranslatef(-15 / sqrt(2), 5, -15 / sqrt(2));
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(135, 0.0, 0.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(0.05f, 0.05f, 0.05f);
	DrawSword7();


	glPopMatrix();
	glPushMatrix();
	
	

	glTranslatef(0, 5, 0);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glScalef(5, 5, 5);

	//yellow plastic
	matAmb[0] = 0;
	matAmb[1] = 0;
	matAmb[2] = 0;
	matAmb[3] = 1;
	matDif[0] = 0.5f;
	matDif[1] = 0.5f;
	matDif[2] = 0;
	matDif[3] = 1;
	matSpec[0] = 0.60f;
	matSpec[1] = 0.60f;
	matSpec[2] = 0.50f;
	matSpec[3] = 1;
	matShininess = 32.0f;

	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	DrawBarrel();

	

	// back 버퍼에 랜더링한 후 swap
	glutSwapBuffers();

}

void Reshape(int w, int h)
{
	// 뷰포트 설정
	glViewport(0, 0, w, h);

	// 원근 투영 사용
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);

	// 모델뷰 매트릭스 초기화
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void Keyboard(unsigned char key, int x, int y)
{
	//To Do

	switch (key) {
	case '1':
		object = 1;
		printf("Torus\n");
		if (translation == 1) {
			printf("translation\n");
			if (coordinate == 1)
				printf("x axis\n");

			else if (coordinate == 2)
				printf("y axis\n");

			else
				printf("z axis\n");
		}

		if (rotation == 1) {
			printf("rotation\n");
			if (coordinate == 1)
				printf("x axis\n");

			else if (coordinate == 2)
				printf("y axis\n");

			else
				printf("z axis\n");
		}
		break;

	case '2':
		object = 2;
		printf("Teapot\n");
		if (translation == 1) {
			printf("translation\n");
			if (coordinate == 1)
				printf("x axis\n");

			else if (coordinate == 2)
				printf("y axis\n");

			else
				printf("z axis\n");
		}

		if (rotation == 1) {
			printf("rotation\n");
			if (coordinate == 1)
				printf("x axis\n");

			else if (coordinate == 2)
				printf("y axis\n");

			else
				printf("z axis\n");
		}
		break;

	case 't':
		translation = 1;
		rotation = 0;
		printf("translation\n");

		if (coordinate == 1)
			printf("x axis\n");

		else if (coordinate == 2)
			printf("y axis\n");

		else
			printf("z axis\n");


		break;

	case 'r':
		translation = 0;
		rotation = 1;
		printf("rotation\n");
		if (coordinate == 1)
			printf("x axis\n");

		else if (coordinate == 2)
			printf("y axis\n");

		else
			printf("z axis\n");
		break;

	case 'x':
		coordinate = 1;
		printf("x axis\n");
		break;

	case 'y':
		coordinate = 2;
		printf("y axis\n");
		break;

	case 'z':
		coordinate = 3;
		printf("z axis\n");
		break;

	case 'a':
		deg = deg - 1;
		break;

	case 'd':
		deg = deg + 1;
		break;

	case 's':
		if (zoom < 179)
			zoom += 1;
		break;

	case 'w':
		if (zoom > 1)
			zoom -= 1;
		break;

	case '3':
		DepthTest = !DepthTest;

		if (DepthTest) {
			glEnable(GL_DEPTH_TEST);
			printf("Depth Test ON\n");
		}
		else {
			glDisable(GL_DEPTH_TEST);
			printf("Depth Test OFF\n");
		}
		break;
	}

	glutPostRedisplay();
}

void EventHandlingAndLoop()
{
	glutKeyboardFunc(Keyboard);  // 키보드 입력 처리 Callback 함수 등록
	glutDisplayFunc(Rendering);  // 변환된 값에 따른 Rendering Callback 함수 등록
	glutReshapeFunc(Reshape);    // 윈도우 창 크기가 바뀌었을때 호출되는 Callback 함수 등록
	glutMouseFunc(Mouse);        // 마우스 클릭 처리 Callback 함수 등록
	glutMotionFunc(Motion);      // 마우스 움직임 처리 Callback 함수 등록

	glutMainLoop(); // 등록된 callback 함수를 반복하여 호출
}

int main(int argc, char** argv)
{
	Initialize(argc, argv);			  // 윈도우 생성, 배경색 설정

	MeshLoad();       //To Do
	ComputeNormal();  //To Do

	EventHandlingAndLoop();      // Event Handling 및 Loop

								 // 에러 없이 끝났을 경우 0을 리턴함
	return 0;
}