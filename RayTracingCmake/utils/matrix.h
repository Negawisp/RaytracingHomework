#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "vec.h"
#include <assert.h>

template<size_t DIM, typename T> struct Mat {
public:
    Mat() {}

    vec<DIM, T> getRow(const size_t i) const {
        assert(i < DIM);
        vec<DIM, T> ret;
        for (int j = 0; j < DIM; j++) {
            ret[j] = _data[i][j];
        }
        return ret;
    }

    vec<DIM, T> getColumn(const size_t j) const {
        assert(j < DIM);
        vec<DIM, T> ret;
        for (int i = 0; i < DIM; i++) {
            ret[i] = _data[i][j];
        }
        return ret;
    }

    float* operator[](const size_t i) {
        return _data[i];
    }

    vec<DIM, T> operator[](const size_t i) const {
        return getRow(i);
    }

    void swapRows(const size_t i, const size_t j) {
        for (int k = 0; k < DIM; k++) {
            float t = _data[i][k];
            _data[i][k] = _data[j][k];
            _data[j][k] = t;
        }
    }

    unsigned int size() const {
        return DIM;
    }

private:
    float _data[DIM][DIM] = {};
};

typedef Mat<4, float> Mat4f;
typedef Mat<3, float> Mat3f;

Vec3f transformPoint(const Mat4f& M, const Vec3f& p);
Vec3f transformVec(const Mat4f& M, const Vec3f& v);

Vec3f operator*(const Mat3f& M, const Vec3f& rightVec) {
    return Vec3f(
        M.getRow(0) * rightVec,
        M.getRow(1) * rightVec,
        M.getRow(2) * rightVec
    );
}

Vec4f operator*(const Mat4f& M, const Vec4f& rightVec) {
    return Vec4f(
        M.getRow(0) * rightVec,
        M.getRow(1) * rightVec,
        M.getRow(2) * rightVec,
        M.getRow(3) * rightVec
    );
}

Vec4f operator*(const Vec4f& leftVec, const Mat4f& M) {
    Vec4f ret(0,0,0,0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[j] += leftVec[j] * M[i][j];
        }
    }
    return ret;
}

Mat4f operator+(const Mat4f& lM, const Mat4f& rM) {
    Mat4f ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = lM[i][j] * rM[i][j];
        }
    }
    return ret;
}

Mat4f operator-(const Mat4f& lM, const Mat4f& rM) {
    Mat4f ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = lM[i][j] - rM[i][j];
        }
    }
    return ret;
}

Mat4f operator*(const Mat4f& M, float a) {
    Mat4f ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = M[i][j] * a;
        }
    }
    return ret;
}

Mat4f operator*(float a, const Mat4f& M) {
    return M * a;
}

Mat4f operator*(const Mat4f& lM, const Mat4f& rM) {
    Mat4f ret;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret[i][j] = lM.getRow(i) * rM.getColumn(j);
        }
    }
    return ret;
}

Mat4f makeAffineMatrix(const Vec3f& translation, float scale, const Vec3f& rotationDir, float rotationAngle) {
    Vec3f t = translation;
    Vec3f d = rotationDir * (1 / rotationDir.norm());
    float cos_fi = cos(rotationAngle);
    float sin_fi = sin(rotationAngle);
    
    Mat4f r;

    r[0][0] = (cos_fi + d.x * d.x * (1 - cos_fi)) * scale;
    r[0][1] = (d.x * d.y * (1 - cos_fi) - d.z * sin_fi) * scale;
    r[0][2] = (d.z * d.x * (1 - cos_fi) + d.y * sin_fi) * scale;

    r[1][0] = (d.y * d.x * (1 - cos_fi) + d.z * sin_fi) * scale;
    r[1][1] = (cos_fi + d.y * d.y * (1 - cos_fi)) * scale;
    r[1][2] = (d.y * d.z * (1 - cos_fi) - d.x * sin_fi) * scale;

    r[2][0] = (d.z * d.x * (1 - cos_fi) - d.y * sin_fi) * scale;
    r[2][1] = (d.y * d.z * (1 - cos_fi) + d.x * sin_fi) * scale;
    r[2][2] = (cos_fi + d.z * d.z * (1 - cos_fi)) * scale;

    r[0][3] = t.x;
    r[1][3] = t.y;
    r[2][3] = t.z;
    r[3][3] = 1;

    return r;
}

Mat4f makeReverseAffineMatrix(const Vec3f& translation, float scale, const Vec3f& rotationDir, float rotationAngle) {
    Vec3f d = rotationDir * (1 / rotationDir.norm());
    float cos_fi = cos(rotationAngle);
    float sin_fi = sin(rotationAngle);
    scale = 1 / scale;

    Mat4f r;

    r[0][0] = (cos_fi + d.x * d.x * (1 - cos_fi)) * scale;
    r[0][1] = (d.y * d.x * (1 - cos_fi) + d.z * sin_fi) * scale;
    r[0][2] = (d.z * d.x * (1 - cos_fi) - d.y * sin_fi) * scale;

    r[1][0] = (d.x * d.y * (1 - cos_fi) - d.z * sin_fi) * scale;
    r[1][1] = (cos_fi + d.y * d.y * (1 - cos_fi)) * scale;
    r[1][2] = (d.y * d.z * (1 - cos_fi) + d.x * sin_fi) * scale;

    r[2][0] = (d.z * d.x * (1 - cos_fi) + d.y * sin_fi) * scale;
    r[2][1] = (d.y * d.z * (1 - cos_fi) - d.x * sin_fi) * scale;
    r[2][2] = (cos_fi + d.z * d.z * (1 - cos_fi)) * scale;

    Vec3f t = transformVec(r, -translation);

    r[0][3] = t.x;
    r[1][3] = t.y;
    r[2][3] = t.z;
    r[3][3] = 1;

    return r;
}

Mat3f makeMatrixFromRows(Vec3f r1, Vec3f r2, Vec3f r3) {
    Mat3f M;

    for (int i = 0; i < 3; i++) {
        M[0][i] = r1[i];
        M[1][i] = r2[i];
        M[2][i] = r3[i];
    }

    return M;
}

Mat3f makeMatrixFromColumns(Vec3f r1, Vec3f r2, Vec3f r3) {
    Mat3f M;

    for (int i = 0; i < 3; i++) {
        M[i][0] = r1[i];
        M[i][1] = r2[i];
        M[i][2] = r3[i];
    }

    return M;
}

Vec3f transformPoint(const Mat4f& M, const Vec3f& p) {
    Vec4f p4(p.x, p.y, p.z, 1);
    Vec4f ret4 = M * p4;
    return Vec3f(ret4.x, ret4.y, ret4.z);
}

Vec3f transformVec(const Mat4f& M, const Vec3f& v) {
    Vec4f vec4(v.x, v.y, v.z, 0);
    Vec4f ret4 = M * vec4;
    return Vec3f(ret4.x, ret4.y, ret4.z);
}

Vec3f transformVec(const Mat3f& M, const Vec3f& v) {
    return M * v;
}

Ray transformRay(const Mat4f& M, const Ray& r) {
    return Ray(transformPoint(M, r.getOrigin()), transformVec(M, r.getDirection()));
}

float determinant(Mat3f M) {
    return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
        M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) +
        M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
}

#endif // !__MATRIX_H__
