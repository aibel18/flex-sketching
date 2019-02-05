/**
====================================================================================
|            @name: HandModel.cpp
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#include "HandModel.h"


/**
------------------------------------------------------------------------------------
|				Class Name: FingerModel.cpp
|	     Description: class that implements HandModel.h
------------------------------------------------------------------------------------
*/

void sum3(vec3& r, vec3&v1, vec3 &v2) {

	float x = v1.x + v2.x;
	float y = v1.y + v2.y;
	float z = v1.z + v2.z;

	r.init(x, y, z);
}

void multiple4x3(vec3& r,float m[16], vec3 &v) {

	float x = v.x * m[0] + v.y * m[4] + v.z * m[8] + m[12];
	float y = v.x * m[1] + v.y * m[5] + v.z * m[9] + m[13];
	float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + m[14];

	r.init(x, y, z);
}

void multiple4xTranslation(float r[16], float m[16], vec3 &p) {

	r[0] = m[0];
	r[1] = m[1];
	r[2] = m[2];
	r[3] = m[3];

	r[4] = m[4];
	r[5] = m[5];
	r[6] = m[6];
	r[7] = m[7];

	r[8] = m[8];
	r[9] = m[9];
	r[10] = m[10];
	r[11] = m[11];

	r[12] = m[0] * p.x + m[4] * p.y + m[8] * p.z + m[12];
	r[13] = m[1] * p.x + m[5] * p.y + m[9] * p.z + m[13];
	r[14] = m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14];
	r[15] = 1;
}

void multiple4x4(float r[16], float m[16], float n[16]) {

	r[0] = m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3];
	r[1] = m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3];
	r[2] = m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3];
	r[3] = m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3];

	r[4] = m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7];
	r[5] = m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7];
	r[6] = m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7];
	r[7] = m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7];
	
	r[8] = m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11];
	r[9] = m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11];
	r[10] = m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11];
	r[11] = m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11];

	r[12] = m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15];
	r[13] = m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15];
	r[14] = m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15];
	r[15] = m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15];
}

/**
| Constructor default
*/
FingerModel::FingerModel() {
}
/*
FingerModel::FingerModel(TypeFinger type) {
	this->type = type;
}*/
FingerModel::~FingerModel() {
}

/**
| uptate point of the skeleton finger
*/
void FingerModel::update(vec3 *val) {

	/// update position of skeleton joints
	for (int i = 0; i < 5; i++) {
		this->joints[i].init( val[i].x, val[i].y, val[i].z );
	}
}


/**
------------------------------------------------------------------------------------
|				Class Name: HandModel.cpp
|	     Description: class that implements HandModel.h
------------------------------------------------------------------------------------
*/

/**
| Constructor default
*/

HandModel::HandModel() {
}
HandModel::~HandModel() {
}

/**
| init por default hand skeleton
*/
void HandModel::init(int particleIndex, bool withParticles) {

	this->unable = true;
	this->sizeParticlesHand = 0;

	float offset = this->type == rightHand ? 0.9f : -0.9f;

	if (withParticles) {
		this->indexParticlesHand = particleIndex;
		this->sizeParticlesHand = 25;
		int k = this->type == rightHand ? 0 : 25;
		
		for (int i = 0; i < FINGERS; i++) {
			FingerModel &f = this->fingers[i];
			for (int j = 0; j < JOINTS; j++, k++) {
				f.joints[j].init(this->model[k].x + offset, this->model[k].y, this->model[k].z);
			}
		}
	}
	
	//this->position.init(offset, 2.0f, 0.0f);
	this->position.init(0.0f, 0.0f, 0.0f);
	this->direction.init(1,0,0);
	this->normal.init(0, 1, 0);
	this->right.init(0, 0, 1);
	this->velocity.init(0,0,0);
}
/**
| Re init por default hand skeleton
*/
void HandModel::reInit() {
	int k = this->type == rightHand ? 0 : 25;
	float offset = this->type == rightHand ? 0.9f : -0.9f;
	for (int i = 0; i < FINGERS; i++) {
		FingerModel &f = this->fingers[i];
		for (int j = 0; j < JOINTS; j++, k++) {
			f.joints[j].init(this->model[k].x + offset, this->model[k].y, this->model[k].z);
		}
	}
}
/**
| update points of skeleton hand
*/
void HandModel::update(vec3 *val) {

	/// update velocity hand
	this->velocity.x = val[0].x;
	this->velocity.y = val[0].y;
	this->velocity.z = val[0].z;


	/// update position hand
	this->position.x = val[1].x;
	this->position.y = val[1].y;
	this->position.z = val[1].z;

	/// update direction hand
	this->direction.x = val[2].x;
	this->direction.y = val[2].y;
	this->direction.z = val[2].z;

	/// update normal hand
	this->normal.x = val[3].x;
	this->normal.y = val[3].y;
	this->normal.z = val[3].z;

	/// update right hand
	this->right.x = val[4].x;
	this->right.y = val[4].y;
	this->right.z = val[4].z;
}

/**
| update points of skeleton finger i
*/
void HandModel::updateFinger(int i, vec3 *val) {
	this->fingers[i].update(val);
}
 
/**
| update points of skeleton of fingers
*/
void HandModel::updateFingers(vec3 *val) {	
	for (int i=0;i<5;i++) {
		this->fingers[i].update( (val + i*5 ));
	}
}

/**
| update points of skeleton of fingers with matrix 4x4
*/
void HandModel::calculateFingers(float *m) {	

	int k = this->type == rightHand ? 0 : 25;
	for (int i = 0; i < FINGERS; i++) {
		FingerModel &f = this->fingers[i];
		for (int j = 0; j < JOINTS; j++,k++) {
			multiple4x3(f.joints[j], m, model[k] );
		}
	}
}


/**
| get matrix 3x3
*/
void HandModel::getMatrix33(float *m) {

	m[0] = this->direction.x;
	m[1] = this->direction.y;
	m[2] = this->direction.z;
	
	m[3] = this->normal.x;
	m[4] = this->normal.y;
	m[5] = this->normal.z;

	m[6] = this->right.x;
	m[7] = this->right.y;
	m[8] = this->right.z;
}
/**
| get matrix 4x4
*/
void HandModel::getMatrix44(float *m) {

	m[0] = this->direction.x;
	m[1] = this->direction.y;
	m[2] = this->direction.z;
	m[3] = 0;

	m[4] = this->normal.x;
	m[5] = this->normal.y;
	m[6] = this->normal.z;
	m[7] = 0;

	m[8] = this->right.x;
	m[9] = this->right.y;
	m[10] = this->right.z;
	m[11] = 0;


	m[12] = this->position.x;
	m[13] = this->position.y;
	m[14] = this->position.z;
	m[15] = 1;
}

/**
| get magnitude of velocity
*/
float HandModel::getVelocity() {
	return this->velocity.magnitude();
}