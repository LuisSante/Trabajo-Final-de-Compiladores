#include<iostream>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>

#define ESC         27

int cuerpo[200][2];
char tecla;
int n = 1;
int size = 3;
int dir = 3;
int velocidad = 100, h=1;

void gotoxy(int x, int y){ //funcion que posiciona
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;

    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon,dwPos);//hacemos uso de la consola
}

void paint_border(){//delimita un borde para nuestra graficadora
    //lineas orizontales
    for(int i=2 ; i < 78 ; i++){
        gotoxy(i,3);
        std::printf("%c",205);
        gotoxy(i,23);
        std::printf("%c",205);
    }

    //lineas verticales
    for(int i=4 ; i < 23 ; i++){
        gotoxy(2,i);
        std::printf("%c",186);
        gotoxy(77,i);
        std::printf("%c",186);
    }

    //esquinas
    gotoxy(2,3);
    std::printf("%c",201);
    gotoxy(2,23);
    std::printf("%c",200);
    gotoxy(77,3);
    std::printf("%c",187);
    gotoxy(77,23);
    std::printf("%c",188);
}

void guardar_pos(int pos_x , int pos_y){//posiciona donde queremos el inicio de la grafica
    cuerpo[n][0] = pos_x;
    cuerpo[n][1] = pos_y;
    n++;

    if(n == size){n = 1;}
}

void dibujar_cuerpo(){//un cuerpo se  dibuja *******
	
	for(int i=0; i<size; i++){
        gotoxy(cuerpo[i][0], cuerpo[i][1]);
        std::cout<<"*";
    }

}

int main(){
	int pos_x , pos_y;
	std::cout<<"Posicion x para iniciar: ";std::cin>>pos_x;
	std::cout<<"Posicion y para iniciar: ";std::cin>>pos_y;
    paint_border();//dibuja el borde

    while(tecla != ESC){
        guardar_pos(pos_x , pos_y);
        dibujar_cuerpo();

        if(dir == 1){pos_y--;}
        if(dir == 2){pos_y++;};
        if(dir == 3){pos_x++;};
        if(dir == 4){pos_x--;}

        Sleep(velocidad);//delimitamos la velocidad de la aparicion de lso *
    }
    system("pause>null");
    return 0;
}

