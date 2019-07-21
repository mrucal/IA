#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{

    //PintarMapa();
    //PintarFP();

    Actualizar();

//Regla 1
    if (dirty_){ // No se puede avanzar a la siguiente casilla sin que la acual esté totalmente limpia.
        accion=actSUCK;
        cout<<"Regla 1"<<endl;
	}

//Regla 2
    else if(dobleGiroL){
        accion=actTURN_L; // Con este giro tenemos ya el doble giro. EL primer giro se hace en la regla 5.
        dobleGiroL=false; // Indicamos que ya hemos hecho el giro.
        cout<<"Regla 2"<<endl;
    }

//Regla 3
    else if(mejorCamino==NORTH){
        accion=actFORWARD;
        cout<<"Regla 3"<<endl;
    }

//Regla 4
    else if(mejorCamino==EAST){
        accion=actTURN_R;
        cout<<"Regla 4"<<endl;
    }

//Regla 5
    else if(mejorCamino==SOUTH){
        accion=actTURN_L;
        dobleGiroL=true;
        cout<<"Regla 5"<<endl;
    }

//Regla 6
    else if(mejorCamino==WEST){
        accion=actTURN_L;
        cout<<"Regla 6"<<endl;
    }

	return accion;

}

// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	bump_ = env.isJustBump();
	dirty_ = env.isCurrentPosDirty();
}

// -----------------------------------------------------------

string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actSUCK: return "SUCK";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}

// -----------------------------------------------------------
// -----------------------------------------------------------






// -----------------------------------------------------------
// -----------------------------------------------------------

void Agent::Actualizar(){

    accAnterior=accion;

    ActualizarOrientacion();
    ActualizarPosicion();
    ActualizarMapa();
    ActualizarFuncionPotencial();
    ActualizarMejorCamino();

}

// -----------------------------------------------------------

void Agent::ActualizarOrientacion(){

    switch(accAnterior){ // El robot solo cambia de orientación al realizar un giro, sea cual sea el sentido.
        case Agent::actTURN_L: orientation=(orientation+3)%4; break;
        case Agent::actTURN_R: orientation=(orientation+1)%4; break;
    }

}

// -----------------------------------------------------------

void Agent::ActualizarPosicion(){

    if(accAnterior==Agent::actFORWARD && !bump_) // Las coordenadas solo cambian al avanzar y siempre que no haya chocado.
        switch(orientation){
            case NORTH: x--; break;
            case EAST: y++; break;
            case SOUTH: x++; break;
            case WEST: y--; break;
        }

}

// -----------------------------------------------------------

void Agent::ActualizarMapa(){

    if(bump_) // Para marcar el obstáculo el robot tiene que chocar. Lo ponemos a 0 para que la posición del obstáculo
              // nunca sea elegida por la variable mejorCamino.
        switch(orientation){
            case NORTH: mapa[x-1][y]=0; break;
            case EAST: mapa[x][y+1]=0; break;
            case SOUTH: mapa[x+1][y]=0; break;
            case WEST: mapa[x][y-1]=0; break;
        }

}

// -----------------------------------------------------------

void Agent::ActualizarFuncionPotencial(){

    for(int i=0;i<20;i++)
        for(int j=0;j<20;j++){
            if(mapa[i][j] != 0) // Si no hay un obstáculo.
                mapa[i][j]++; // Incrementamos la función de potencial.
        }

    mapa[x][y]=1; // Ponemos el valor de inicialización en las coordenadas actuales para indicar que acabamos
                  // de pasar por ahí. Al tener de nuevo un valor bajo, si el robot en pasos recientes vuelve a
                  // tener en cuenta estas coordenadas sabrá que ha pasado hace poco y no volvera a tomar ese camino.

}

// -----------------------------------------------------------

void Agent::ActualizarMejorCamino(){

    switch(orientation){ // Según la orientación del robot el sentido de la dirección sera distinta.
        case NORTH:
            if(mapa[x-1][y]>=mapa[x][y-1] && mapa[x-1][y]>=mapa[x+1][y] && mapa[x-1][y]>=mapa[x][y+1]) mejorCamino=NORTH;
            else if(mapa[x+1][y]>=mapa[x][y-1] && mapa[x+1][y]>=mapa[x][y+1] && mapa[x+1][y]>=mapa[x-1][y]) mejorCamino=SOUTH;
            else if(mapa[x][y-1]>=mapa[x][y+1] && mapa[x][y-1]>=mapa[x+1][y] && mapa[x][y-1]>=mapa[x-1][y]) mejorCamino=WEST;
            else if(mapa[x][y+1]>=mapa[x][y-1] && mapa[x][y+1]>=mapa[x+1][y] && mapa[x][y+1]>=mapa[x-1][y]) mejorCamino=EAST;
            break;
        case EAST:
            if(mapa[x][y+1]>=mapa[x][y-1] && mapa[x][y+1]>=mapa[x+1][y] && mapa[x][y+1]>=mapa[x-1][y]) mejorCamino=NORTH;
            else if(mapa[x][y-1]>=mapa[x][y+1] && mapa[x][y-1]>=mapa[x+1][y] && mapa[x][y-1]>=mapa[x-1][y]) mejorCamino=SOUTH;
            else if(mapa[x-1][y]>=mapa[x][y-1] && mapa[x-1][y]>=mapa[x+1][y] && mapa[x-1][y]>=mapa[x][y+1]) mejorCamino=WEST;
            else if(mapa[x+1][y]>=mapa[x][y-1] && mapa[x+1][y]>=mapa[x][y+1]&& mapa[x+1][y]>=mapa[x-1][y]) mejorCamino=EAST;
            break;
        case SOUTH:
            if(mapa[x+1][y]>=mapa[x][y-1] && mapa[x+1][y]>=mapa[x][y+1] && mapa[x+1][y]>=mapa[x-1][y]) mejorCamino=NORTH;
            else if(mapa[x-1][y]>=mapa[x][y-1] && mapa[x-1][y]>=mapa[x+1][y] && mapa[x-1][y]>=mapa[x][y+1]) mejorCamino=SOUTH;
            else if(mapa[x][y+1]>=mapa[x][y-1] && mapa[x][y+1]>=mapa[x+1][y] && mapa[x][y+1]>=mapa[x-1][y]) mejorCamino=WEST;
            else if(mapa[x][y-1]>=mapa[x][y+1] && mapa[x][y-1]>=mapa[x+1][y] && mapa[x][y-1]>=mapa[x-1][y]) mejorCamino=EAST;
            break;
        case WEST:
            if(mapa[x][y-1]>=mapa[x][y+1] && mapa[x][y-1]>=mapa[x+1][y] && mapa[x][y-1]>=mapa[x-1][y]) mejorCamino=NORTH;
            else if(mapa[x][y+1]>=mapa[x][y-1] && mapa[x][y+1]>=mapa[x+1][y] && mapa[x][y+1]>=mapa[x-1][y]) mejorCamino=SOUTH;
            else if(mapa[x+1][y]>=mapa[x][y-1] && mapa[x+1][y]>=mapa[x][y+1] && mapa[x+1][y]>=mapa[x-1][y]) mejorCamino=WEST;
            else if(mapa[x-1][y]>=mapa[x][y-1] && mapa[x-1][y]>=mapa[x+1][y] && mapa[x-1][y]>=mapa[x][y+1]) mejorCamino=EAST;
            break;
    }

    // mejorCamino se quedará con el mayor valor de las casillas que tiene el robot arriba, abajo, a izquierda o a derecha.
    // En caso de empate la prioridad es norte, sur, oeste y este.
}

// -----------------------------------------------------------
// -----------------------------------------------------------






// -----------------------------------------------------------
// -----------------------------------------------------------

void Agent::PintarMapa(){

    cout<<endl;
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            if(i==x && j==y)
                cout<<"X ";
            else if(mapa[i][j]==0)
                cout<<"1 ";
            else
                cout<<"0 ";
        }
        cout<<endl;
    }
    cout<<endl;

}

void Agent::PintarFP(){

    cout<<endl;
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            if(i==x && j==y)
                cout<<"X  ";
            else cout<<mapa[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

}

// -----------------------------------------------------------
// -----------------------------------------------------------
