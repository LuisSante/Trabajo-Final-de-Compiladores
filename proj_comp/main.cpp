/******************************************************
Gramatica
    Programa --> inicio Bloque final
    Bloque --> Instruccion Bloque
    Bloque -->
    Instruccion --> avanza Numero
    Instruccion --> retrocede Numero
    Instruccion --> izquierda
    Instruccion --> derecha
    Numero --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ...

******************************************************/

#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <vector>

#define NUM 258
#define AVA 259
#define RET 260
#define INI 261
#define FIN 262
#define IZQ 263
#define DER 264
#define ERROR 265

#include <allegro.h>
#define MAXFILAS 10 // para eje y
#define MAXCOLS  17 // para eje x
#define UNIDAD   60
BITMAP *buffer;
BITMAP *roca;
BITMAP *ranabmp;
BITMAP *rana;

int dir = 0;
int px, py;

// ======== INICIALIZACION DE DATOS COMPILADOR ===================

// coordenada de incio (x y) del objeto
// y su direccion (0-3 = Norte Este Sur Oeste)
std::vector<int> punto = {7,8,0};
std::vector<std::vector<int>> vecPuntos(1,punto);

//variables analizador
char lexema[80];
int tok;
int cantDigitos = 0; // para contar cant digitos cuando tok = NUM
FILE *f;
void parea(int);
void error();
int espalres();
void Numero();
void Instruccion();
void Bloque();
int scanner();
void mostrar(int);


// ======== INICIALIZACION DE DATOS GRAFICOS ===================

//using namespace std;

char mapa[MAXFILAS][MAXCOLS] = { //19x30
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
"xxxxxxxxxxxxxxxx",
};

// ========== FUNCIONES DEL COMPILADOR =========================


// funciones de desplazamiento en coordenadas -------------------------------

int lexemaToInt()
{
    std::string c;
    int num;
    for(int i = 0; i < cantDigitos; i++)
        c[i] = lexema[i];
    num = atoi(c.c_str());
    return num;
}

void avanza(int n)
{
    if (tok == n)
    {
        int d = lexemaToInt();  // cantidad de posiciones para el DESPLAZAMIENTO
        int dir = punto[2];     // direccion
        cantDigitos = 0;

        if     (dir==0) punto[1]-=d; // avanza al norte
        else if(dir==1) punto[0]+=d; // avanza al este
        else if(dir==2) punto[1]+=d; // avanza al sur
        else if(dir==3) punto[0]-=d; // avanza al oeste
    }
    else
        error();
}

void retrocede(int n)
{
    if (tok == n)
    {
        int d = lexemaToInt();  // cantidad de posiciones para el DESPLAZAMIENTO
        int dir = punto[2];     // direccion
        cantDigitos = 0;

        if     (dir==0) punto[1]+=d; // avanza al norte
        else if(dir==1) punto[0]-=d; // avanza al este
        else if(dir==2) punto[1]-=d; // avanza al sur
        else if(dir==3) punto[0]+=d; // avanza al oeste
    }
    else
        error();

}

void izquierda()
{
    if ( (punto[2]-1) < 0 )
        punto[2] = 3;
    else
        punto[2]-=1;
}

void derecha()
{
    punto[2] = (punto[2]+1) % 4;
}

void imprimirCoordenada(std::vector<int> p)
{
    std::cout << "punto : (" << p[0] << "," << p[1] << ") direccion : " << p[2] << std::endl;
}

void imprimirVecPuntos()
{
    std::cout << "VEC PUNTOS: "<< std::endl;
    for(int i = 0; i < vecPuntos.size(); i++)
        imprimirCoordenada(vecPuntos[i]);
}

void guardarCoordenada()
{
    vecPuntos.push_back(punto);
}


// funciones de analizador lexico-sintactico ----------------------------

int espalres()
{
    if(strcmp(lexema,"avanza")==0) return AVA;
    if(strcmp(lexema,"retrocede")==0) return RET;
    if(strcmp(lexema,"inicio")==0) return INI;
    if(strcmp(lexema,"final")==0) return FIN;
    if(strcmp(lexema,"izquierda")==0) return IZQ;
    if(strcmp(lexema,"derecha")==0) return DER;
    if(strcmp(lexema,"error")==0) return ERROR;

    return -1;
}

int scanner()
{
    int c, i;
    do c=fgetc(f);
    while(isspace(c)); //ignora blancos
    if(c==EOF)
    {
        return EOF;
    }
    if(isalpha(c)) //regla de PALABRAS-RESERVADAS
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = fgetc(f);
        }
        while(isalpha(c));
        lexema[i]=0;
        ungetc(c,f); //se devuelve c al flujo de entrada
        i = espalres(); // verifica si es palabra reservada

        if(i >= 0)
            return i;
        return ERROR;
    }


    if(isdigit(c)) //regla del NUM
    {
        i=0;
        do
        {
            lexema[i++]=c;
            c=fgetc(f);
            cantDigitos++; // cuenta la cantidad de digitos en lexema
        }

        while(isdigit(c));
        lexema[i] = 0;
        ungetc(c,f);
        return NUM;
    }
    if (c=='\n')
        return FIN;
}

void mostrar(int token)
{
    switch(token)
    {
    case NUM: printf("token = NUM [%s]\n",lexema); break;
    case AVA: printf("token = AVA [%s]\n",lexema); break;
    case RET: printf("token = RET [%s]\n",lexema); break;
    case INI: printf("token = INI [%s]\n",lexema); break;
    case FIN: printf("token = FIN [%s]\n",lexema); break;
    case IZQ: printf("token = IZQ [%s]\n",lexema); break;
    case DER: printf("token = DER [%s]\n",lexema); break;
    }
}

void parea(int t)
{
    if (tok == t)
        tok=scanner();
    else
        error();
}

void error()
{
    std::cout << "hubo un error" << std::endl;
    exit(-1);
}

void Instruccion()
{
    if(tok == AVA)
    {
        parea(AVA);

        avanza(NUM);        // acciones semanticas
        guardarCoordenada();

        parea(NUM);

    }
    else if(tok == RET)
    {
        parea(RET);

        retrocede(NUM);     // acciones semanticas
        guardarCoordenada();

        parea(NUM);
    }
    else if(tok == IZQ)
    {
        izquierda();        // acciones semanticas
        guardarCoordenada();

        parea(IZQ);
    }
    else if(tok == DER)
    {
        derecha();          // acciones semanticas
        guardarCoordenada();

        parea(DER);
    }
}

void Bloque()
{
    if(tok == AVA || tok == RET || tok == IZQ || tok == DER)
    {
        Instruccion();
        Bloque();
    }
    else if(tok == '\n')
        return;
    else if(tok == FIN)
    {
        return;
    }
    else
        error();
}

void Programa()
{
    if(tok == INI)
    {
        parea(INI);
        Bloque();
        parea(FIN);
        return;
    }
    else
        error();
}


// =============== FUNCIONES GRAFICA ==============================


void dibujar_mapa(){
    int row,col;
    for(row = 0; row<MAXFILAS; row++)
    {
        for(col = 0; col < MAXCOLS; col++)
        {
            if(mapa[row][col] == 'x'){
                draw_sprite(buffer, roca, col*UNIDAD, row*UNIDAD);
            }
        }
    }
}

void iniciar()
{
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, UNIDAD*(MAXCOLS-1), UNIDAD*MAXFILAS, 0, 0);
}

void pantalla(){
    blit(buffer, screen, 0,0,0,0,UNIDAD*MAXCOLS, UNIDAD*MAXFILAS); // imprima todo en pantalla
}

void dibujar_pj(){
    blit(ranabmp,rana,dir*UNIDAD,0,0,0,UNIDAD,UNIDAD);
    draw_sprite(buffer,rana,px,py);
}


int main()
{
// ================ MAIN COMPILADOR ====================
    std::string s, ins;
    int i = 0;

// recibe entrada y guarda en string
    do {
        s.clear();
        getline(std::cin,s);
        ins.append(s);
        ins.push_back(' ');
        i++;
    } while(s != "final");
    ins.pop_back();

// inserta entrada en txt
    const char * cc = ins.c_str();
    FILE* archivo;
    archivo = fopen("entrada.txt","w");
    fprintf(archivo,cc);
    fclose(archivo);

// lee entrada desde txt
    f = fopen("entrada.txt","rt");
    //f = stdin; //entrada estandar del teclado

// ejecuta programa
    tok = scanner();
    Programa();

// ================== MAIN GRAFICA ==============================

    iniciar();
    buffer = create_bitmap(UNIDAD*MAXCOLS, UNIDAD*MAXFILAS);
    roca = load_bitmap("cuadro60x60.bmp",NULL);
    ranabmp = load_bitmap("rana60x60.bmp",NULL);
    rana = create_bitmap(UNIDAD,UNIDAD);

    int k = 0;
    int t = 2300;

    px = UNIDAD*vecPuntos[0][0];
    py = UNIDAD*vecPuntos[0][1];
    clear(buffer);
    dibujar_mapa();
    dibujar_pj();
    pantalla();
    textprintf(screen, font, ((MAXCOLS-2)*UNIDAD)/2,((MAXFILAS-1)*UNIDAD)/2, palette_color[14],"INICIO");
    rest(t); // hace pausa

    while(k < vecPuntos.size() && !key[KEY_ESC])
    {
        int x = vecPuntos[k][0];
        int y = vecPuntos[k][1];

        dir = vecPuntos[k][2];
        px = UNIDAD*x;
        py = UNIDAD*y;

        clear(buffer);
        dibujar_mapa();
        dibujar_pj();
        pantalla();
        rest(t);
        k++;
    }
    while(!key[KEY_ESC])
    {
        clear(buffer);
        dibujar_mapa();
        dibujar_pj();
        pantalla();
        textprintf(screen, font, ((MAXCOLS-3)*UNIDAD)/2,((MAXFILAS-1)*UNIDAD)/2,
                palette_color[14],"LLEGASTE A TU DESTINO");
        rest(t);
    }

    return 1;
}
END_OF_MAIN ()
