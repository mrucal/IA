#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}



// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

double MiPuntuacionIJ(int i, int j, int jugador, const Environment &estado){
    double suma=0;

    for (int k=i-1; k<+1; k++)
      for (int l=-1; l<+1; l++){
         if (estado.See_Casilla(i,j)==jugador){
            suma+=2;
         }else
         if (estado.See_Casilla(i,j)==0){
            suma+=1;
         }
      }

    return suma;
}

double MiPuntuacionIJ2(int i,int j,int jugador, const Environment &estado){


    int jugador2 = jugador==1 ? 2 : 1;
    double valor=0;

    int k1=0,l1=0,n1=3,n2=3;

    if(i==0){
        k1=1;valor++;
	}if(i==6){
        n1=2;valor++;
	}if(j==0){
        l1=1;valor++;
	}if(j==6){
        n2=2;valor++;
    }

	if((i==0 || i==6) && (j==0||j==6)) valor++;

    i=i-1;j=j-1;

    bool yaContadoFila=false;
    bool yaContadoColumna[3]={false,false,false};
    bool yaContadoDiagonal1=false;
    bool yaContadoDiagonal2=false;

    for(int k=k1;k<n1;k++){
        for(int l=l1;l<n2;l++){
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoFila==false){
               valor++;
               yaContadoFila=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoColumna[l]==false){
               valor++;
               yaContadoColumna[l]=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoDiagonal1==false && k==l){
               valor++;
               yaContadoDiagonal1=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoDiagonal2==false && (k+l)==2 && k!=l){
               valor++;
               yaContadoDiagonal2=true;
           }
        }
        yaContadoFila=false;

    }
    return 8-valor;
}




double MiPuntuacionIJ3(int i,int j,int jugador, const Environment &estado){


    int jugador2 = jugador==1 ? 2 : 1;
    double valor=0; // Numero de filas,columnas y/o diagonales OCUPADAS y en las que ya no se puede hacer el 4 en línea desde la posicion (i,j)

    // Variables que indican si una fila, columna o diagonal ha sido ocupada, para que no se indique mas de una vez en valor
    bool yaContadoFila=false;
    bool yaContadoColumna[5]={false,false,false,false,false};
    bool yaContadoDiagonal1=false;
    bool yaContadoDiagonal2=false;

    int k1=0,l1=0,n1=5,n2=5;

    // Ajuste de los valores iniciales y finales del bucle para que no se salga del tablero.
    // En cada caso hay que contar las filas, columnas o diagonales que se pierden al estar en los extremos del tablero 7x7
    if(i==0){
        k1=2;valor+=2;
	}if(i==1){
	    k1=1;valor++;
	}if(i==5){
        n1=2;valor++;
	}if(i==6){
        n1=3;valor+=2;
	}if(j==0){
        l1=2;valor+=2;
	}if(j==1){
	    l1=1;valor++;
	}if(j==5){
        n2=2;valor++;
	}if(j==6){
        n2=3;valor+=2;
	}

	if((i==0 || i==6) && (j==0||j==6)) valor++; // Si la posición es una de las esquinas hay que contar la diagonal que falta

    i=i-2;j=j-2;

    for(int k=k1;k<n1;k++){
        for(int l=l1;l<n2;l++){
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoFila==false){ // Si la casilla la ocupa el contrincante y no se ha marcado ya la columna como ocupada
               valor++;
               yaContadoFila=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoColumna[l]==false){
               valor++;
               yaContadoColumna[l]=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoDiagonal1==false && k==l){
               valor++;
               yaContadoDiagonal1=true;
           }
           if (estado.See_Casilla(k+i,l+j)==jugador2 && yaContadoDiagonal2==false && (k+l)==4 && k!=l){
               valor++;
               yaContadoDiagonal2=true;
           }
        }
        yaContadoFila=false;

    }
    return 12-valor; // Restamos al máximo de filas, columnas y diagonales que puede haber libres (12 en total), las ocupadas
}

double EnLinea3(int fila, int columna, int jugador, const Environment &estado) {

    double valor=0;

    //3 en línea en la misma fila
    if (columna<5){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila,columna+1) and
          estado.See_Casilla(fila,columna+1) == estado.See_Casilla(fila,columna+2) and
          estado.See_Casilla(fila,columna)==jugador){

          if(columna>0)
                if(estado.See_Casilla(fila,columna-1)==0) valor+=2; // Si hay extermo izquierdo y está libre
          if(columna<4)
             if(estado.See_Casilla(fila,columna+3)==0) valor+=2; // Si hay extermo derecho y está libre
      }
    }

    //Posible cuatro en línea en la misma fila (la tercera casilla libre)
    if (columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila,columna+1) and
          estado.See_Casilla(fila,columna+1) == estado.See_Casilla(fila,columna+3) and
          estado.See_Casilla(fila,columna+2)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //Posible cuatro en línea en la misma fila (la segunda casilla libre)
    if (columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila,columna+2) and
          estado.See_Casilla(fila,columna+2) == estado.See_Casilla(fila,columna+3) and
          estado.See_Casilla(fila,columna+1)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //3 en línea en la misma columna
    if (fila<5){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+1,columna) and
          estado.See_Casilla(fila+1,columna) == estado.See_Casilla(fila+2,columna) and
          estado.See_Casilla(fila,columna)==jugador and
          estado.See_Casilla(fila,columna)==jugador){

          if(fila>0)
            if(estado.See_Casilla(fila-1,columna)==0) valor+=2;
          if(fila<4)
             if(estado.See_Casilla(fila+3,columna)==0) valor+=2;
      }
    }

    //Posible cuatro en línea en la misma columna (la tercera casilla libre)
    if (fila<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+1,columna) and
          estado.See_Casilla(fila+1,columna) == estado.See_Casilla(fila+3,columna) and
          estado.See_Casilla(fila,columna+2)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //Posible cuatro en línea en la misma fila (la segunda casilla libre)
    if (fila<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+2,columna) and
          estado.See_Casilla(fila+2,columna) == estado.See_Casilla(fila+3,columna) and
          estado.See_Casilla(fila,columna+1)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }


    //3 en línea en la diagonal hacia arriba
    if (fila<4 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+1,columna+1) and
          estado.See_Casilla(fila+1,columna+1) == estado.See_Casilla(fila+2,columna+2) and
          estado.See_Casilla(fila,columna)==jugador){

          if(fila>0 && columna>0)
            if(estado.See_Casilla(fila-1,columna-1)==0) valor+=2;
          if(fila<4 && columna<4)
             if(estado.See_Casilla(fila+3,columna+3)==0) valor+=2;
      }
    }

    //Posible cuatro en línea en la diagonal hacia arriba (la tercera casilla libre)
    if (fila<4 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+1,columna+1) and
          estado.See_Casilla(fila+1,columna+1) == estado.See_Casilla(fila+3,columna+3) and
          estado.See_Casilla(fila+2,columna+2)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //Posible cuatro en línea en la diagonal hacia arriba (la segunda casilla libre)
    if (fila<4 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila+2,columna+2) and
          estado.See_Casilla(fila+2,columna+2) == estado.See_Casilla(fila+3,columna+3) and
          estado.See_Casilla(fila+1,columna+1)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //3 en línea en la diagonal hacia abajo
    if (fila>2 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila-1,columna+1) and
          estado.See_Casilla(fila-1,columna+1) == estado.See_Casilla(fila-2,columna+2) and
          estado.See_Casilla(fila,columna)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          if(fila>6 && columna>6)
            if(estado.See_Casilla(fila+1,columna-1)==0) valor+=2;
          if(fila<2 && columna<4)
             if(estado.See_Casilla(fila-3,columna+3)==0) valor+=2;
      }
    }

    //Posible cuatro en línea en la diagonal hacia abajo (la tercera casilla libre)
    if (fila>2 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila-1,columna+1) and
          estado.See_Casilla(fila-1,columna+1) == estado.See_Casilla(fila-3,columna+3) and
          estado.See_Casilla(fila-2,columna+2)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    //Posible cuatro en línea en la diagonal hacia abajo (la tercera casilla libre)
    if (fila>2 and columna<4){
      if (estado.See_Casilla(fila,columna)   == estado.See_Casilla(fila-2,columna+2) and
          estado.See_Casilla(fila-2,columna+2) == estado.See_Casilla(fila-3,columna+3) and
          estado.See_Casilla(fila-1,columna+1)==0 and
          estado.See_Casilla(fila,columna)==jugador){

          valor+=2;
      }
    }

    return valor;

}

double MiPuntuacion(int jugador, const Environment &estado){

    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++)
         if (estado.See_Casilla(i,j)==jugador)
               suma+=/*MiPuntuacionIJ3(i,j,jugador,estado);//+*/EnLinea3(i,j,jugador,estado);

    return suma;
}

double Valoracion1(const Environment &estado, int jugador){

    int ganador = estado.RevisarTablero();
    int jugador2 = jugador==1 ? 2 : 1;
    //cout<<"JUGADOR: "<<jugador<<" JUGADOR2: "<<jugador2<<endl;
    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
        return MiPuntuacion(jugador,estado) - MiPuntuacion(jugador2,estado);
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){

    //return ValoracionTest(estado,jugador);
    return Valoracion1(estado,jugador);
}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<7; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Poda_AlfaBeta (const Environment & tablero, int jug, int prof, int limiteProf, Environment::ActionType & accion, double a, double b){

    Environment::ActionType accion2;
    int aux;

    if (!prof){
        a=menosinf;
        b=masinf;
    }


    if(prof==limiteProf){

        return Valoracion(tablero,jug);

    }else{

        int act=-1;
        Environment hijo = tablero.GenerateNextMove(act); // Generar primer descendiente

        while(act<7){
            aux=Poda_AlfaBeta(hijo,jug,prof+1,limiteProf,accion2,a,b);

            if((prof%2)==0){

                if(a<aux){
                    a=aux;
                    accion=(Environment::ActionType)act;
                }
                if(a>=b) return b;

            }else{

                if(b>aux){
                    b=aux;
                    accion=(Environment::ActionType)act;
                }
                if(b<=a) return a;
            }

            hijo = tablero.GenerateNextMove(act);

        }

        return prof%2==0 ? a : b;
    }
}



// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 3;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[7]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<7; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui
/*
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
*/
    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}


