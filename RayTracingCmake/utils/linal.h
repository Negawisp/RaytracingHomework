#ifndef __LINAL_H__
#define __LINAL_H__

#include <Eigen/Dense>
#include "matrix.h"
#include "myMath.h"

using namespace Eigen;

Matrix3f mat3fToMatrix3f(const Mat3f& A) {
		Matrix3f ret;
		for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
						ret(i, j) = A[i][j];
				}
		}
		return ret;
}

Mat3f mat3fToMatrix3f(const Matrix3f& A) {
		Mat3f ret;
		for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
						ret[i][j] = A(i, j);
				}
		}
		return ret;
}

Vector3f vec3fToVector3f(const Vec3f& v) {
		return Vector3f(v[0], v[1], v[2]);
}

Vec3f vector3fToVec3f(const Vector3f& v) {
		return Vec3f(v[0], v[1], v[2]);
}

bool solve(const Mat3f& A, const Vec3f& b, Vec3f& x) {
		Vector3f xx = mat3fToMatrix3f(A).colPivHouseholderQr().solve(vec3fToVector3f(b));
		x = vector3fToVec3f(xx);

		return true;
}


#endif //__LINEAR_SOLVER_H__
