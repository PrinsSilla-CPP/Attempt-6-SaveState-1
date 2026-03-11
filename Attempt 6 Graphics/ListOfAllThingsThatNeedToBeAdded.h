//Semantic Versioning - Net als oude minecraft teehee woohoo. 

//zoek op Vietas formulas voor optimalisatie

//To-do voor t project
	//CSE PID tuner														(Alpha +)
		//root locus
		//plotter
		//step functie
	//3D multivariable plotter met de volgende mogelijkheden			(alpha content)
		//Impliciete functies											(Alpha)
		//Curl en Divergentie, Gradient									(Alpha)
		//Diff eq solver voor partiele en ordinaire functies			(Alpha +)
		//Functies gelijkzetten											(Alpha +)
		//Discrete functies (basis)										(Alpha)
		//Discrete functies (De rest)									(Alpha +)
		// 
		//doorsneden en projecties										(Alpha +)
	//Lineaire algebra en hogere dimensionale functies					(Alpha +)
		//Jacobian
		//Tensoren (orde 3 en hoger ook dus)
	//fluid simulator													(Beta +)
		//Input van domeinen 
		//Navier stokes whatevs
		//2D
		//3D (Beta+)
	//bijbehorend CAD programmaatje met basisfuncties					(Beta)
		//STL files lezen
		//Letterlijk alleen de basisfuncties
		//STL files maken
	//Topological optimizer van vormen									(Beta)
		//FEA krachtberekenaar
		//automatische vormgever
	//LOD and Optimisations												(Beta)
	
	//Rand functie patroonvinder (van verschillende fucnties - niet van elke ofz, gewoon iets om dus patronen te zien
	//Easter eggs
		//HELPER EASTER EGGS! 
		//Ramones

//Everyone who wants a version when it comes out.
//Hanshu the GOAT
//Haochen Liao (Chris) 
//Sam
//Dave
//Franklin
//Qy 
//Ramon
//Cody (Cody van je werk Smulders. yk.)
//Stefan (STEFANOVICH GAY MING) 
//Tom(Onze tommywommy uwu)
//Luca en Danyla
// 
//Joan maljers(ICT PROJECTWEEK gozer die super chill is dus hier is de remembrance)
//AIVD kerstpuzzel bekijke want blijkbaar is die shit luek lmao
//Je krijgt een certificaat voor het oplossen van de hoofdpuzzel

//Zorg dat het programma portable is naar .app.image  



//press Windows Key + R, type resmon



	//Is dit af dan gaan we naar OpenGL4.3
	//3D multivariable plotter met de volgende mogelijkheden
		//Impliciete functies
		//Curl en Divergentie
		//Diff eq solver voor partiele en ordinaire functies
		//Functies gelijkzetten
		//discrete functies

	//BIj conversie naar linux -> verander alle frontend naar QT uwu.

	//Laatste paar kleine challenges.
	//Spinning Cube
#pragma once
#include <cmath>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glu32.lib")

static float GlobalAngleCube = 0.0f;
class SpinningCubeVertex;

struct Vec3 {
	float x;
	float y;
	float z;
};

constexpr Vec3 Vertices[8] = {
		{-1,-1,-1}, //A or 0
		{1,-1,-1}, //B or 1
		{1,-1,1}, //C or 2
		{-1,-1,1}, //D or 3
		{-1,1,-1}, // E or 4
		{1,1,-1}, // F or 5
		{1,1,1}, //G or 6
		{-1,1,1} // H 0r 7
};

constexpr int faces[6][4] = {
	{0,1,2,3}, //ABCD bottom
	{4,5,6,7}, //EFGH top


	{0,1,5,4}, //ABEF front{0,1,5,4}


	{2,3,7,6}, //DCGH back
	{0,3,7,4}, //ADHE left
	{1,2,6,5}  //BCGF right
};


//void SpawnSpinningCube(float x, float y, float z);


class SpinningCubeVertex {
public:
	
	float AngleRadianCube{};
	
	

	Vec3 CoordinatesCube{ 0,0,0 };
	
	
	
	void DrawCube() const {

		float c = cosf(GlobalAngleCube);
		float s = sinf(GlobalAngleCube);
		Vec3 Transformed[8];

		for (int i = 0; i < 8; i++) {
			Vec3 v = Vertices[i];

			v = RotateY(v, c, s);

			Transformed[i] = v;
		}

		for (int i = 0; i < 6; i++) {
			Vec3& a = Transformed[faces[i][0]];
			Vec3& b = Transformed[faces[i][1]];
			Vec3& c = Transformed[faces[i][2]];
			Vec3& d = Transformed[faces[i][3]];
			
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex3f(d.x + CoordinatesCube.x, d.y + CoordinatesCube.y, d.z + CoordinatesCube.z);
			glColor3f(0.0f, 1.0f, 1.0f);
			glVertex3f(a.x + CoordinatesCube.x, a.y + CoordinatesCube.y, a.z + CoordinatesCube.z);

			glColor3f(1.0f, 0.0f, 1.0f);
			glVertex3f(b.x + CoordinatesCube.x, b.y + CoordinatesCube.y, b.z + CoordinatesCube.z);//A
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(c.x + CoordinatesCube.x, c.y + CoordinatesCube.y, c.z + CoordinatesCube.z);
			
			

		}
	}

	static void UpdateGlobalAngle() {
		GlobalAngleCube += 0.011f;
	}

	static Vec3 RotateY(const Vec3& v, float c, float s) {
		
		
		
		return {
		v.x * c + v.z * s,
		v.y,
		-v.x * s + v.z * c
		};

	}


};

//void SpawnSpinningCube(float x, float y, float z) {



//}