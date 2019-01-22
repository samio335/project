#pragma once

#include "Vector.h"

class MATRIX
{
public:
	float m[4][4];
	float ele[3][3];

	MATRIX(float mIn[4][4] = NULL)
	{
		if (mIn)
			set(mIn);
		else
			memset(m, 0, sizeof(float) * 16);
	}

	void set(float glMatrix[4][4])
	{
		memcpy(m, glMatrix, sizeof(float) * 16);
	}

	float* matrix()
	{
		return (float*)m;
	}

	MATRIX operator*(MATRIX m) {
		MATRIX result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.ele[i][j] = 0;

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					result.ele[i][j] += ele[i][k] * m.ele[k][j];

		return result;
	}

	VECTOR3D operator*(VECTOR3D v) {
		VECTOR3D result;

		result.x = ele[0][0] * v.x + ele[0][1] * v.y + ele[0][2] * v.z;
		result.y = ele[1][0] * v.x + ele[1][1] * v.y + ele[1][2] * v.z;
		result.z = ele[2][0] * v.x + ele[2][1] * v.y + ele[2][2] * v.z;

		return result;
	}
};