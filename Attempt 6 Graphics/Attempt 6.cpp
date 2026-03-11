#include <Windows.h>
#include <vector>
#include "resource.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cmath>
#include <chrono> 
//#include "stb_image.h" yeah for if you want to load images. Anyway.... this is for later you moron.
#include "Lexer.h"
#include "Parser.h"
#include "TokenTypeList.h"
#include "Type_checker.h"
#include "CodeGen.h"
#include "ListOfAllThingsThatNeedToBeAdded.h"
//misschien het word tijd om GLUT en GLEW te downloaden zodra je je eerste paar grafieken kan plotten jij aarschlog
//Welke mensen komen er op de lijst voor een early release alpha
//Sam wilt een early release beta.
//misschien een functie om STL's te importeren.
//cache utilization 

//TEMP INCLUDE LIBRARIES

static bool PromptInjectVar = 0;

constexpr auto MAX_EDIT_TEXT = 1024;

GLuint TextureIdentification;

static POINT pt;

static inline float Degree2RadiansFloat(float g) { return g * 3.1415926535f / 180.0f; }

RECT rect;

std::vector<SpinningCubeVertex> cubes;


struct KeyboardInputs {
	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;
	bool keyP = false;
	bool keySpace = false;
	bool keyShift = false;
};
KeyboardInputs g_keys;

static bool Pause = 0; //return of extremily convoluted names.jpg
static bool WasPauseButtonGettingPressed = 0; 
GLdouble GLlookEyeX = 0; GLdouble GLlookEyeY = 0; GLdouble GLlookEyeZ = 0;
GLdouble GLEyeLookAtX = 0; GLdouble GLEyeLookAtY = 0;

float moveX = 0.0f; float moveY = 0.0f; float MoveZ = 0.0f;
const float SPEED = 5.0f; const float SpeedDT = 0.064f;

static float yaw = -90.0f;
static float pitch = 0.0f;
float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f;
bool firstMouse = true;
static float HerlaadDieFockingSceneGodverdomme = 0;

void SetupPixelFormat(HDC HandlerDeviceContext);
void renderScene();
void ControlsMovement();

void StartAxes();
void StartTriangle();
void PrismStartTriangle();

char XYMuisPositieStringConverterBuffer[128]; //ja het kan nog langer.dus waffel dicht ouwe. - ok ik heb hier serieus spijt van at times
char DebugPauseButtonString[64];
int xPosMouse = 0;
int yPosMouse = 0;
int xVerschilMouse = 0;
int yVerschilMouse = 0;
static int xPosMouseOud = 0;
static int yPosMouseOud = 0;

#define MAX_LOADSTRING 100

HINSTANCE		handleNaarInstantieOpzet; 
HWND			handleNaarParentWindow;
HWND            glChildWindow;
HDC				HandlerDeviceContext;
HGLRC			handleNaarGLRenderContext;
WCHAR			TitelVanDePagina[MAX_LOADSTRING];
WCHAR			WindowKlasse[MAX_LOADSTRING];
WNDCLASSEX EigenschappenVanDeWindow;
ATOM			MyRegisterClass(HINSTANCE handleNaarInstantie); //ATOM -> Acces TO Memory, goed voor memory leaks dus lol
ATOM			RegisterGLWindowClass(HINSTANCE handleNaarInstantie);
BOOL			InitInstance(HINSTANCE, int);

HWND InputPrompt;
HWND DebugPromptOutput;

DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

void SetupPixelFormat(HDC HandlerDeviceContext);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK GLCHILDWndProc(HWND, UINT, WPARAM, LPARAM);


char OutputStringPrompt[MAX_EDIT_TEXT];

//void HardcodedFunct(Input Slider){
// NIET IMPLICIET
// Eerste functie ontvangt waarden in een vec3(xfloat,yfloat,zfloat)
// BereikY(MinYValFloat,MaxYValFloat)en DomeinX(MinXValFloat,MaxXValFloat) en DomeinZ(MinZValFloat,MaxZValFloat) moeten we als inputwaarden krijgen 
// slider word de Z-as
// for( i == MinXValFloat){
//}pppppp


/*void KeyboardMovementInputs(WPARAM key) {
	switch (key) {
	case 'W':
		GLlookEyeZ = GLlookEyeZ - 1;
		//pitch (Up and down) en yaw (links naar rechts)
		//GLlookEyez = GLlookEyeZ - 
		break;

	case 'S':
		GLlookEyeZ = GLlookEyeZ + 1;

		//GLlookEyeZ = GLlookEyeZ - 0.333 * dirX - 0.333 * dirY - 0.333 * dirZ; Dit werkt alleen op de Z-as dus.
		break;

	case 'A':
		GLlookEyeX = GLlookEyeX - 1;
		break;

	case 'D':
		GLlookEyeX = GLlookEyeX + 1;
		break;

	case 'P':
		if (IsPauseButtonPGettingPressed == 0) {
			IsPauseButtonPGettingPressed = 1;
		}
		else { IsPauseButtonPGettingPressed = 0; }
		sprintf_s(DebugPauseButtonString, "Het nummertje is (%d)\n", IsPauseButtonPGettingPressed);
		OutputDebugStringA(DebugPauseButtonString); //onthoud es ff heeeel goed wat deze commando is - want als je t fout doet kan je gepijpt worden door een babyblender.
		OutputDebugStringA("GEPAUZEERD!\n");
		break;

	case VK_SPACE:
		GLlookEyeY = GLlookEyeY + 1;
		break;

	case VK_SHIFT:
		GLlookEyeY = GLlookEyeY - 1;
		break; //why do these cases catch a break but i can't? god fucking damnit man this shit is tiring.
		//case VK_ESCAPE:
		//	SetFocus(glChildWindow);
	}
} */

static void MOUSEMOVE(HWND hwnd, LPARAM lParameter) {
	if (Pause == 0) {
		int xPosMouse = (int)(short)LOWORD(lParameter);
		int yPosMouse = (int)(short)HIWORD(lParameter);
		//OutputDebugStringA("werkt ie nou?");
		if (firstMouse) {
			xPosMouseOud = xPosMouse;
			yPosMouseOud = yPosMouse;
			firstMouse = false;
		
		}
		GetClientRect(handleNaarParentWindow, &rect);
		//POINT pt;  if anything goes wrong and you get 44 errors, this is the fucker. its about static and dynamic and eclectic and autistic and hyperbolic and hypergolic and euphemistic and whatever tisms you got.

		pt.x = (rect.right - rect.left) / 2;
		pt.y = (rect.bottom - rect.top) / 2;
		int xVerschilMouse = xPosMouse - xPosMouseOud;
		int yVerschilMouse = yPosMouse - yPosMouseOud;
		sprintf_s(XYMuisPositieStringConverterBuffer, "Muis positie: (%d, %d, %d, %d)\n", xPosMouse, yPosMouse, xVerschilMouse, yVerschilMouse);
		//OutputDebugStringA("WM_MOUSE ontvange\n");

		float gevoeligheid = 0.05f;
		yaw += xVerschilMouse * gevoeligheid;
		pitch -= yVerschilMouse * gevoeligheid;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		float yawRadian = Degree2RadiansFloat(yaw), pitchRadian = Degree2RadiansFloat(pitch);
		dirX = cosf(yawRadian) * cosf(pitchRadian); //wil je je muis inverten? zet een minnetje in deze formula.
		dirY = sinf(pitchRadian); //NIET in deze formula. 
		dirZ = sinf(yawRadian) * cosf(pitchRadian);// of in deze formula. 

		ClientToScreen(hwnd, &pt);
		SetCursorPos(pt.x, pt.y); //960, 540 middle screen
		xPosMouseOud = (rect.right - rect.left) / 2;
		yPosMouseOud = (rect.bottom - rect.top) / 2;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE handleNaarInstantie, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdshow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	static auto vorigeFrame = std::chrono::high_resolution_clock::now();

	auto nu = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> delta = nu - vorigeFrame;
	vorigeFrame = nu;

	float tijdSindsLaatsteFrame = delta.count();

	LoadStringW(handleNaarInstantie, IDS_APP_TITLE, TitelVanDePagina, MAX_LOADSTRING); //IDS_APP_TITLE
	LoadStringW(handleNaarInstantie, IDC_WINDOWSKLASSE, WindowKlasse, MAX_LOADSTRING); //IDC_ATTEMPT_SIX
	handleNaarInstantieOpzet = handleNaarInstantie;

	HACCEL hAccelTable = LoadAccelerators(handleNaarInstantie, MAKEINTRESOURCE(IDC_ATTEMPT_SIX));

	if (!MyRegisterClass(handleNaarInstantie)) //voer de myregisterclass uit! en checkt ook nog of ie werkt. Anders gebeurt het volgende
	{
		MessageBox(nullptr, L"Registreren van de window class is gefaald ouwe! Pak een L", L"Fout", MB_OK | MB_ICONERROR); //dit is het volgende
		return FALSE;
	}

	if (!RegisterGLWindowClass(handleNaarInstantie))
	{
		MessageBox(nullptr, L"GLwindow werkt niet gvd", L"Pak een L flikker", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (!InitInstance(handleNaarInstantie,nCmdshow)) //voer de InitieerDeInstantie uit! en checkt ook nog of ie werkt. Anders gebeurt het volgende
	{ 
		MessageBox(nullptr, L"Geen initiatie!", L"Fout", MB_OK | MB_ICONERROR);
		return FALSE;
	} 

	MSG Bericht;

	while (GetMessage(&Bericht, nullptr, 0, 0))
	{ 
		if (!TranslateAccelerator(Bericht.hwnd, hAccelTable, &Bericht))  //handleNaarParentwindow HAccelTable
		{
			TranslateMessage(&Bericht);
			DispatchMessage(&Bericht);
		}
	}
	return (int)Bericht.wParam;
}

ATOM MyRegisterClass(HINSTANCE handleNaarInstantie)
{
	WNDCLASSEXW EigenschappenVanDeWindow; //maak de classe/struct aan
	
	EigenschappenVanDeWindow.cbSize = sizeof(WNDCLASSEX);
	EigenschappenVanDeWindow.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	EigenschappenVanDeWindow.lpfnWndProc = WndProc;
	EigenschappenVanDeWindow.hInstance = handleNaarInstantie;
	EigenschappenVanDeWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	EigenschappenVanDeWindow.lpszClassName = WindowKlasse;
	EigenschappenVanDeWindow.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1); //achtergrond kleur
	EigenschappenVanDeWindow.cbClsExtra = 0;
	EigenschappenVanDeWindow.cbWndExtra = 0;
	EigenschappenVanDeWindow.hIcon = LoadIcon(handleNaarInstantie, MAKEINTRESOURCE(IDI_ATTEMPT_SIX)); // of eigen icoon 
	EigenschappenVanDeWindow.lpszMenuName = MAKEINTRESOURCE(IDC_ATTEMPT_SIX); // of MAKEINTRESOURCE je menu id
	EigenschappenVanDeWindow.hIconSm = LoadIcon(handleNaarInstantie, MAKEINTRESOURCE(IDI_ATTEMPT_SIX)); 

	return RegisterClassExW(&EigenschappenVanDeWindow); //geeft al deze eigenschappen terug want anders zeggen we well wat, maar word er niks mee gedaan
}

ATOM RegisterGLWindowClass(HINSTANCE handleNaarInstantie)
{
	WNDCLASSEXW glWindowClass;

	glWindowClass.cbSize = sizeof(WNDCLASSEX);
	glWindowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	glWindowClass.lpfnWndProc = GLCHILDWndProc;
	glWindowClass.hInstance = handleNaarInstantie;
	glWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	glWindowClass.lpszClassName = L"OpenGLWindow";
	glWindowClass.cbClsExtra = 0;
	glWindowClass.cbWndExtra = 0;
	glWindowClass.hIcon = NULL;
	glWindowClass.lpszMenuName = NULL;
	glWindowClass.hIconSm = NULL;
	glWindowClass.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);

	ATOM WerktIeNouOfNie = RegisterClassExW(&glWindowClass);
	if (!WerktIeNouOfNie) {
		DWORD err = GetLastError(); //dit ook
		char msg[MAX_EDIT_TEXT];
		sprintf_s(msg, "RegisterGLwindowClass heeft t verpest/is verpest: %lu\n", err); // dit ook
		OutputDebugStringA(msg);

	}
	return WerktIeNouOfNie;


	//return RegisterClassExW(&glWindowClass);
}

BOOL InitInstance(HINSTANCE handleNaarInstantieOpzet, int nCmdShow)
{
	handleNaarParentWindow = CreateWindowW(WindowKlasse, TitelVanDePagina, windowStyle, CW_USEDEFAULT,
		CW_USEDEFAULT, 1600, 1100, nullptr,
		nullptr, handleNaarInstantieOpzet, nullptr);
	if (!handleNaarParentWindow)
	{
		OutputDebugStringA("Handle naar Parent Window Geinitializeerd");
		return FALSE;
	}

	ShowWindow(handleNaarParentWindow, nCmdShow);
	UpdateWindow(handleNaarParentWindow);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND handleNaarParentWindow, UINT message, WPARAM wParameter, LPARAM lParameter)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParameter);
		switch (wmId)
		{
		case 1001: {
			MessageBeep(0xFFFFFFFF);

			if (HIWORD(wParameter) == BN_CLICKED) {
				wchar_t buffer[MAX_EDIT_TEXT];
				GetWindowText(InputPrompt, buffer, MAX_EDIT_TEXT);
				OutputDebugStringA("input was: ");
				OutputDebugString(buffer);
				OutputDebugStringA("\n");
				SetWindowText(DebugPromptOutput, buffer);

				char input[64];
				WideCharToMultiByte(CP_ACP, 0, buffer, -1, input, 64, NULL, NULL); //learn wtf this command is and its shit
				if (strcmp(input, "PromptInjectVar = 1") == 0) { //learn strcmp
					PromptInjectVar = true; // fuck this little bugger up its ugly arse.
					SetWindowText(DebugPromptOutput, L"PromptInjectVar = 1 - OK");
				}
			}
		}
			break;
		case 123: 
			//as empty as my lovelife.

			
		
			break;
		case 124:


			break;
		case IDM_EXIT:  //dit is de beschrijving van IDM_EXIT

			DestroyWindow(handleNaarParentWindow);
			break;
		default:
			return DefWindowProc(handleNaarParentWindow, message, wParameter, lParameter);

		}
		break;
	}
	break;
	case WM_CREATE:
	{
		RAWINPUTDEVICE RawInput = {};
		RawInput.usUsagePage = 0x01;
		RawInput.usUsage = 0x06;
		RawInput.dwFlags = RIDEV_INPUTSINK;
		RawInput.hwndTarget = handleNaarParentWindow;

		RegisterRawInputDevices(&RawInput, 1, sizeof(RawInput));

		glChildWindow = CreateWindowEx(0, L"OpenGLWindow", L"", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 10, 10, 925, 675, handleNaarParentWindow, NULL, ((LPCREATESTRUCT)lParameter)->hInstance, NULL);
		HDC glDC = GetDC(glChildWindow);
		SetupPixelFormat(glDC);
		HGLRC glRC = wglCreateContext(glDC);
		wglMakeCurrent(glDC, glRC);
		glEnable(GL_DEPTH_TEST);
		HandlerDeviceContext = glDC;
		handleNaarGLRenderContext = glRC;
		SetTimer(handleNaarParentWindow, 1, 16, NULL);

		CreateWindowEx(0, L"BUTTON", L"Plot!", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			950, 50, 120, 30,   // x, y, breedte, hoogte
			handleNaarParentWindow, (HMENU)1001,       // ID van de knop
			((LPCREATESTRUCT)lParameter)->hInstance, NULL);

		InputPrompt =CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE |ES_AUTOHSCROLL, 970, 140, 550, 300,	// x, y, w, h
			handleNaarParentWindow, (HMENU)123,
			((LPCREATESTRUCT)lParameter)->hInstance, NULL);
		
		

		DebugPromptOutput = CreateWindowEx(WS_EX_CLIENTEDGE, L"Static", L"OutputStringPrompt", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 970, 540, 550, 100, handleNaarParentWindow, (HMENU)124, ((LPCREATESTRUCT)lParameter)->hInstance, NULL);
		//INPUT GRAPHS WILL BE WITH ONE INPUT TEXT BOX! 

		SpinningCubeVertex StartCube;
		StartCube.CoordinatesCube = { 0.0f,0.0f,0.0f };
		cubes.push_back(StartCube);

		
		const int GRID_X = 50;
		const int GRID_Y = 50;
		const int GRID_Z = 50;
		const float SPACING = 5.0f; // space between cubes

		for (int x = 0; x < GRID_X; ++x) {
			for (int y = 0; y < GRID_Y; ++y) {
				for (int z = 0; z < GRID_Z; ++z) {
					SpinningCubeVertex cube;
					cube.CoordinatesCube = { x * SPACING, y * SPACING, z * SPACING };
					cubes.push_back(cube);
				}
			}
		}
		
		//CreateWindowExW(0, L"TestObject",L"Klassenaam Object",WS_CAPTION | WS_VISIBLE | WS_CHILD, 1300, 100, 50,30,handleNaarParentWindow,(HMENU)1002,((LPCREATESTRUCT)lParameter)->hInstance, NULL);

		return 0; //gedeelte van WM_CREATE
	}
	case WM_TIMER:
	{
		ControlsMovement();

		SpinningCubeVertex::UpdateGlobalAngle();
		renderScene();
		SwapBuffers(HandlerDeviceContext);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		SetFocus(glChildWindow);
		break;
	}
	case WM_MOUSEMOVE:
	{
		MOUSEMOVE(handleNaarParentWindow, lParameter);
		break;
	}
	case WM_INPUT: {
		UINT size = 0;
		GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

		BYTE buffer[sizeof(RAWINPUT)];
		if (GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) != size) {
			break;
		}
		RAWINPUT* raw = (RAWINPUT*)buffer;
		
		if (raw->header.dwType == RIM_TYPEKEYBOARD) {
			RAWKEYBOARD& k = raw->data.keyboard;
			bool pressed = !(k.Flags & RI_KEY_BREAK);

			switch (k.VKey) {
			case'W': g_keys.keyW = pressed; break;
			case'A': g_keys.keyA = pressed; break;
			case'S': g_keys.keyS = pressed; break;
			case'D': g_keys.keyD = pressed; break;
			case'P': g_keys.keyP = pressed; break;
			case VK_SPACE: g_keys.keySpace = pressed; break;
			case VK_SHIFT: g_keys.keyShift = pressed; break;

			}
		}
		return 0;
	}
	case WM_KEYDOWN: //dit is de code om de driehoek voorbij te rijden met AWSD 
	{
		//KeyboardMovementInputs(wParameter);

		switch (wParameter) {
			case VK_ESCAPE:
				SetFocus(glChildWindow);
			break;
		}
		return 0;
	} 
	case WM_PAINT: {	
		PAINTSTRUCT PaintStruct;
		//HDC HandlerDeviceContext = BeginPaint(handleNaarParentWindow, &PaintStruct);
		BeginPaint(handleNaarParentWindow, &PaintStruct);
		EndPaint(handleNaarParentWindow, &PaintStruct);
		return 0;
	}

	case WM_DESTROY: {
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(handleNaarGLRenderContext);
		ReleaseDC(handleNaarParentWindow, HandlerDeviceContext);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(handleNaarParentWindow, message, wParameter, lParameter);
	}
	return DefWindowProc(handleNaarParentWindow, message, wParameter, lParameter); //and another one for if it doesnt default lmao
}

LRESULT CALLBACK GLCHILDWndProc(HWND handleNaarChildWindow, UINT message, WPARAM wParameter, LPARAM lParameter)
{
	switch (message)
		{
	case WM_MOUSEMOVE:
		{
		MOUSEMOVE(handleNaarChildWindow, lParameter);
			return 0;
		}
	case WM_LBUTTONDOWN: 
		{
			SetFocus(handleNaarChildWindow);
			return 0;
		}
		case WM_KEYDOWN: //dit is de code om de driehoek voorbij te rijden met AWSD 
		{
			//KeyboardMovementInputs(wParameter);
			//in the case more keyboard inputs are needed. Like in the case of writing 67, where the program will remind you that that type of behaviour is the reason your parents divorced.
			return 0;
		}
		default:
		{
			return DefWindowProc(handleNaarChildWindow, message, wParameter, lParameter);
		}
		return 0;
	}
}

//LRESULT CALLBACK 

void SetupPixelFormat(HDC HandlerDeviceContext) {
	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,    // color depth
	0, 0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	24,8, 0, //24 is de depth buffer en de 8 is de stencil buffer
	PFD_MAIN_PLANE,
	0, 0, 0, 0
	};
	int pixelFormat = ChoosePixelFormat(HandlerDeviceContext, &pfd);
	SetPixelFormat(HandlerDeviceContext, pixelFormat, &pfd);
}

void renderScene() {
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 925, 675);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Stel de viewport en perspectief
	glViewport(0, 0, 925, 675);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 800.0 / 600.0, 0.01, 1000.0);
	// Zet camera en model

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(GLlookEyeX, GLlookEyeY, 5.0f + GLlookEyeZ,   // camera positie
		dirX + GLlookEyeX, dirY + GLlookEyeY, 5.0f + dirZ + GLlookEyeZ,   // kijkpunt
		0, 1, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	glBegin(GL_POINTS); //begin het genereren van een punt - had ik deze functie maar in gesprekken.
	glColor3f(0.0f, 0.0f, 0.0f); //maak punt zwoaart (We discrimineren niet op kleur hoor!)
	glVertex3f(0.0f, 0.0f, 0.0f); // zet she/them op coordinaat x=0, y=0, z=0
	glEnd(); //FATALITY
	//Op naar de assen


	glBegin(GL_LINES);
	StartAxes();
	glEnd();

	//POOIIIIIIINTS
	if (PromptInjectVar == true) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(20.0f);
		glBegin(GL_POINTS);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);
		glEnd();
		glPointSize(1.0f);
	}

	glBegin(GL_TRIANGLES);
	//StartTriangle();
	glEnd();

	glBegin(GL_QUADS);
	//PrismStartTriangle();
	for (auto& cube : cubes)
		cube.DrawCube();

	glEnd();

	glDisable(GL_SCISSOR_TEST);
}

void ControlsMovement() {
	if (g_keys.keyW) {
		GLlookEyeX += dirX * SPEED * SpeedDT;
		GLlookEyeY += dirY * SPEED * SpeedDT;
		GLlookEyeZ += dirZ * SPEED * SpeedDT;
	}
	if (g_keys.keyS) {
		GLlookEyeX -= dirX * SPEED * SpeedDT;
		GLlookEyeY -= dirY * SPEED * SpeedDT;
		GLlookEyeZ -= dirZ * SPEED * SpeedDT;
	}
	if (g_keys.keyD) {
		GLlookEyeX += cosf(Degree2RadiansFloat(yaw + 90.0f)) * SPEED * SpeedDT;
		GLlookEyeZ += sinf(Degree2RadiansFloat(yaw + 90.0f)) * SPEED * SpeedDT;
	}
	if (g_keys.keyA) {
		GLlookEyeX -= cosf(Degree2RadiansFloat(yaw + 90.0f)) * SPEED * SpeedDT;
		GLlookEyeZ -= sinf(Degree2RadiansFloat(yaw + 90.0f)) * SPEED * SpeedDT;
	}
	if (g_keys.keySpace) {
		GLlookEyeY += SPEED * SpeedDT;
	}
	if (g_keys.keyShift) {
		GLlookEyeY -= SPEED * SpeedDT;
	}
	if (g_keys.keyP && !WasPauseButtonGettingPressed) {
		WasPauseButtonGettingPressed = !WasPauseButtonGettingPressed;
		Pause = !Pause;
		OutputDebugStringA("P was pressed \n");
	}

	WasPauseButtonGettingPressed = g_keys.keyP;
}


//ctrl + windows + shift + B - start ik mn opdrivers op
void StartAxes(){
	glColor3f(1.0f, 0.0f, 0.0f); //rood kleur
	glVertex3f(0.0f, 0.0f, 0.0f); //Startpunt van de x-as, de volgende glvertex ding is t eindpunt
	glVertex3f(10.0f, 0.0f, 0.0f);// de eerste as is de X-as en deze is rood
	glColor3f(0.0f, 1.0f, 0.0f); // groen kleur
	glVertex3f(0.0f, 0.0f, 0.0f);//Startpunt van de y-as, de volgende glvertex ding is t eindpunt
	glVertex3f(0.0f, 10.0f, 0.0f);// de tweede as is de Y-as en deze is groeun
	glColor3f(0.0f, 0.0f, 1.0f); // blauwie kleur
	glVertex3f(0.0f, 0.0f, 0.0f);//Startpunt van de z-as, de volgende glvertex ding is t eindpunt
	glVertex3f(0.0f, 0.0f, 10.0f);// de derde as is de Z-as en deze is blauwie!
}
// Begin render — voorbeeld: een driehoek
void StartTriangle() {

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 1.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 1.0f);

}

void PrismStartTriangle() {
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, -1.0f, 1.0f);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
}
