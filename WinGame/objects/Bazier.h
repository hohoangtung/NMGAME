

#ifndef __BAZIER_H__
#define __BAZIER_H__

// ref: seminar nhóm Bùi Đình Lộc Thọ.
// http://www.mediafire.com/download/500rw9lur9acj9s/%5BBezierCurve%5D+-+13520844+-+Bu%CC%80i+%C4%90i%CC%80nh+L%C3%B4%CC%A3c+Tho%CC%A3+%28Tr%C6%B0%C6%A1%CC%89ng+nho%CC%81m%29.rar

#include <vector>
using namespace std;

#include "../FrameWork/define.h"

class Bazier
{
public:
	Bazier();
	~Bazier();
	GVector2 getPositionInBazier(float ratio);
	void insertAnchor(GVector2 point);
	GVector2& getPointAt(int index);
	void updatePoint(int index, GVector2 value);
	static GVector2 getPositionInBazier(float ratio, GVector2 anchorpoints[], int count);
private:
	vector<GVector2> _anchorPoints;

};
#endif // !__BAZIER_H__
