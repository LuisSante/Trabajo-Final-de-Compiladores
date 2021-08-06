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
using namespace std;

#define NUM 258
#define AVA 259
#define RET 260
#define INI 261
#define FIN 262
#define IZQ 263
#define DER 264
#define ERROR 265
//#define FIN -1

// coordenada de incio (x y) del objeto
// y su direccion (0-3 = Norte Este Sur Oeste)
vector<int> punto = {4,4,0};
vector<vector<int>> vecPuntos(1,punto);

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


// funciones de desplazamiento en coordenadas -------------------------------

int lexemaToInt()
{
    string c;
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

void imprimirCoordenada(vector<int> p)
{
    cout << "punto : (" << p[0] << "," << p[1] << ") direccion : " << p[2] << endl;
}

void imprimirVecPuntos()
{
    cout << "VEC PUNTOS: "<< endl;
    for(int i = 0; i < vecPuntos.size(); i++)
        imprimirCoordenada(vecPuntos[i]);
}

void guardarCoordenada()
{
    vecPuntos.push_back(punto);
}


// funciones de analizador lexico-sintactico ----------------------------

void imprimirLexema()
{
    for(int i = 0; i < 10; i++)
        cout << lexema[i];
    cout << endl;
}

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
        lexema[i] = 0; // crea un espacio al final del numero en lexema
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
    cout << "hubo un error" << endl;
    exit(-1);
}

void Instruccion()
{
    if(tok == AVA)
    {
        parea(AVA);

        avanza(NUM);        // acciones semanticas
        imprimirCoordenada(punto);
        guardarCoordenada();

        parea(NUM);

    }
    else if(tok == RET)
    {
        parea(RET);

        retrocede(NUM);     // acciones semanticas
        imprimirCoordenada(punto);
        guardarCoordenada();

        parea(NUM);
    }
    else if(tok == IZQ)
    {
        izquierda();        // acciones semanticas
        imprimirCoordenada(punto);
        guardarCoordenada();

        parea(IZQ);
    }
    else if(tok == DER)
    {
        derecha();          // acciones semanticas
        imprimirCoordenada(punto);
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
        cout << "inicio de programa" << endl;
        Bloque();
        parea(FIN);
        cout << "fin de programa" << endl;
        return;
    }
    else
        error();
}

int main()
{
    string s, ins;
    int i = 0;

// recibe entrada y guarda en string
    do {
        s.clear();
        getline(cin,s);
        ins.append(s);
        ins.push_back(' ');
        i++;
    } while(s != "final");
    ins.pop_back(); // borar el ultimo salto

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
    imprimirCoordenada(punto);
    tok = scanner();
    Programa();
    imprimirVecPuntos();

    return 0;
}
