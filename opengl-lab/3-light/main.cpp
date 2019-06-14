#include <iostream>
#include <windows.h>

#include <memory>
#include <vector>

#include <cmath>
namespace std {
	template <typename T> int sign(T a) {
    	return (T(0) < a) - (a < T(0));
	}
}

#include <gl/gl.h>
#include <gl/freeglut.h>

// #################################### Event machine #################################### 

void EventMachine_MouseWrapper(int btn, int state, int x, int y);
void EventMachine_MotionWrapper(GLsizei x, GLsizei y);

struct EventMachine {
	
	static EventMachine *ref;

	int mouseButtonStatus = 0;
	int mousePosDelta[2];
	int mousePos[2];

	EventMachine() {
		ref = this;
	}

	void Init() {
		glutMouseFunc(EventMachine_MouseWrapper);
		glutMotionFunc(EventMachine_MotionWrapper);
	}

	bool IsKept_LeftButton() {
		return mouseButtonStatus == 1;
	}

	bool IsKept_RightButton() {
		return mouseButtonStatus == 2;
	}

	bool IsKept_AnyButton() {
		return mouseButtonStatus != 0;
	}

	int GetMouseDeltaPosX() {
		return mousePosDelta[0];
	}

	int GetMouseDeltaPosY() {
		return mousePosDelta[1];
	}

	static EventMachine& Ref() {
		return *ref;
	}
	static EventMachine* Ptr() {
		return ref;
	}

	void MouseCallback(int btn, int state, int x, int y) {
		if (state == GLUT_DOWN) {

			mousePos[0] = x;
			mousePos[1] = y;

			if (btn == GLUT_LEFT_BUTTON) mouseButtonStatus = 1;
			else if (btn == GLUT_RIGHT_BUTTON) mouseButtonStatus = 2;
		}
		else mouseButtonStatus = 0;
	}

	void MotionCallback(GLsizei x, GLsizei y) {

		mousePosDelta[0] = x - mousePos[0];
		mousePosDelta[1] = y - mousePos[1];

		mousePos[0] = x;
		mousePos[1] = y;

		glutPostRedisplay();
	}

};

EventMachine *EventMachine::ref = nullptr;

void EventMachine_MouseWrapper(int btn, int state, int x, int y) {
	EventMachine::Ptr()->MouseCallback(btn, state, x, y);
}

void EventMachine_MotionWrapper(GLsizei x, GLsizei y) {
	EventMachine::Ptr()->MotionCallback(x, y);
}

// ################## Event object ################## 

class EventObject {

	EventMachine& _em;

public:

	EventObject() : _em(EventMachine::Ref()) {}
	EventMachine& EM() {
		return _em;
	}
};

// #################################### Render object #################################### 

struct RenderObject {	
	virtual void Render() = 0;
};

// #################################### Egg #################################### 

template<int N = 20>
struct Egg : public EventObject, RenderObject {
	float tab[N][N][3];
	float normalTab[N][N][3];
	float rgb[3];

	int model = 0;

	Egg() : rgb{} {
		calculateTab();
		calculateNormalTab();
	}

	void SetColor(float r, float g, float b) {
		rgb[0] = r;
		rgb[1] = g;
		rgb[2] = b;
	}

	float FuncX(float u, float v) {
		float t[5] = { -90.0, 225.0, -270.0, 180.0, -45.0 };
		float f = 0.0;
		float u2 = u;
		for (int i = 4; i >= 0; --i) {
			f += t[i] * u2;
			u2 *= u;
		}
		return f * std::cos(3.14159 * v);
	}

	float FuncY(float u, float v) {
		float t[4] = { 160.0, -320.0, 160.0, 0.0 };
		float f = 0.0;
		float u2 = u;
		for (int i = 3; i >= 0; --i) {
			f += t[i] * u2;
			u2 *= u;
		}
		return f;// - 5.0;
	}

	float FuncZ(float u, float v) {
		float t[5] = { -90.0, 225.0, -270.0, 180.0, -45.0 };
		float f = 0.0;
		float u2 = u;
		for (int i = 4; i >= 0; --i) {
			f += t[i] * u2;
			u2 *= u;
		}
		return f * std::sin(3.14159 * v);
	}

	void calculateTab() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				float u = float(i) / (N - 1);
				float v = float(j) / (N - 1);
				tab[i][j][0] = FuncX(u, v); //x(u,v)
				tab[i][j][1] = FuncY(u, v); //y(u,v)
				tab[i][j][2] = FuncZ(u, v); //z(u,v)
			}
		}
	}

	void calculateTU(float *tu, float u, float v) {
		tu[0] = (-450.f * u*u*u*u + 900.f * u*u*u - 810.f * u*u + 360.f * u - 45.f) * std::cos(3.14159f*v);
		tu[1] = (640.f * u*u*u - 960.f * u*u + 320.f * u);
		tu[2] = (-450.f * u*u*u*u + 900.f * u*u*u - 810.f * u*u + 360.f * u - 45.f) * std::sin(3.14159f*v);
		//tu[0] = 1.f;
		//tu[1] = 1.f;
		//tu[2] = 1.f;
	}

	void calculateTV(float *tv, float u, float v) {
		tv[0] = 3.14159*(90.f * u*u*u*u*u - 225.f * u*u*u*u + 270.f * u*u*u - 180.f * u*u + 45.f * u) * std::sin(3.14159f*v);
		tv[1] = 0.f;
		tv[2] = -3.14159*(90.f * u*u*u*u*u - 225.f * u*u*u*u + 270.f * u*u*u - 180.f * u*u + 45.f * u) * std::cos(3.14159f*v);
		//tv[0] = 1.f;
		//tv[1] = 1.f;
		//tv[2] = 1.f;
	}

	void calculateNormalTab() {
		float tu[3]{}, tv[3]{};

		// for (int i = 0; i < N; ++i) {
		// 	for (int j = 0; j < N; ++j) {

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {

				float u = float(i) / (N - 1);
				float v = float(j) / (N - 1);

				calculateTU(tu, u, v);
				calculateTV(tv, u, v);

				float *p = normalTab[i][j];

				p[0] = tu[1] * tv[2] - tu[2] * tv[1];
				p[1] = tu[2] * tv[0] - tu[0] * tv[2];
				p[2] = tu[0] * tv[1] - tu[1] * tv[0];

				float len = std::sqrt(p[0] * p[0]  +  p[1] * p[1]  +  p[2] * p[2]);

				// if(i == 0 || i == N-1) {
				// 	p[1] = 1.f;
				// }
				// else if(i == N/2) {
				// 	p[1] = -1.f;
				// }		

				//std::cout<<len<<' ';

				if(i == 0 || i == N-1) {
					p[1] = -1.f;
				}
				else {		
					if (u >= 0.5f) {
						len *= -1.f;
					}							
					p[0] /= len;
					p[1] /= len;
					p[2] /= len;
				}

				// if (i == 0) {
				// 	p[0] = 0.f;
				// 	p[1] /= len;
				// 	// p[1] = -1.f + 2.f*(u>=0.5f);
				// 	p[2] = 0.f;
				// 	std::cout<<p[1]<<' ';
				// }
				// else {	
				// 	if (u >= 0.5f) {
				// 		len *= -1.f;
				// 	}				
				// 	p[0] /= len;
				// 	p[1] /= len;
				// 	p[2] /= len;
				// }

				
			}
		}
	}

	/*void putColorPoint(int x, int y) {
		glColor3f(tab[x][y][0] * 20.0, tab[x][y][1] * 20.0, tab[x][y][2] * 20.0);
		glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
	}*/

	void putColorPoint(int x, int y) {
		glColor3f(rgb[0], rgb[1], rgb[2]);
		putNormalVertex(x, y);
		putVertex(x, y);
	}

	void putVertex(int x, int y) {
		glVertex3f(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
	}

	void putNormalVertex(int x, int y) {
		glNormal3f(normalTab[x][y][0], normalTab[x][y][1], normalTab[x][y][2]);
	}

	void putNormalVec(int x, int y) {
		glVertex3f(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
		glVertex3f(
			tab[x][y][0] + normalTab[x][y][0],
			tab[x][y][1] + normalTab[x][y][1],
			tab[x][y][2] + normalTab[x][y][2]
		);
	}

	void Render() {
		//static float r = 0.0;
		glTranslated(0.0, -4.0, 0.0);
		//glRotated(-30.0, 1.0, 0.0, 0.0);
		//glRotated(r, 0.0, 1.0, 0.0);
		//r += 10.0;

		if (model == 1) {
			glBegin(GL_POINTS);
			glColor3f(1.0, 1.0, 0.0);
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					putVertex(i, j);
				}
			}
		}
		else if (model == 2) {
			glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N - 1; ++j) {

					int x = i, y = j;
					putVertex(x, y);
					x = (i + 1) % N;
					putVertex(x, y);

					x = i; y = j;
					putVertex(x, y);
					y = (j + 1) % N;
					putVertex(x, y);

					x = i; y = j;
					putVertex(x, y);
					x = (i + 1) % N;
					y = (j + 1) % N;
					putVertex(x, y);

				}
			}
		}
		else {
			glBegin(GL_TRIANGLES);
			glColor3f(0.0, 0.0, 0.0);
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N - 1; ++j) {
					int x = i, y = j;
					putColorPoint(x, y);
					x = (i + 1) % N;
					putColorPoint(x, y);
					y = (j + 1) % N;
					putColorPoint(x, y);

					x = i; y = j;
					putColorPoint(x, y);
					y = (j + 1) % N;
					putColorPoint(x, y);
					x = (i + 1) % N;
					putColorPoint(x, y);
				}
			}
			glEnd();
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N - 1; ++j) {
					int x = i, y = j;
					putNormalVec(x, y);
				}
			}
		}
		glEnd();
	}
};

// #################################### Axes #################################### 

struct Axes : public RenderObject {

	//typedef float vec3[3];
	using vec3 = float[3];

	vec3 x_min, x_max;
	vec3 y_min, y_max;
	vec3 z_min, z_max;

	Axes() :
		x_min{ -5.0, 0.0, 0.0 }, x_max{ 5.0, 0.0, 0.0 },
		y_min{ 0.0, -5.0, 0.0 }, y_max{ 0.0,  5.0, 0.0 }, 
		z_min{ 0.0, 0.0, -5.0 }, z_max{ 0.0, 0.0,  5.0 } 
	{

	}

	void Render() {
		glColor3f(1.0f, 0.0f, 0.0f); 
		glBegin(GL_LINES); 
		glVertex3fv(x_min);
		glVertex3fv(x_max);
		glEnd();

		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);

		glVertex3fv(y_min);
		glVertex3fv(y_max);
		glEnd();

		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);

		glVertex3fv(z_min);
		glVertex3fv(z_max);
		glEnd();
	}

};

// #################################### Camera #################################### 

struct Camera : public EventObject {

	static Camera *ptr;

	float angleX = 0;
	float angleY = 3.14159*1000.0;
	float zoom = 20.0;
	float top = 1.0;
	float dpi = 0.05;

	GLfloat viewer[3];
	GLfloat pix2angle;

	Camera() : viewer{ 0.0, 0.0, 10.0 }
	{
		ptr = this;
	}

	static Camera& Ref() {
		return *ptr;
	}

	static Camera* Ptr() {
		return ptr;
	}

	void ChangeScreenSizeCallback(GLsizei horizontal, GLsizei vertical) {
		pix2angle = 360.f / horizontal;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, 1.0, 1.0, 30.0);

		if (horizontal <= vertical) 
			glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
		else 
			glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void UpdatePosition() {

		float aX = angleX;
		float aY = angleY;
		viewer[0] = zoom * std::cos(aX) * std::cos(aY);
		viewer[1] = zoom * std::sin(aY);
		viewer[2] = zoom * std::sin(aX) * std::cos(aY);
	}

	void UpdateAngle() {		
		angleX += EM().GetMouseDeltaPosX() * pix2angle * dpi;
		angleY += EM().GetMouseDeltaPosY() * pix2angle * dpi;

		top = 1.0 - 2.0 * (std::fmod(angleY + 3.14159 * 0.5, 3.14159*2.0) > 3.14159);

		//float aY = std::fmod(angleY + 3.14159 * 0.5, 3.14159*2.0);

		//std::cout << angleY << '\n';
	}

	void UpdateZoom() {

		zoom += EM().GetMouseDeltaPosY() * pix2angle * dpi;
		if (zoom < 0.05)
			zoom = 0.05;
		else if (zoom > 20.0) {
			zoom = 20.0;
		}
	}

	void Update() {
		if(EM().IsKept_LeftButton()) {
			UpdateAngle();
		}
		else if(EM().IsKept_RightButton()) {
			UpdateZoom();
		}
		UpdatePosition();

		gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0, top, 0);
	}

};

Camera *Camera::ptr = nullptr; 

// #################################### Light #################################### 

struct Light {

	int idx = 0;
	int glIdx = GL_LIGHT0;

	GLfloat light_position[4], light_ambient[4], light_diffuse[4], light_specular[4];
	GLfloat att_constant, att_linear, att_quadratic;

	Light(int idx ) : idx(idx), glIdx(GL_LIGHT0 + idx),
		light_position{ 30.0, 0.0, 10.0, 1.0 },
		light_ambient{ 0.0, 0.0, 0.0, 1.0 },
		light_diffuse{ 0.8, 0.5, 0.0, 1.0 },
		light_specular{ 0.9, 0.4, 0.0, 1.0 },

		att_constant{ 0.1 },
		att_linear{ 0.01 },
		att_quadratic{ 0.003 } 
	{

	}

	void Init() {
		glLightfv(glIdx, GL_AMBIENT, light_ambient);
		glLightfv(glIdx, GL_DIFFUSE, light_diffuse);
		glLightfv(glIdx, GL_SPECULAR, light_specular);
		glLightfv(glIdx, GL_POSITION, light_position);

		glLightf(glIdx, GL_CONSTANT_ATTENUATION, att_constant);
		glLightf(glIdx, GL_LINEAR_ATTENUATION, att_linear);
		glLightf(glIdx, GL_QUADRATIC_ATTENUATION, att_quadratic);
	}

	Light& SetCLQ(float constant, float linear, float quadratic) {
		att_constant = constant;
		att_linear = linear;
		att_quadratic = quadratic;
		glLightf(glIdx, GL_CONSTANT_ATTENUATION, att_constant);
		glLightf(glIdx, GL_LINEAR_ATTENUATION, att_linear);
		glLightf(glIdx, GL_QUADRATIC_ATTENUATION, att_quadratic);
		return *this;
	}

	Light& SetPosition(float x, float y, float z) {
		light_position[0] = x;
		light_position[1] = y;
		light_position[2] = z;
		glLightfv(glIdx, GL_POSITION, light_position);
		return *this;
	}
	Light& SetAmbient(float x, float y, float z) {
		light_ambient[0] = x;
		light_ambient[1] = y;
		light_ambient[2] = z;
		glLightfv(glIdx, GL_AMBIENT, light_ambient);
		return *this;
	}
	Light& SetDiffuse(float x, float y, float z) {
		light_diffuse[0] = x;
		light_diffuse[1] = y;
		light_diffuse[2] = z;
		glLightfv(glIdx, GL_DIFFUSE, light_diffuse);
		return *this;
	}
	Light& SetSpecular(float x, float y, float z) {
		light_specular[0] = x;
		light_specular[1] = y;
		light_specular[2] = z;
		glLightfv(glIdx, GL_SPECULAR, light_specular);
		return *this;
	}
};

struct LightManager {

	GLfloat mat_ambient[4];
	GLfloat mat_diffuse[4];
	GLfloat mat_specular[4];
	GLfloat mat_shininess;

	std::vector<std::unique_ptr<Light>> lights;

	LightManager() :		
		mat_ambient{ 0.2, 0.2, 0.2, 1.0 },
		mat_diffuse{ 1.0, 1.0, 1.0, 1.0 },
		mat_specular{ 1.0, 1.0, 1.0, 1.0 },
		mat_shininess{ 100.0 }
	{

	}

	void Init() {

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

		glShadeModel(GL_SMOOTH);

		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	Light* GetLight(int idx) {
		return lights[idx].get();
	}

	Light* CreateLight() {

		glEnable(GL_LIGHT0+lights.size());

		lights.emplace_back(new Light(lights.size()))->Init();

		return lights.back().get();
	}
};

// #################################### World ####################################

void World_RenderWrapper();
void World_ChangeScreenSizeWrapper(GLsizei x, GLsizei y);

struct World {

	static World *ptr;

	EventMachine em;
	Camera cam;
	LightManager lm;

	std::vector<std::unique_ptr<RenderObject>> objects;

	World() {
		ptr = this;
	}

	static World* Ptr() {
		return ptr;
	}

	static World& Ref() {
		return *ptr;
	}

	template<typename T>
	T* AddObject(T *newObject) {
		objects.emplace_back(newObject).get();
		return newObject;
	}

	template<typename T, typename ...Ts>
	T* AddObject(Ts... args) {
		T* newObject = new T(args...);
		objects.emplace_back(newObject);
		return newObject;
	}

	void Render() {		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		cam.Update();

		for(auto& obj : objects) {
			glColor3f(1.0f, 1.0f, 1.0f);
			obj->Render();
		}


		//glutWireTeapot(3.0);
		//glutSolidTeapot(3.0);
		//lightUpdate(); 

		glFlush();
		glutSwapBuffers();
	}

	void Init() {
		em.Init();
		lm.Init();

		lm.CreateLight()->SetPosition(-20.0, 0.0, 10.0).SetDiffuse(0.5, 0.0, 0.0).SetSpecular(1.0, 0.2, 0.1).SetCLQ(0.001, 0.05, 0.0001);
		lm.CreateLight()->SetPosition(20.0, 0.0, 10.0).SetDiffuse(0.0, 0.0, 0.5).SetSpecular(0.2, 0.2, 1.0).SetCLQ(0.001, 0.05, 0.0001);

		AddObject<Axes>();
		AddObject<Egg<50>>()->SetColor(1.0, 0.0, 0.0);
	}

	void Run() {

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(300, 300);

		glutCreateWindow("Rzutowanie perspektywiczne");

		glutDisplayFunc(World_RenderWrapper);
		glutReshapeFunc(World_ChangeScreenSizeWrapper);                   

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);

		Init();

		glutMainLoop();
	}

};
World *World::ptr = nullptr;

void World_RenderWrapper() {
	World::Ptr()->Render();
}

void World_ChangeScreenSizeWrapper(GLsizei horizontal, GLsizei vertical) {
	Camera::Ptr()->ChangeScreenSizeCallback(horizontal, vertical);
}

/*************************************************************************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	World app;
	app.Run();

}

/*************************************************************************************/