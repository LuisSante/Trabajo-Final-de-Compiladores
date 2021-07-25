/******************************************************
Gramatica
    Programa --> inicio Bloque final
    Bloque --> Instruccion Bloque
    Bloque -->
    Instruccion --> avanza Numero
    Instruccion --> retrocede Numero
    Instruccion --> izquierda
    Instruccion --> derecha
    Numero --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

******************************************************/

#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
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

char lexema[80];

int tok;
FILE *f;
void parea(int);
void error();

int scanner();

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
    if(c==EOF) return EOF;
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
        }
        while(isdigit(c));
        lexema[i]=0;
        ungetc(c,f);
        return NUM;
    }
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
    if (tok == FIN)
        return;
    else if (tok == t)
        tok=scanner();
    else
        error();
}

void error()
{
    cout << "hubo un error" << endl;
}

void Numero()
{
    if(tok == NUM)
        parea(NUM);
}

void Instruccion()
{
    if(tok == AVA)
    {
        parea(AVA);
        parea(NUM);
        cout << "avanza num" << endl;
    }
    else if(tok == RET)
    {
        parea(RET);
        parea(NUM);
        cout << "retrocede num;" << endl;
    }
    else if(tok == IZQ)
    {
        parea(IZQ);
        cout << "se mueve a izq" << endl;
    }
    else if(tok == DER)
    {
        parea(DER);
        cout << "se mueve a der" << endl;
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
    f = stdin; //entrada estandar del teclado
    tok = scanner();
//    while(1)
//    {
//        tok = scanner();
//        if(tok == EOF) break;
//        mostrar(tok);
//    }
    Programa();
    return 0;
}



