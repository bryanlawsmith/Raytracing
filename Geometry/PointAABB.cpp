#include "PointAABB.h"

namespace GeometryLib
{
	bool PointInsideAABB(const vector4& point, const vector4& center, const vector4& halfWidths)
	{
		vector4 pointRelative;
		vector4_sub(point, center, pointRelative);
		
		if (fabs(pointRelative.extractX()) > halfWidths.extractX() ||
			fabs(pointRelative.extractY()) > halfWidths.extractY() ||
			fabs(pointRelative.extractZ()) > halfWidths.extractZ())
			return false;
		
		return true;
	}


}