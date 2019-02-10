#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

int col, row;
int x, y;
int mines;
bool juegoPerdido=false;
int contadorMarcas=0, contadorMinas=0;
bool regresar;

int tablero[100][100];
int tableroMostrar[100][100];

HANDLE h = GetStdHandle (STD_OUTPUT_HANDLE);

void imprimirTablero(int tab[][100], int n, int m){
	cout<<"  X  ";
	for(int i=1; i<=m; i++){
		SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
		cout<<setw(3)<<i;
	}
	cout<<endl<<setw(2)<<"Y    ";
	for(int i=0; i<m; i++){
		cout<<setw(3)<<" __";
	}
	cout<<endl<<endl;
	for(int i=0; i<n; i++){
		SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
		cout<<setw(3)<<i+1<<" |";
		for(int j=0; j<m; j++){
			if(tab[i][j]==99){
				SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
				cout<<setw(3)<<"#";
			}else{
				if(tab[i][j]==1 || tab[i][j]==4 || tab[i][j]==7){
					SetConsoleTextAttribute(h,FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
					cout<<setw(3)<<tab[i][j];
				}else{
					if(tab[i][j]==2 || tab[i][j]==5 || tab[i][j]==8){
						SetConsoleTextAttribute(h,FOREGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
						cout<<setw(3)<<tab[i][j];
					}else{
						if(tab[i][j]==3 || tab[i][j]==6){
							SetConsoleTextAttribute(h,FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
							cout<<setw(3)<<tab[i][j];
						}else{
							if(tab[i][j]==9){
							SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
							cout<<setw(3)<<"*";
							}else{
								if(tab[i][j]==0){
									SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
									cout<<setw(3)<<" ";
								}else{
									if(tab[i][j]==100){
										SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
										cout<<setw(3)<<"?";
									}
								}
							}
						}
					}
				}				
			}			
		}
		cout<<endl;
	}
}

void creacionTablero(int tab[][100], int n, int m){
	for(int i=0; i<n+1; i++){
		for(int j=0; j<m+1; j++){
			tab[i][j]=99;
		}
	}
}

void cantidadMinas(int tab[][100], int n, int m){
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			if(tab[i][j] != 9){ //Verifica que la posición de la matriz no tenga una mina 
				//Comprueba si las aristas tienen minas
				if(i-1 >= 0){ 
					if(tab[i-1][j] == 9)
						tab[i][j]++;
				}						
				if(i+1 < n){ 
					if(tab[i+1][j] == 9)
						tab[i][j]++;
				}					
				if(j-1 >= 0){
					if(tab[i][j-1] == 9)
						tab[i][j]++;
				}					
				if(j+1 < m){
					if(tab[i][j+1] == 9)
						tab[i][j]++;
				}				
				//Comprueba si las esquinas tienen minas
				if(i-1 >= 0 && j-1 >= 0) {
					if(tab[i-1][j-1] == 9)
						tab[i][j]++;
				}				
				if(i-1 >= 0 && j+1 < m) {
					if(tab[i-1][j+1] == 9)
						tab[i][j]++;
				}				
				if(i+1 < n && j-1 >= 0) {
					if(tab[i + 1][j - 1] == 9)
						tab[i][j]++;
				}				
				if(i+1 < n && j+1 < m) {
					if(tab[i+1][j + 1] == 9)
						tab[i][j]++;
				}
			}		
		}
	} 
}

void ubicacionMinas(int tab[][100], int n, int m, int ms){
	srand(time(NULL));
	int count=0;
	while(ms>0){
		int i=rand()%n;
		int j=rand()%m;
		if(tab[i][j]==0){
			tab[i][j]=9;
		}else{
			ms++;
		}
		ms--;
	}
}

bool comprobarPerdedor(int ms){
	if(contadorMarcas==ms && contadorMinas==ms){
		return 1;
	}else{
		return 0;
	}
}

void revelarTablero(int tab[][100], int tab2[][100], int n, int m, int x, int y){
	if(tab2[y][x] == 99 && tab[y][x] != 9 && x >= 0 && x < m && y >= 0 && y < n){
		tab2[y][x]=tab[y][x];
		
		if(tab[y][x]==0){
			revelarTablero(tab, tab2, n, m, x, y - 1);
			revelarTablero(tab, tab2, n, m, x, y + 1);
			revelarTablero(tab, tab2, n, m, x - 1, y - 1);
			revelarTablero(tab, tab2, n, m, x + 1, y - 1);
			revelarTablero(tab, tab2, n, m, x + 1, y + 1);
			revelarTablero(tab, tab2, n, m, x - 1, y + 1);
			revelarTablero(tab, tab2, n, m, x - 1, y);
			revelarTablero(tab, tab2, n, m, x + 1, y);
		}
	}
}

void leerJugada(int tab[][100], int tab2[][100], int row, int col, int ms){
	int ciclo, tipoJugada;
	SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
	int Banderas;
	Banderas=ms-contadorMarcas;
	cout<<endl;
	cout<<"   Banderas restantes: "<<Banderas<<endl;
	cout<<"1) Descubrir casilla"<<endl;
	cout<<"2) Ingresar marca"<<endl;
	cin>>tipoJugada;
	switch(tipoJugada){
		case 1:
			SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
			cout<<"Ingrese coordenada x: ";
			cin>>x;			
			cout<<"Ingrese coordenada y: ";
			cin>>y;
			if(x==-1 && y==-1){
				system("cls");
				imprimirTablero(tableroMostrar,row,col);
				leerJugada(tab,tab2,row,col,ms);
				break;
			}			
			while(x<=0 || x>=col+1 || y<=0 || y>=row+1 || tab2[y-1][x-1]!=99 || tab2[y-1][x-1]==100){
				cout<<"Reingrese coordenadas"<<endl;
				cout<<"Ingrese coordenada x: ";
				cin>>x;
				cout<<"Ingrese coordenada y: ";
				cin>>y;
				if(x==-1 && y==-1){
					system("cls");
					imprimirTablero(tableroMostrar,row,col);
					leerJugada(tab,tab2,row,col,ms);
					break;
				}
			}
			x--;
			y--;			
			if(tab[y][x]==9){
				tab2[y][x]=9;
				juegoPerdido=true;
				for(int i=0; i<row; i++){
					for(int j=0; j<col; j++){
						if(tab[i][j]==9){
							tab2[i][j]=9;
						}
					}
				}
			}else{
				revelarTablero(tab,tab2,row,col,x,y);
			}
			break;
		case 2:
			SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
			cout<<"Ingrese coordenada x: ";
			cin>>x;
			cout<<"Ingrese coordenada y: ";
			cin>>y;
			if(x==-1 && y==-1){
				system("cls");
				imprimirTablero(tableroMostrar,row,col);
				leerJugada(tab,tab2,row,col,ms);
				break;
			}
			while(x<=0 || x>=col+1 || y<=0 || y>=row+1 || tab2[y-1][x-1]<99){
				cout<<"Reingrese marca"<<endl;
				cout<<"Ingrese coordenada x: ";
				cin>>x;
				cout<<"Ingrese coordenada y: ";
				cin>>y;
				if(x==-1 && y==-1){
					system("cls");
					imprimirTablero(tableroMostrar,row,col);
					leerJugada(tab,tab2,row,col,ms);
					break;
				}
			}
			x--;
			y--;			
			if(tab2[y][x]==99 && Banderas>0){
				tab2[y][x]=100;
				contadorMarcas++;
				if(tab[y][x]==9){
					contadorMinas++;
				}
			}else{
				if(tab2[y][x]==100){
					tab2[y][x]=99;
					contadorMarcas--;
					if(tab[y][x]==9){
						contadorMinas--;
					}
				}
			}
			break;
		default:
			system("cls");
			imprimirTablero(tableroMostrar,row,col);
			leerJugada(tab,tab2,row,col,ms);
			break;
	}	
}

void pedirDatos(){
	cout<<" Ingrese el numero de filas:    ";
	cin>>row;
	while(row<=0 || row>50){
		cout<<" Reingrese el numero de filas:    ";
		cin>>row;
	}
	cout<<" Ingrese el numero de columnas: ";
	cin>>col;
	while(col<=0 || col>50){
		cout<<" Reingrese el numero de columnas: ";
		cin>>col;
	}
	if(row*col<=200){
		mines=sqrt(row*col);
	}else{
		mines=(2*sqrt(row*col));
	}	
}

void juego(){
	pedirDatos();
	creacionTablero(tableroMostrar,row,col);
	ubicacionMinas(tablero,row,col,mines);
	cantidadMinas(tablero,row,col);
	
	do{
		system("cls");
		imprimirTablero(tableroMostrar,row,col);
		leerJugada(tablero,tableroMostrar,row,col,mines);
	} while(!juegoPerdido && !comprobarPerdedor(mines));
	
	if(juegoPerdido){
		system("cls");
		imprimirTablero(tablero, row, col);
		SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
		cout<<endl<<"Has Perdido"<<endl;
		system("pause");
	}
	if(comprobarPerdedor(mines)){
		system("cls");	
		imprimirTablero(tablero, row, col);
		SetConsoleTextAttribute(h,BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
		cout<<endl<<"Felicitaciones!"<<endl<<"Has ganado!"<<endl;
		system("pause");
	}
	
}

void comoJugar(){
	system("cls");
	cout<<"  Instrucciones de juego"<<endl<<endl;
	cout<<"  -) El tablero se basa en un sistema de coordenadas x,y."<<endl;
	cout<<"  -) Para descubrir una casilla ingrese a la opcion 1 e ingrese las coordenadas."<<endl;
	cout<<"  -) Para ingresar una marca (banderita) ingrese la opcion 2 e ingrese las coordenadas."<<endl;
	cout<<"  -) Cuando ingrese una marca la casilla seleccionada no podra ser descubierta."<<endl;
	cout<<"  -) Para retirar una marca ingresela de nuevo."<<endl;
	cout<<"  -) Si desea cancelar la jugada (descubrir casilla o ingresar marca) ingrese -1,-1 como coordenadas"<<endl;
	cout<<"  -) Tablero maximo 50x50."<<endl<<endl;
	cout<<"  Dificultad"<<endl<<endl;
	cout<<"  -) Si columnas*filas <= 200 entonces el numero de minas se determinara por la raiz de columnas*filas."<<endl;
	cout<<"  -) Si columnas*filas > 200 entonces el numero de minas se determinara por el doble de la raiz de columnas*filas"<<endl<<endl;
	cout<<"  Recomendaciones"<<endl<<endl;
	cout<<"  -) Para un nivel intermedio ingrese un tablero de 9x9."<<endl;
	cout<<"  -) Para un nivel avanzado ingrese un tablero de 15x15."<<endl<<endl;
	cout<<"  Recuerda que para ganar debes marcar todas las casillas donde hayan minas"<<endl;
	cout<<"  Good Luck!"<<endl<<endl;
	system("pause");
	regresar=true;
}

void eleccionJuego(){
	bool sel=true;
	while(sel==true){
		system("cls");
		cout<<setw(4)<<char(-53)<<char(-51)<<char(-69)<<char(-62)<<char(0)<<char(-62)<<char(-38)<<char(-60)<<char(-65)<<char(-38)<<char(-60)<<char(-65)<<char(-38)<<char(-60);
		cout<<char(-65)<<char(-38)<<char(-62)<<char(-65)<<char(-62)<<char(-38)<<char(-65)<<char(-38)<<char(-38)<<char(-60)<<char(-65)<<char(-38)<<char(-60)<<char(-65)<<endl;
		cout<<setw(4)<<char(-52)<<char(-51)<<char(-71)<<char(-77)<<char(0)<<char(-77)<<char(-64)<<char(-60)<<char(-65)<<char(-77)<<char(0)<<char(0)<<char(-61)<<char(-60);
		cout<<char(-76)<<char(-77)<<char(-77)<<char(-77)<<char(-77)<<char(-77)<<char(-77)<<char(-77)<<char(-61)<<char(-60)<<char(-76)<<char(-64)<<char(-60)<<char(-65)<<endl;
		cout<<setw(4)<<char(-54)<<char(-51)<<char(-68)<<char(-64)<<char(-60)<<char(-39)<<char(-64)<<char(-60)<<char(-39)<<char(-64)<<char(-60)<<char(-39)<<char(-63)<<char(0);
		cout<<char(-63)<<char(-63)<<char(0)<<char(-63)<<char(-63)<<char(-39)<<char(-64)<<char(-39)<<char(-63)<<char(0)<<char(-63)<<char(-64)<<char(-60)<<char(-39)<<endl<<endl;
		cout<<" 1) Iniciar juego"<<endl;
		cout<<" 2) Como jugar"<<endl;
		cout<<" 3) Salir"<<endl;
		int opcionJuego;	
		cin>>opcionJuego;
		switch(opcionJuego){
			case 1:
				juego();
				break;
			case 2:
				regresar=false;
				while(regresar==false){
					comoJugar();
				}			
				break;
			case 3:
				sel=false;
				break;
			default:
				eleccionJuego();
				break;
		}
	}
}

int main(){
	system("color F0");
	eleccionJuego();
}
