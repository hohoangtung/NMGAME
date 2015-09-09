

#include "Graphics.h"
US_FRAMEWORK

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}


Graphics::Graphics(HINSTANCE hInstance, LPWSTR wcName, int width, int height, int fps, int isFullScreen)
{

	this->_hInstance	= hInstance;
	this->_wcName		= wcName;

	this->_isFullScreen = isFullScreen;

	this->_width = width;
	this->_height = height;
	this->_fps = fps;
}

void Graphics::initWindow()
{

/* --- Init Window class extra --- */
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);//

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = this->_hInstance;//

	wc.lpfnWndProc = Graphics::winProc;//
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->_wcName;//
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

/* --- Init window --- */
	DWORD style;
	if (this->_isFullScreen)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
	else
		style = WS_OVERLAPPEDWINDOW;

	this->_hWnd = CreateWindow(
		this->_wcName,
		this->_wcName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		this->_width,
		this->_height,
		NULL,
		NULL,
		this->_hInstance,
		NULL);
	if (this->_hWnd == NULL)
	{
		throw;
	}

/* --- Show and Update --- */
	ShowWindow(this->_hWnd, SW_SHOWNORMAL);
	UpdateWindow(this->_hWnd);
}


HRESULT CALLBACK Graphics::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}


int Graphics::isFullScreen(){
	return _isFullScreen;
}

int Graphics::getWidth(){
	return _width;
}

int Graphics::getHeight(){
	return _height;
}

int Graphics::getFrameRate(){
	return _fps;
}

HWND Graphics::getWnd()
{
	return _hWnd;
}

HINSTANCE Graphics::gethInstance()
{
	return this->_hInstance;
}