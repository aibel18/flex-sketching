/**
====================================================================================
|            @name: HandModel.h
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#ifndef HAND_MODEL_H
#define HAND_MODEL_H

#ifdef _WIN32
	#include <windows.h>
#endif // _WIN32

#include <cmath>

struct vec3 {
	float x;
	float y;
	float z;

	vec3() {this->init(0.0f);}
	vec3(float p) { this->init(p); }
	//vec3(vec3 &v) { this->init(v); }
	vec3(vec3 &v) { this->init(v); }
	vec3(float x, float y, float z) { this->init(x,y,z); }

	void init (vec3 &v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	void init(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void init(float p) {
		this->x = p;
		this->y = p;
		this->z = p;
	}

	vec3 operator+ (vec3 v2) {
		vec3 v(this->x + v2.x, this->y + v2.y, this->z + v2.z);
		return v;
	}
	vec3 operator* (float val) {
		vec3 v(this->x * val, this->y * val, this->z * val);
		return v;
	}
	vec3 operator/ (float val) {
		vec3 v(this->x / val, this->y / val, this->z / val);
		return v;
	}
	/**
	* Magnitude
	*/
	float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}
	/**
	* Normalized
	*/
	vec3 normalize() {
	return *this / this->magnitude();
	}
};

inline int MyRound(float f) {
	return int(f + 0.5f);
}

void sum3(vec3& r, vec3&v1, vec3 &v2);
void multiple4x3(vec3 &r,float m[16], vec3 &v);
void multiple4x4(float r[16], float m[16], float n[16]);
void multiple4xTranslation(float r[16], float m[16], vec3 &p);

enum TypeHand { leftHand, rightHand };
//enum TypeFinger{ thumb, index, middle, ring, pinky};


struct ControlModel {
	bool active = false;
	int indexTrigger;
	int indexJoystick;

	float valTrigger;
	float valJoystick[2];

	ControlModel() {
		this->active = false;
		this->indexJoystick = 0;
		this->indexTrigger = 0;

		this->valTrigger = 0.0f;
		this->valJoystick[0] = 0.0f;
		this->valJoystick[1] = 0.0f;
	}
};

/**
------------------------------------------------------------------------------------
|				Class Name: FingerModel
|	     Description: class that represent a the skeleton model finger's
------------------------------------------------------------------------------------
*/
#define JOINTS 5
class FingerModel {
	public:
		/// type Finger
		//TypeFinger type;
		

		/// position
		vec3 position;
		vec3 direction;

		/// joints
		vec3 joints[JOINTS];

		FingerModel();
		//FingerModel(TypeFinger type);
		virtual ~FingerModel();

		void update(vec3 val[]);
		void draw();
};




/**
------------------------------------------------------------------------------------
|				Class Name: HandModel
|	     Description: class that represent a the skeleton model hand's
------------------------------------------------------------------------------------
*/
#define FINGERS 5

class HandModel {
	
	public:
		/*
		vec3 model[25] = {
			{-0.2, 0.0, 0.1},{-0.2, 0.0,-0.2},{-0.2, 0.0,-0.3},{-0.2, 0.0,-0.4},{-0.2, 0.0,-0.5},
			{-0.1, 0.0, 0.1},{-0.1, 0.0,-0.2},{-0.1, 0.0,-0.3},{-0.1, 0.0,-0.4},{-0.1, 0.0,-0.5},
			{ 0.0, 0.0, 0.1},{ 0.0, 0.0,-0.2},{ 0.0, 0.0,-0.3},{ 0.0, 0.0,-0.4},{ 0.0, 0.0,-0.5},
			{ 0.1, 0.0, 0.1},{ 0.1, 0.0,-0.2},{ 0.1, 0.0,-0.3},{ 0.1, 0.0,-0.4},{ 0.1, 0.0,-0.5},
			{ 0.2, 0.0, 0.1},{ 0.2, 0.0,0.05},{ 0.2, 0.0,-0.1},{ 0.2, 0.0,-0.2},{ 0.2, 0.0,-0.3}
		};*/

		vec3 model[50] = 
		{
			/// rigth hand
			{-0.176931f,0.355885f,0.49162f},{-0.176931f,0.355885f,0.49162f},{-0.38731f,0.350073f,0.0411208f},{-0.46613f,0.295512f,-0.290965f},{-0.471611f,0.226348f,-0.517848f},
			{-0.0270195f,0.530555f,0.438995f},{-0.148431f,0.680539f,-0.267979f},{-0.279323f,0.603115f,-0.676098f},{-0.373658f,0.432085f,-0.824046f},{-0.433119f,0.271557f,-0.850413f},
			{0.090073f,0.510419f,0.408532f},{0.0637102f,0.621315f,-0.277024f},{0.0271874f,0.596833f,-0.763674f},{-0.060669f,0.462195f,-1.00296f},{-0.142962f,0.326418f,-1.10824f},
			{0.199854f,0.460421f,0.387939f},{0.263646f,0.524991f,-0.229389f},{0.303099f,0.510307f,-0.680369f},{0.251537f,0.397159f,-0.932171f},{0.181415f,0.276944f,-1.06065f},
			{0.282156f,0.345149f,0.381826f},{0.423777f,0.394629f,-0.17595f},{0.468455f,0.351435f,-0.528977f},{0.419683f,0.262141f,-0.699159f},{0.330274f,0.150434f,-0.799464f},

			/// left hand
			{0.176931f,0.355885f,0.49162f},{0.176931f,0.355885f,0.49162f},{0.38731f,0.350073f,0.0411208f},{0.46613f,0.295512f,-0.290965f},{0.471611f,0.226348f,-0.517848f},
			{0.0270195f,0.530555f,0.438995f},{0.148431f,0.680539f,-0.267979f},{0.279323f,0.603115f,-0.676098f},{0.373658f,0.432085f,-0.824046f},{0.433119f,0.271557f,-0.850413f},
			{-0.090073f,0.510419f,0.408532f},{-0.0637102f,0.621315f,-0.277024f},{-0.0271874f,0.596833f,-0.763674f},{0.060669f,0.462195f,-1.00296f},{0.142962f,0.326418f,-1.10824f},
			{-0.199854f,0.460421f,0.387939f},{-0.263646f,0.524991f,-0.229389f},{-0.303099f,0.510307f,-0.680369f},{-0.251537f,0.397159f,-0.932171f},{-0.181415f,0.276944f,-1.06065f},
			{-0.282156f,0.345149f,0.381826f},{-0.423777f,0.394629f,-0.17595f},{-0.468455f,0.351435f,-0.528977f},{-0.419683f,0.262141f,-0.699159f},{-0.330274f,0.150434f,-0.799464f}
		};

		bool unable = false;
		int deviceID = -1;
		/// type hand
		TypeHand type;

		/// position
		vec3 position;
		vec3 direction;
		vec3 normal;
		vec3 right;
		vec3 velocity;

		/// indexHand
		int indexParticlesHand =-1;
		int sizeParticlesHand = 0;
		/// fingers
		FingerModel fingers[FINGERS];

		HandModel();
		virtual ~HandModel();

		void init(int particleIndex,bool withParticles);
		void reInit();
		void update( vec3 *val );
		void updateFinger( int i, vec3 *vals );
		void updateFingers( vec3 *vals );
		void calculateFingers(float *m);

		void getMatrix33(float *matrix);
		void getMatrix44(float *matrix);
		float getVelocity();
	
};
/*
class A : public HandModel{
	A():HandModel(rigthHand) {

	}
};*/


#endif // HAND_MODEL_H