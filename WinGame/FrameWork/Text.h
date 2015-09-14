#ifndef __TEXT_H__
#define __TEXT_H__

#include <string>
#include "define.h"
#include "DeviceManager.h"
#include "Transformable.h"

US_FRAMEWORK

class Text : public Transformable
{
public:
	/*
	Tạo text với font trên hệ thống máy
		@fontName: tên font, vì là kiểu LPCWSTR nên thêm L đằng trước (vd: L"Arial")
		@text: chữ muốn ghi
		@x, y: tọa độ để vẽ
	Sau khi tạo gọi hàm draw() để vẽ.
	*/
	Text(LPCWSTR fontName, string text, float x = 0, float y = 0, int fontSize = 21);
	~Text();

	void draw();

	void setText(string text);
	string getText();

	/*
	Set kích thước chữ
	*/
	void setFontHeight(int fontheight);
	int getFontHeight();
	
	/*
	Set độ đậm chữ
		@fontW: có thể sử dụng các mẫu FW_NORMAL, FW_BOLD, FW_LIGHT
	*/
	void setFontWeight(UINT fontW);
	UINT getFontWeight();

	/*
	Set chữ có nghiêng hay không
	*/
	void setItalic(bool i);
	bool isItalic();

	/*
	Set canh lề cho chữ
		@align: DT_LEFT, DT_CENTER, DT_RIGHT, DT_WORDBREAK, v.v..
	*/
	void setTextAlign(DWORD align);
	DWORD getTextAlign();

	/*
	Set màu cho chữ
	*/
	void setColor(D3DCOLOR color);
	D3DCOLOR getColor();

private:
	ID3DXFont *_font;
	LPCWSTR _fontName;
	string _text;

	int _fontHeight;
	UINT _fontWeight;
	bool _italic;
	DWORD _textAlign;					//DT_LEFT | DT_WORDBREAK | DT_CENTER | DT_RIGHT ...
	D3DCOLOR _color;

	RECT _textRect;

	void updateFont();
};

#endif // !__TEXT_H__
