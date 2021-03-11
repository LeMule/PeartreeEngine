#ifndef __PEAR_MATH__
#define __PEAR_MATH__

namespace PearTree
{
	static constexpr float IdentityMatrix[4][4] = {
		{ 1.0, 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 }
	};

	typedef float Vector2[2];
	typedef float Vector3[3];
	typedef float Vector4[4];

	template <class T>
	static void AddVectors(T& vec1, T& vec2, T& vec3)
	{
		if (sizeof(vec1) != sizeof(vec2) != sizeof(vec3))
			return;
		const auto vecSize = sizeof(vec3) / sizeof(float);

		auto resultVec;
	}
}

#endif //__PEAR_3D__
