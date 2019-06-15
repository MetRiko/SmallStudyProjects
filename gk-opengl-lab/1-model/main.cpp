/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>


typedef float point3[3];
int model = 1; // 1- punkty, 2- siatka, 3 - wypełnione trójkąty

/*************************************************************************************/

// Funkcja rysująca osie układu współrzędnych

void Axes(void)
{

	point3  x_min = { -1.0, 0.0, 0.0 };
	point3  x_max = { 1.0, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -1.0, 0.0 };
	point3  y_max = { 0.0,  1.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -1.0 };
	point3  z_max = { 0.0, 0.0,  1.0 };
	//  początek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();

}

template<int N = 20>
struct Egg {
	float tab[N][N][3];

	Egg() {
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				float u = float(i) / (N-1);
				float v = float(j) / (N-1);
				tab[i][j][0] = FuncX(u, v); //x(u,v)
				tab[i][j][1] = FuncY(u, v); //y(u,v)
				tab[i][j][2] = FuncZ(u, v); //z(u,v)
			}
		}
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

	void putColorPoint(int x, int y) {
		glColor3f(tab[x][y][0] * 20.0, tab[x][y][1] * 20.0, tab[x][y][2] * 20.0);
		glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
	}

	void RenderModel_Points() {		
		glBegin(GL_POINTS);
		glColor3f(1.0, 1.0, 0.0);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				glVertex3d(tab[i][j][0], tab[i][j][1], tab[i][j][2]);
			}
		}
	}
	
	void RenderModel_Mesh() {		
		glBegin(GL_LINES);
		glColor3f(0.0, 1.0, 0.0);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N-1; ++j) {

				int x = i, y = j;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
				x = (i + 1) % N;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);

				x = i; y = j;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
				y = (j + 1) % N;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);

				x = i; y = j;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
				x = (i + 1) % N; y = (j + 1) % N;
				glVertex3d(tab[x][y][0], tab[x][y][1], tab[x][y][2]);
				
			}
		}
	}
	void RenderModel_Solid() {
		glBegin(GL_TRIANGLES);
		glColor3f(0.0, 0.0, 0.0);
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N-1; ++j) {
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
	}

	void Render() {
		static float r = 0.0;
		glTranslated(0.0, -4.0, 0.0);
		glRotated(-30.0, 1.0, 0.0, 0.0);
		glRotated(r, 0.0, 1.0, 0.0);
		r+=10.0;

		if (model == 1) {
			RenderModel_Points();
		}
		else if (model == 2) {
			RenderModel_Mesh();
		}
		else {
			RenderModel_Solid();
		}
		glEnd();
	}
};
Egg<20> egg;

/*************************************************************************************/

// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba 
// przerysować scenę)

void RenderScene(void)
{
	float ortho = 1.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej
	Axes(); // Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej

	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały
	//glRotated(60.0, 1.0, 1.0, 1.0);  // Obrót o 60 stopni
	//glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
	egg.Render();

	glFlush();
	// Przekazanie poleceń rysujących do wykonania


	glutSwapBuffers();
	//
}

void eventCallback(unsigned char key, int x, int y) {
	if (key == 'p') model = 1;
	if (key == 'w') model = 2;
	if (key == 's') model = 3;

	RenderScene(); // przerysowanie obrazu sceny
}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcący (wypełnienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  określającej proporcję
	// wymiarów okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bieżącej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie współczynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest określenie tak zwanej
	// przestrzeni ograniczającej pozwalającej zachować właściwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczjącej służy funkcja
	// glOrtho(...)            
	if (horizontal <= vertical)

		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else

		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bieżącej
}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(300, 300);

	glutCreateWindow("Układ współrzędnych 3-D");

	glutKeyboardFunc(eventCallback);

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Bedzie ona wywoływana za każdym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/

