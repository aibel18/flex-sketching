#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#include <cmath>

class vector3f {
public:

	float x;
	float y;
	float z;

	vector3f(void) {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	vector3f(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	float dotProduct(float x2, float y2, float z2) {
		return(x * x2 + y * y2 + z * z2);
	}
	float dotProduct(const vector3f &v2) {
		return(x * v2.x + y * v2.y + z * v2.z);
	}
	static float dotProduct(const vector3f &v1, const vector3f &v2) {
		return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	void crossProduct(float x2, float y2, float z2) {
		float cross_x = y * z2 - z * y2;
		float cross_y = -x * z2 + z * x2;
		float cross_z = x * y2 - y * x2;

		x = cross_x;
		y = cross_y;
		z = cross_z;
	}
	void crossProduct(const vector3f &v2) {
		float cross_x = y * v2.z - z * v2.y;
		float cross_y = -x * v2.z + z * v2.x;
		float cross_z = x * v2.y - y * v2.x;

		x = cross_x;
		y = cross_y;
		z = cross_z;
	}
	static vector3f crossProduct(const vector3f &v1, const vector3f &v2) {
		float cross_x = v1.y * v2.z - v1.z * v2.y;
		float cross_y = -v1.x * v2.z + v1.z * v2.x;
		float cross_z = v1.x * v2.y - v1.y * v2.x;

		return vector3f(cross_x, cross_y, cross_z);
	}
	/**
	* opposite vector
	*/
	vector3f opposite() {
		return vector3f(-this->x, -this->y, -this->z);
	}
	/**
	* Add vector
	*/
	vector3f add(const vector3f &v2) {
		float diff_x = this->x + v2.x;
		float diff_y = this->y + v2.y;
		float diff_z = this->z + v2.z;

		return vector3f(diff_x, diff_y, diff_z);
	}
	static vector3f add(const vector3f &v1, const vector3f &v2) {
		float diff_x = v1.x + v2.x;
		float diff_y = v1.y + v2.y;
		float diff_z = v1.z + v2.z;

		return vector3f(diff_x, diff_y, diff_z);
	}
	/**
	* Subtract vector
	*/
	vector3f subtract(const vector3f &v2) {
		float diff_x = this->x - v2.x;
		float diff_y = this->y - v2.y;
		float diff_z = this->z - v2.z;

		return vector3f(diff_x, diff_y, diff_z);
	}
	static vector3f subtract(const vector3f &v1, const vector3f &v2) {
		float diff_x = v1.x - v2.x;
		float diff_y = v1.y - v2.y;
		float diff_z = v1.z - v2.z;

		return vector3f(diff_x, diff_y, diff_z);
	}
	/**
	* Multiply vector for a scalar
	*/
	void multiply(float scalar) {
		this->x = scalar * this->x;
		this->y = scalar * this->y;
		this->z = scalar * this->z;
	}
	/**
	* Magnitude
	*/
	float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}
	/**
	* Normalize
	*/
	void normalize() {
		float m = magnitude();

		x = x / m;
		y = y / m;
		z = z / m;
	}
	/// distance between two vectors
	float distance(const vector3f &v2) {
		float diff_x = v2.x - this->x;
		float diff_y = v2.y - this->y;
		float diff_z = v2.z - this->z;

		return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
	}
	static float distance(const vector3f &v1, const vector3f &v2) {
		float diff_x = v2.x - v1.x;
		float diff_y = v2.y - v1.y;
		float diff_z = v2.z - v1.z;

		return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
	}
};
inline void calculateMatrixP(float projectionMatrix4x4[16], float modelViewMatrix4x4[16], float(*result)[4][4]) {

	int i, j, k;            // Loop counters
	for (i = 0; i<4; ++i) {
		for (j = 0; j<4; ++j) {
			double value = 0.;
			for (k = 0; k<4; ++k) {
				//value += modelViewMatrix4x4[i+4*k] * projectionMatrix4x4[k+4*j];
				value += projectionMatrix4x4[i + 4 * k] * modelViewMatrix4x4[k + 4 * j];
			}
			(*result)[i][j] = value;
		}
	}
};
inline void calculateMatrixPInv(float projectionMatrix4x4[16], float modelViewMatrix4x4[16], float(*result)[4][4]) {

	int i, j, k;            // Loop counters
	for (i = 0; i<4; ++i) {
		for (j = 0; j<4; ++j) {
			double value = 0.;
			for (k = 0; k<4; ++k) {
				//value += modelViewMatrix4x4[i+4*k] * projectionMatrix4x4[k+4*j];
				value += projectionMatrix4x4[i + 4 * k] * modelViewMatrix4x4[k + 4 * j];
			}
			(*result)[i][j] = value;
		}
	}
};
inline void calculateProjection(float matrix4x4f[4][4], float vector3f[3], float result[3]) {
	float vector4f[4];
	vector4f[0] = vector3f[0];
	vector4f[1] = vector3f[1];
	vector4f[2] = vector3f[2];
	vector4f[3] = 1;

	float result4f[4];
	for (register int i = 0; i<4; ++i) {
		double value = 0.;
		for (register int j = 0; j<4; ++j) {
			value += (matrix4x4f[i][j] * vector4f[j]);
		}
		result4f[i] = value;
	}

	result[0] = result4f[0];//result4f[3];
	result[1] = result4f[1];//result4f[3];
	result[2] = result4f[2];//result4f[3];

};
inline void calculatePontos3D(float m[4][4], float vector3f[3], float y, float result[3]) {

	float a = m[0][0] - m[2][0] * vector3f[0];
	float b = m[0][2] - m[2][2] * vector3f[0];
	float c = m[1][0] - m[2][0] * vector3f[1];
	float d = m[1][2] - m[2][2] * vector3f[1];

	float e = vector3f[0] * m[2][3] - m[0][3];
	float f = vector3f[1] * m[2][3] - m[1][3];

	float g = (m[0][1] - m[2][1] * vector3f[0])*y;
	float h = (m[1][1] - m[2][1] * vector3f[1])*y;

	e = e - g;
	f = f - h;

	float fa = a * d - b * c;
	result[0] = (d*e - b * f) / fa;//result4f[3];
	result[1] = y;//result4f[3];
	result[2] = (-c * e + a * f) / fa;//result4f[3];

};
inline void invert4x4(float M[4][4]) {
	float a00 = M[0][0]; float a01 = M[0][1]; float a02 = M[0][2]; float a03 = M[0][3];


	float a10 = M[1][0]; float a11 = M[1][1]; float a12 = M[1][2]; float a13 = M[1][3];


	float a20 = M[2][0]; float a21 = M[2][1]; float a22 = M[2][2]; float a23 = M[2][3];


	float a30 = M[3][0]; float a31 = M[3][1]; float a32 = M[3][2]; float a33 = M[3][3];





	M[0][0] = a11 * a22*a33 - a11 * a23*a32 - a21 * a12*a33 + a21 * a13*a32 + a31 * a12*a23 - a31 * a13*a22;


	M[0][1] = -a01 * a22*a33 + a01 * a23*a32 + a21 * a02*a33 - a21 * a03*a32 - a31 * a02*a23 + a31 * a03*a22;


	M[0][2] = a01 * a12*a33 - a01 * a13*a32 - a11 * a02*a33 + a11 * a03*a32 + a31 * a02*a13 - a31 * a03*a12;


	M[0][3] = -a01 * a12*a23 + a01 * a13*a22 + a11 * a02*a23 - a11 * a03*a22 - a21 * a02*a13 + a21 * a03*a12;


	M[1][0] = -a10 * a22*a33 + a10 * a23*a32 + a20 * a12*a33 - a20 * a13*a32 - a30 * a12*a23 + a30 * a13*a22;


	M[1][1] = a00 * a22*a33 - a00 * a23*a32 - a20 * a02*a33 + a20 * a03*a32 + a30 * a02*a23 - a30 * a03*a22;


	M[1][2] = -a00 * a12*a33 + a00 * a13*a32 + a10 * a02*a33 - a10 * a03*a32 - a30 * a02*a13 + a30 * a03*a12;


	M[1][3] = a00 * a12*a23 - a00 * a13*a22 - a10 * a02*a23 + a10 * a03*a22 + a20 * a02*a13 - a20 * a03*a12;


	M[2][0] = a10 * a21*a33 - a10 * a23*a31 - a20 * a11*a33 + a20 * a13*a31 + a30 * a11*a23 - a30 * a13*a21;


	M[2][1] = -a00 * a21*a33 + a00 * a23*a31 + a20 * a01*a33 - a20 * a03*a31 - a30 * a01*a23 + a30 * a03*a21;


	M[2][2] = a00 * a11*a33 - a00 * a13*a31 - a10 * a01*a33 + a10 * a03*a31 + a30 * a01*a13 - a30 * a03*a11;


	M[2][3] = -a00 * a11*a23 + a00 * a13*a21 + a10 * a01*a23 - a10 * a03*a21 - a20 * a01*a13 + a20 * a03*a11;


	M[3][0] = -a10 * a21*a32 + a10 * a22*a31 + a20 * a11*a32 - a20 * a12*a31 - a30 * a11*a22 + a30 * a12*a21;


	M[3][1] = a00 * a21*a32 - a00 * a22*a31 - a20 * a01*a32 + a20 * a02*a31 + a30 * a01*a22 - a30 * a02*a21;


	M[3][2] = -a00 * a11*a32 + a00 * a12*a31 + a10 * a01*a32 - a10 * a02*a31 - a30 * a01*a12 + a30 * a02*a11;


	M[3][3] = a00 * a11*a22 - a00 * a12*a21 - a10 * a01*a22 + a10 * a02*a21 + a20 * a01*a12 - a20 * a02*a11;


	//float D =


	//  (


	//       a00*a11*a22*a33 - a00*a11*a23*a32 - a00*a21*a12*a33 + a00*a21*a13*a32 + a00*a31*a12*a23 - a00*a31*a13*a22


	//     - a10*a01*a22*a33 + a10*a01*a23*a32 + a10*a21*a02*a33 - a10*a21*a03*a32 - a10*a31*a02*a23 + a10*a31*a03*a22


	//     + a20*a01*a12*a33 - a20*a01*a13*a32 - a20*a11*a02*a33 + a20*a11*a03*a32 + a20*a31*a02*a13 - a20*a31*a03*a12


	//     - a30*a01*a12*a23 + a30*a01*a13*a22 + a30*a11*a02*a23 - a30*a11*a03*a22 - a30*a21*a02*a13 + a30*a21*a03*a12


	//  );


	float D = a00 * M[0][0] + a10 * M[0][1] + a20 * M[0][2] + a30 * M[0][3];


	if (D) {


		M[0][0] /= D; M[0][1] /= D; M[0][2] /= D; M[0][3] /= D;


		M[1][0] /= D; M[1][1] /= D; M[1][2] /= D; M[1][3] /= D;


		M[2][0] /= D; M[2][1] /= D; M[2][2] /= D; M[2][3] /= D;


		M[3][0] /= D; M[3][1] /= D; M[3][2] /= D; M[3][3] /= D;


	}


};

inline char* converNumber(float _numero, int _decimal = 6) {
	int a = (int)_numero;
	int cont = 0, c = 0;
	if (_numero<0) {
		a = -1 * a;
		cont++;
		c = 1;
	}
	if (a == 0)
		cont++;

	int temp = a, b;
	while (temp) {
		temp = temp / 10;
		cont++;
	}
	char *pt = new char[cont + 1 + _decimal];

	int i = cont - 1;
	for (; i >= c; i--) {
		b = a % 10;
		a = a / 10;

		pt[i] = '0' + b;
	}

	if (c)
		pt[i] = '-';
	pt[cont] = '\0';

	if (_numero<0)
		_numero = -1 * _numero;
	float num_temp = _numero - (int)_numero;

	if (0<num_temp && num_temp<1) {
		pt[cont] = '.';
		a = (int)(num_temp*pow(10, _decimal));//1000000

		for (int i = cont + _decimal; i>cont; i--) {
			b = a % 10;
			a = a / 10;

			pt[i] = '0' + b;
		}
		int j = cont + _decimal;
		while (pt[j] == '0')
			j--;

		pt[j + 1] = '\0';
	}


	return pt;
	delete[]pt;
};





#endif // _VECTOR3F_H_
