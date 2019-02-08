/**
====================================================================================
|            @name: HandModel.h
|          @author: abel ticona
|           @email: jaticona@gmail.com
====================================================================================
*/

#ifndef TOOL_MODEL_H
#define TOOL_MODEL_H


#include "VRControl.h"
#include <vector>

constexpr int DIM_X = 151;
constexpr int DIM_Y = 151;
constexpr int DIM_Z = 151;

constexpr int PERCEPT_PARTICLES = 20;

constexpr int SIZE_PARTICLES = DIM_X * DIM_Y * DIM_Z / PERCEPT_PARTICLES;

struct ContainerModel{

	vec3 particlesResult[SIZE_PARTICLES];
	int phaseResult[SIZE_PARTICLES];
	int indicesResult[SIZE_PARTICLES];

	int matrixIndex[DIM_X][DIM_Y][DIM_Z];

	const float dimensionMatrix[3] = {DIM_X, DIM_Y, DIM_Z};
	const int centerMatrix[3] = {DIM_X/2, 0, DIM_Z / 2};

	/// buffer adjacentes
	int indexAdjacent[SIZE_PARTICLES];
	/// buffer particles
	vec3 particles[SIZE_PARTICLES];
	//int indices[SIZE_PARTICLES];
	int phases[SIZE_PARTICLES];

	/// properties for fill 
	float dimensionContainer[3];
	float centerContainer[3];

	/// particles actives
	int activeParticles;
	/// index object prev
	//int activeParticlesPrev;
	/// numberNewParticlces
	int numberNewparticles;

	/// flag for indique new object
	bool newObject;

	/// variablel temporal for indique the material type
	int typeMaterialTemp = 0;

	/// radius particle
	float radius;
	float halfRadius;

	/// ids for rendering
	unsigned int idVBO;
	unsigned int indicesVBO;
	//unsigned int colorVBO;

	void init(float radius, float center[3]) {
		/// init index of matrix
		for (int x = 0; x < DIM_X; ++x) {
			for (int y = 0; y < DIM_Y; ++y) {
				for (int z = 0; z < DIM_Z; ++z) {
					this->matrixIndex[x][y][z] = -1;
				}
			}
		}
		for (int i=0;i< SIZE_PARTICLES; i++) {
			this->indexAdjacent[i] = 0;
		}

		this->radius = radius;
		this->halfRadius = radius*0.5f;
		this->activeParticles = 0;
		//this->activeParticlesPrev = this->activeParticles;
		this->numberNewparticles = 0;

		this->newObject = false;


		this->centerContainer[0] = center[0];
		this->centerContainer[1] = center[1];
		this->centerContainer[2] = center[2];

		this->dimensionContainer[0] = this->radius * (DIM_X - 1);
		this->dimensionContainer[1] = this->radius * (DIM_Y - 1);
		this->dimensionContainer[2] = this->radius * (DIM_Z - 1);
	}

	void discretization(vec3& position, int posDis[3] ) {
		posDis[0] = MyRound(position.x  * (DIM_X - 1) / dimensionContainer[0] + centerMatrix[0]);
		posDis[1] = MyRound(position.y  * (DIM_Y - 1) / dimensionContainer[1] + centerMatrix[1] - this->halfRadius);
		posDis[2] = MyRound(position.z  * (DIM_Z - 1) / dimensionContainer[2] + centerMatrix[2]);
	}
	void discretizationInv(int posDis[3], vec3& position) {
		position.x = (posDis[0] - centerMatrix[0]) * dimensionContainer[0] / (DIM_X - 1);
		position.y = (posDis[1] - centerMatrix[1]) * dimensionContainer[1] / (DIM_Y - 1) + this->halfRadius;
		position.z = (posDis[2] - centerMatrix[2]) * dimensionContainer[2] / (DIM_Z - 1);
	}
	bool verifyState(vec3& position, int phase) {
		int posDis[3];
		discretization( position, posDis);

		if ( this->verifyPosition(posDis[0], posDis[1], posDis[2] ) && this->isEmpty(posDis[0], posDis[1], posDis[2])) {
			verifyNeignbor(posDis);
			discretizationInv(posDis, position);
			this->matrixIndex[posDis[0]][posDis[1]][posDis[2]] = this->activeParticles;

			this->particles[this->activeParticles].init(position);
			//this->indices[this->activeParticles] = this->activeParticles;
			this->phases[this->activeParticles] = phase;

			this->activeParticles++;
			return true;
		}
		return false;
	}
	bool verifyStateInv(vec3& position, int *index) {
		int posDis[3];
		discretization(position, posDis);

		if (this->verifyPosition(posDis[0], posDis[1], posDis[2]) && !this->isEmpty(posDis[0], posDis[1], posDis[2])) {
			this->verifyNeignborInv(posDis);
			*index = this->matrixIndex[posDis[0]][posDis[1]][posDis[2]];
			this->matrixIndex[posDis[0]][posDis[1]][posDis[2]] = -1;
			this->phases[*index] = -1;

			return true;
		}
		return false;
	}
	bool verifyPosition(int x, int y, int z) {
		if (x >= DIM_X || y >= DIM_Y || z >= DIM_Z || x < 0 || y < 0 || z < 0) {
			//std::cout << "OVERFLOW" << std::endl;
			return false;
		}
		return true;
	}
	bool isEmpty(int x, int y, int z) {		
		if (this->matrixIndex[x][y][z] < 0) {
			return true;
		}
		return false;
	}
	void verifyNeignbor(int p[3]) {
		/// front 
		if (verifyPosition(p[0], p[1], p[2] + 1)){
			if( isEmpty(p[0], p[1], p[2] + 1) )
				this->indexAdjacent[this->activeParticles] += 1;
			else 
				this->indexAdjacent[this->matrixIndex[p[0]][p[1]][p[2] + 1]] -= 4;
		}
		/// right
		if (verifyPosition(p[0]+1, p[1], p[2])) {
			if (isEmpty(p[0] + 1, p[1], p[2]))
				this->indexAdjacent[this->activeParticles] += 2;
			else
				this->indexAdjacent[this->matrixIndex[p[0]+1][p[1]][p[2] ]] -= 8;
		}
		/// back
		if (verifyPosition(p[0], p[1], p[2] -1)) {
			if (isEmpty(p[0], p[1], p[2] - 1))
				this->indexAdjacent[this->activeParticles] += 4;
			else
				this->indexAdjacent[this->matrixIndex[p[0]][p[1]][p[2] - 1]] -= 1;
		}
		/// left
		if (verifyPosition(p[0] - 1, p[1], p[2])) {
			if (isEmpty(p[0] - 1, p[1], p[2]))
				this->indexAdjacent[this->activeParticles] += 8;
			else
				this->indexAdjacent[this->matrixIndex[p[0]-1][p[1]][p[2]]] -= 2;
		}
		/// up
		if (verifyPosition(p[0] , p[1] + 1, p[2])) {
			if (isEmpty(p[0], p[1] + 1, p[2]))
				this->indexAdjacent[this->activeParticles] += 16;
			else
				this->indexAdjacent[this->matrixIndex[p[0]][p[1] + 1][p[2]]] -= 32;
		}
		/// down
		if (verifyPosition(p[0], p[1] -1, p[2])) {
			if (isEmpty(p[0], p[1] - 1, p[2]))
				this->indexAdjacent[this->activeParticles] += 32;
			else
				this->indexAdjacent[this->matrixIndex[p[0]][p[1] - 1][p[2]]] -= 16;
		}
	}
	void verifyNeignborInv(int p[3]) {
		/// front 
		if (verifyPosition(p[0], p[1], p[2] + 1) && !isEmpty(p[0], p[1], p[2] + 1)) {
			this->indexAdjacent[this->matrixIndex[p[0]][p[1]][p[2] + 1]] += 4;
		}
		/// right
		if (verifyPosition(p[0] + 1, p[1], p[2]) && !isEmpty(p[0] + 1, p[1], p[2])) {
			this->indexAdjacent[this->matrixIndex[p[0] + 1][p[1]][p[2]]] += 8;
		}
		/// back
		if (verifyPosition(p[0], p[1], p[2] - 1) && !isEmpty(p[0], p[1], p[2] - 1)) {
			this->indexAdjacent[this->matrixIndex[p[0]][p[1]][p[2] - 1]] += 1;
		}
		/// left
		if (verifyPosition(p[0] - 1, p[1], p[2]) && !isEmpty(p[0] - 1, p[1], p[2])) {
			this->indexAdjacent[this->matrixIndex[p[0] - 1][p[1]][p[2]]] += 2;
		}
		/// up
		if (verifyPosition(p[0], p[1] + 1, p[2]) && !isEmpty(p[0], p[1] + 1, p[2])) {
			this->indexAdjacent[this->matrixIndex[p[0]][p[1] + 1][p[2]]] += 32;
		}
		/// down
		if (verifyPosition(p[0], p[1] - 1, p[2]) && !isEmpty(p[0], p[1] - 1, p[2])) {
			this->indexAdjacent[this->matrixIndex[p[0]][p[1] - 1][p[2]]] += 16;
		}
		this->indexAdjacent[this->matrixIndex[p[0]][p[1]][p[2]]] = 0;
	}

	
	void rebuild() {
		int newActiveParticles = 0;
		for (int i=0;i< this->activeParticles; i++) {
			if (this->phases[i] != -1 ) {
				this->particlesResult[newActiveParticles] = this->particles[i];
				this->phaseResult[newActiveParticles] = this->phases[i];
				this->indicesResult[newActiveParticles] = newActiveParticles;
				newActiveParticles++;
			}
		}
		this->numberNewparticles = newActiveParticles;
	}
	void reset() {
		int newActiveParticles = 0;
		int posDis[3];
		for (int i = 0; i < this->activeParticles; i++) {
			if (this->phases[i] != -1) {
				discretization(this->particles[i], posDis);
				this->matrixIndex[posDis[0]][posDis[1]][posDis[2]] = -1;
				this->phases[i] = -1;
				this->indexAdjacent[i] = 0;
			}
		}
		this->numberNewparticles = this->activeParticles = 0;
		this->newObject = false;
	}
	void rebuild2() {
		int newActiveParticles = 0;
		for (int i = 0; i < this->activeParticles; i++) {
			if (this->phases[i] != -1 && this->indexAdjacent[i] > 0 ) {
				this->particlesResult[newActiveParticles] = this->particles[i];
				this->phaseResult[newActiveParticles] = this->phases[i];
				this->indicesResult[newActiveParticles] = newActiveParticles;
				newActiveParticles++;
			}
		}
		this->numberNewparticles = newActiveParticles;
		//this->newObject = false;
	}
};


struct EmitterModel
{
	EmitterModel() : speed(0.0f), positionIni(0.0f), width(3), formBrush(0), typeMaterial(0) {}

	vec3 position;
	//vec3 direction;
	vec3 velocity;
	vec3 right;

	float speed;

	int material; /// type material ( solid , deformable, liquid, gas)
	vec3 positionIni;

	/// variables that define type brush
	int width;
	int formBrush;
	int typeMaterial;
	int typeMaterialLast;
};

/**
------------------------------------------------------------------------------------
|				Class Name: ToolModel
|	     Description: class that represent a the tool hand'
------------------------------------------------------------------------------------
*/
/// Maximun Val
constexpr int WidthMax = 7;
constexpr int FormBrushMax = 6;
constexpr int TypeMaterialMax = 4;

class ToolModel
{
public:

	/// offset number of the particles
	int offsetParticles = 0;
	int numberParticles = 0;
	std::vector<vec3> particles;
	
	/// Reference to Hand tracking
	HandModel *handModel = 0;
	ControlModel *controlModel = 0;

	/// Reference to Container for modeling
	ContainerModel *container = 0;

	/// Information of the emitter
	EmitterModel emitter;

	/// Indique if will be an emitter tool( true= without collision, false= with collision )
	bool toolEmitter;

	/// Indique if tool is emitting particles currently
	bool isEmitter;

	/// Indique if tool is grasp in objects
	bool isGrasp;

	/// function that create type brushes
	void(*brushes[FormBrushMax]) (float radius, int width, std::vector<vec3>* newposition);

	/// matrix transformation for Tool
	float matrixTool[16];
	float matrixBrush[16];
	
	/// only for oculus tracking
	bool joystickActive = false;

	ToolModel();
	~ToolModel();

	bool bindHand(TypeHand type,VRControl *vrcontrol, ContainerModel *container);
	void init(bool emitter = true);
	

	void eventControllerJoystick();
	void eventController(vr::VREvent_t event);

	/// update position tool
	bool update();

	/// emitter particles in the Matrix
	void managerParticles(int phase);

	/// refuse particles in the Matrix
	void refuseParticles();

	/// interpolation velocity
	void interpolationVelocity(std::vector<vec3>* list);
};

#endif // TOOL_MODEL_H
