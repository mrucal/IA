#ifndef AGENT__
#define AGENT__

#include <string>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:

	Agent(){
	    bump_=dirty_=dobleGiroL=false;
	    x=y=9;
	    orientation=mejorCamino=NORTH;

	    for(int i=0;i<20;i++)
            for(int j=0;j<20;j++)
                mapa[i][j]=1; // Se inicializa a 1 para poder diferenciarlos de los obstáculos
                              // y que se pueda incrementar.

        accion=accAnterior=actIDLE;
    }

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
	    actSUCK,
	    actIDLE
	};

	void Perceive(const Environment &env);
	ActionType Think();

	void Actualizar(); // Función que actualiza todas las variables.
	void ActualizarOrientacion(); // Función que actualiza la orientación del robot respecto al mapa.
	void ActualizarPosicion(); // Función que actualiza las coordenadas del robot.
 	void ActualizarMapa(); // Función que introduce un obstaculo en el mapa.
	void ActualizarFuncionPotencial(); // Función que actualiza la función de potencial.
	void ActualizarMejorCamino(); // Función que actualiza mejorCamino.

	void PintarMapa();
    void PintarFP();


private:
	bool bump_, dirty_;

	int mapa[20][20]; // Mapa que representa el mundo de la aspiradora. Los obstáculos se representan con un 0.
                      // También representa la función de potencial que indicará cual es el mejor camino a seguir.
	int x,y; // Coordenadas de la posicion donde se encuentra la aspiradora.
	int orientation; // Orientacion de la aspiradora respecto al mundo.
	int mejorCamino; // Indica la dirección en la que se encuentra la casilla en la que hace mas tiempo que no ha pasado el robot.
	bool dobleGiroL; // Vale true si el robot necesita hacer un doble giro.

	ActionType accion,accAnterior; // Variables que representan la acción que se va a tomar, y la que se tomó en el paso previo.

};

string ActionStr(Agent::ActionType);

#endif
