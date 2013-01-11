#pragma once

using std::sqrt;
using std::pow;
using std::abs;

class AudioLine
{
public:
	AudioLine() {};
	AudioLine(Windows::Foundation::Point initial)
	{
		start.x = initial.X;
		start.y = initial.Y;
	}

	D2D1_POINT_2F start;
	D2D1_POINT_2F end;

	double ComputeSlope()
	{
		auto slope = abs(end.y - start.y)/abs(end.x - start.x);
		return slope;
	}

	double Length()
	{
		auto length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
		return length;
	}
};
