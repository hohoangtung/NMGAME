#include "Text.h"

Text::Text(LPCWSTR fontName, string text, float x, float y, int fontSize)
{
	_fontName = fontName;
	_text = text;
	_position.x = x;
	_position.y = y;

	_fontWeight = FW_NORMAL;
	_italic = false;
	_fontHeight = fontSize;
	_textAlign = DT_LEFT | DT_NOCLIP;
	_color = 0xFFFFFFFF;

	this->updateFont();
}

Text::~Text()
{
	_font->Release();
	SAFE_DELETE(_font);
}

void Text::draw()
{
	_textRect.left = _position.x;
	_textRect.top = WINDOW_HEIGHT - _position.y;

	_font->DrawTextA(
		NULL,
		_text.c_str(),
		-1,												// số chữ để vẽ ra, -1 là vẽ hết
		&_textRect,
		_textAlign,
		_color
		);
}

void Text::setText(string text)
{
	if (_text.compare(text) != 0)
		_text = text;
}

string Text::getText()
{
	return _text;
}

void Text::setFontHeight(int fontheight)
{
	if (fontheight != _fontHeight)
		_fontHeight = fontheight;

	this->updateFont();
}

int Text::getFontHeight()
{
	return _fontHeight;
}

void Text::setFontWeight(UINT fontW)
{
	if (fontW != _fontWeight)
		_fontWeight = fontW;

	this->updateFont();
}

UINT Text::getFontWeight()
{
	return _fontWeight;
}

void Text::setItalic(bool i)
{
	if (_italic != i)
		_italic = i;

	this->updateFont();
}

bool Text::isItalic()
{
	return _italic;
}

void Text::setTextAlign(DWORD align)
{
	if (align != _textAlign)
		_textAlign = align;

	this->updateFont();
}

DWORD Text::getTextAlign()
{
	return _textAlign;
}

void Text::setColor(D3DCOLOR color)
{
	if (_color != color)
		_color = color;

	this->updateFont();
}

D3DCOLOR Text::getColor()
{
	return _color;
}

void Text::updateFont()
{
	if (_font)
	{
		_font->Release();
		_font = nullptr;
	}

	D3DXCreateFont(
		DeviceManager::getInstance()->getDevice(),		// device
		_fontHeight,									// font height
		0,												// font width
		_fontWeight,									// font weight
		1,												//
		_italic,										// italic
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		_fontName,										// font face
		&_font											// pointer font
		);
}
