/*
 * ListaDoble.c
 *
 *  Created on: 28/02/2013
 *      Author: Andres
 *      Definicion: Las pilas y colas son tipos de listas
 *      			la pilas tienen un comportamiento LIFO
 *      			las colas un comportamiento FIFO
 *      			En este ejercico la lista se comporta como cola o pila segun se desee insertando
 *      			por el inicio(pila-LIFO o cola -FIFO) o el final.El hecho de sacar elementos
 *      			por el inicio o el final hace que se puedan tener los dos comportamientos(pila,cola).
 *      			Las estructuras basicas de datos que permiten esto tener:
 *      				Una lista con puntero a incio i fin.
 *      				Los nodos estan enlazados con el nodo que les antecede y con el que le precede.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Variables miembro y estrcturas
static int calcSigId=10;
typedef struct Producto_t Producto;
typedef struct Nodo_t Nodo;
typedef struct Lista_t Lista;

typedef enum eSentido{DIRECTO,INVERSO}Sentido;


struct Producto_t
{
	int id;
	char nombre[51];
	float coste;
	float venta;
	int boolCatalogo; // esta o no en catalogo
};

struct Nodo_t
{
	//Pregunta que es mas optimo o usual que el producto este dentro o que sea un puntero?
	Producto producto;
	Nodo* pAnterior;
	Nodo* pPosterior;
};

struct Lista_t
{
	Nodo* pInicio;
	Nodo* pFinal;
};

//Prototipado
//Con insertaNodoInicio el comportamiento es tipo PILA dado que el ultimo insertado queda el primero(LIFO)
void insertaNodoInicio(Producto* producto, Lista* lista);
//Con insertaNodoFinal el comportamiento es tipo LISTA dado que el ultimo insertado queda al final de la lista(FIFO)
void insertaNodoFinal(Producto* producto, Lista* lista);
void muestraProducto(Producto* producto, FILE* salida);
// Retorna el numero de elementos recorridos, saca la lista por consola o fichero si se indica, el sentido
// hara que se muestre le lista de incio a fin (DIRECTO) o de fin a inicio (INVERSO)
int recorreLista(Lista* lista, FILE* salida, int condicion, Sentido recorrido, Producto* copia, Producto* valorOrdenado);
void eliminaNodoFinal(Lista* lista);
void eliminaNodoInicio(Lista* lista);
int listaLLena(Lista* lista);
Producto* pideProducto();

void ordenaProductos(Lista* lista);
int colocar(Producto *v, int b, int t);
void quickSort(Producto* v, int b, int t);

//Programa
int main(int argc, char **argv)
{
	Lista milista= {NULL,NULL};
	ordenaProductos(&milista);
	system("PAUSE");
	Producto p1 = {10,"producto1",1.1,1.1,1};
	Producto p2 = {10,"producto2",2.1,1.1,1};
	Producto p3 = {10,"producto3",3.1,1.1,1};
	Producto p4 = {10,"producto4",4.1,1.1,1};
	Producto misproductos[4]={p1,p2,p3,p4};
	int i=0;
	for (i = 0; i < 4; ++i)
	{
		//muestraProducto(&misproductos[i],stdout);
		insertaNodoFinal(&misproductos[i],&milista);
	}

	//Menu
	int opcion;
	char nomfichero[120]="";
	do {
		do
		{
			system("cls");
			printf("\n OPCIONES");
			printf("\n1. Insercion de nodo por el inicio");
			printf("\n2. Insercion de nodo por el final");
			printf("\n3. Eliminacion del primer nodo");
			printf("\n4. Eliminacion del ultimo nodo");
			printf("\n5. Ver longitud de la lista");
			printf("\n6. Ver estado de la lista");
			printf("\n7. Recorrido de la lista");
			printf("\n8. Guardar en fichero");
			printf("\n9. Eliminar lista");
			printf("\n10. Salir\n");
			scanf("%d",&opcion);
			system("cls");
		} while (opcion<0 || opcion>11);
		switch (opcion) {
			case 1:insertaNodoInicio(pideProducto(),&milista);
				break;
			case 2:insertaNodoFinal(pideProducto(),&milista);
				break;
			case 3:eliminaNodoInicio(&milista);
				break;
			case 4:eliminaNodoFinal(&milista);
				break;
			case 5:printf("\nTienes %d elementos en tu lista\n",recorreLista(&milista,NULL,0,DIRECTO,NULL,NULL));system("PAUSE");
				break;
			case 6:
				if (listaLLena(&milista))
				{
					printf("\nNo caben mas elementos la lista esta llena");
				}
				else
				{
					printf("\nSe pueden insertar mas elementos, la lista no esta llena.\n");
				}
				printf("Tienes %d elementos en tu lista.\n",recorreLista(&milista,NULL,0,DIRECTO,NULL,NULL));
				system("PAUSE");
				break;
			case 7:recorreLista(&milista,stdout,0,DIRECTO,NULL,NULL);system("PAUSE");
				break;
			case 8:
				printf("Indica un nombre para el fichero donde se gauradaran los productos: ");
				scanf("%s",&nomfichero[0]);
				FILE * pfichero;
				pfichero = fopen (nomfichero , "w");
				if (pfichero == NULL) perror ("Error al crear fichero");
				else
				{
					recorreLista(&milista,pfichero,0,DIRECTO,NULL,NULL);
					fclose (pfichero);
				};
				break;

			case 9:
				while(milista.pFinal != NULL)
				{
					//Como se pretende eliminar toda la lista es indiferente
					//utilizar eliminaNodoFinal eliminaNodoInicio
					eliminaNodoFinal(&milista);
				}
				printf("Han sido eliminados todos los elementos de la lista.");
				break;

			default:
				break;
		}
//		printf("\n***********recorre lista sentido directo***************");
//		recorreLista(&milista,stdout,0,DIRECTO);
//		printf("\n***********recorre lista sentido inverso***************");
//		recorreLista(&milista,stdout,0,INVERSO);
//		printf("\n***********primero lista*******************************");
//		muestraProducto(&milista.pInicio->producto,stdout);
//		printf("\n***********final   lista*******************************");
//		muestraProducto(&milista.pFinal->producto,stdout);
	} while (opcion!=10);
	return 0;
}
void ordenaProductos(Lista* lista)
{
	int numProductosListas = recorreLista(lista, NULL,0,DIRECTO,NULL,NULL);
	Producto misProductos[numProductosListas];
	recorreLista(lista, NULL,0,DIRECTO,&misProductos[0],NULL);
	quickSort(&misProductos[0],0,5);
	recorreLista(lista, NULL,0,DIRECTO,NULL,&misProductos[0]);
}
int colocar(Producto *v, int b, int t)
{
	int i;
	int pivote;
	Producto valor_pivote;
	Producto temp;

	pivote = b;
	valor_pivote = v[pivote];
	for (i=b+1; i<=t; i++)
	{
		if (v[i].id < valor_pivote.id)
		{
			pivote++;
			temp=v[i];
			v[i]=v[pivote];
			v[pivote]=temp;
		}
    }
	temp=v[b];
	v[b]=v[pivote];
	v[pivote]=temp;
	return pivote;
}

void quickSort(Producto* v, int b, int t)
{
	int pivote;
	if(b < t)
	{
		pivote=colocar(v, b, t);
		quickSort(v, b, pivote-1);
		quickSort(v, pivote+1, t);
	}
}

int recorreLista(Lista* lista, FILE* salida, int condicion, Sentido recorrido, Producto* copia, Producto* valorOrdenado)
{
	int contador=0;
	Nodo* pNodoActual=(recorrido==DIRECTO?lista->pInicio:lista->pFinal);
	while(pNodoActual!=NULL)
	{
		if (pNodoActual->producto.id> condicion)
		{
			contador++;
			if (salida!=NULL)
			{
				if (copia !=NULL)
				{
					*(copia+contador)= pNodoActual->producto;
				}
				if (valorOrdenado !=NULL)
				{
					pNodoActual->producto=*(valorOrdenado+contador);
				}
				muestraProducto(&pNodoActual->producto,salida);
				fprintf(salida,"\nEl puntero del Nodo es 		:%p", pNodoActual);
				fprintf(salida,"\nEl puntero del anterior es	: %p", pNodoActual->pAnterior);
				fprintf(salida,"\nEl puntero del siguiente es	: %p", pNodoActual->pPosterior);
			}
		}
		pNodoActual=(recorrido==DIRECTO?pNodoActual->pPosterior:pNodoActual->pAnterior);
	}
	return contador;
}

int listaLLena(Lista* lista)
{
	Nodo* nTemp =  (Nodo*)malloc((sizeof(Nodo))*1);
	if (nTemp==NULL) {
		//Si no se ha creado el puntero la memoria esta llena
		//por tanto no caben mas Nodos y la lista esta llena (TRUE=1)
		return 1;
	}else
	{
		free(nTemp);
		// caben mas elementos por tanto la lista no esta llena (False=0)
		return 0;
	}
}
void eliminaNodoFinal(Lista* lista)
{
	if (lista->pFinal==NULL || lista->pInicio==NULL)
	{
		printf("\nLa lista esta vacia");
		system("PAUSE");
	}
	else
	{
		if (lista->pFinal->pAnterior==NULL)
		{
			//solo queda un nodo
			free(lista->pFinal);
			lista->pFinal=NULL;
			lista->pInicio=NULL;
		}else
		{
			lista->pFinal= lista->pFinal->pAnterior;
			free(lista->pFinal->pPosterior);
			lista->pFinal->pPosterior=NULL;
		}
	}
}

void eliminaNodoInicio(Lista* lista)
{
	if (lista->pFinal==NULL || lista->pInicio==NULL)
	{
		printf("\nLa lista esta vacia");
		system("PAUSE");
	}
	else
	{
		if (lista->pInicio->pPosterior==NULL)
		{
		//solo queda un nodo
		free(lista->pInicio);
		lista->pFinal=NULL;
		lista->pInicio=NULL;
		}
		else
		{
			lista->pInicio= lista->pInicio->pPosterior;
			free(lista->pInicio->pAnterior);
			lista->pInicio->pAnterior=NULL;
		}
	}
}

//Implementacion de metodos y funciones

void muestraProducto(Producto* producto, FILE* salida)
{
	fprintf(salida,"\nId		: %d",producto->id);
	fprintf(salida,"\nNombre	: %s",producto->nombre);
	fprintf(salida,"\nCoste		: %.2f",producto->coste);
	fprintf(salida,"\nVenta		: %.2f",producto->venta);
	fprintf(salida,"\nCatalogado: %d",producto->boolCatalogo);
}

void insertaNodoInicio(Producto* producto, Lista* lista)
{
	producto->id=calcSigId;
	calcSigId+=10;
	Nodo* nTemp =  (Nodo*)malloc((sizeof(Nodo))*1);
	nTemp->producto= *(producto);
	free(producto);
	nTemp->pAnterior=NULL;
	nTemp->pPosterior=lista->pInicio;
	if (lista->pInicio!=NULL) {
		lista->pInicio->pAnterior=nTemp;
	}else {
		lista->pFinal=nTemp;
	}

	lista->pInicio=nTemp;
}
void insertaNodoFinal(Producto* producto, Lista* lista)
{
	producto->id=calcSigId;
	calcSigId+=10;
	Nodo* nTemp =  (Nodo*)malloc((sizeof(Nodo))*1);
	nTemp->producto= *(producto);
	free(producto);
	nTemp->pPosterior=NULL;
	nTemp->pAnterior=lista->pFinal;
	if (lista->pFinal!=NULL) {
		lista->pFinal->pPosterior=nTemp;
	}else {
		lista->pInicio=nTemp ;
	}
	lista->pFinal=nTemp;
}

Producto* pideProducto()
{
	system("cls");
	printf("\n**********Insertar datos del producto*************\n");
	Producto* temp= NULL;
	temp = (Producto*)malloc((sizeof(Producto))*1);
	char sNombre[51]="";
	float scoste=0.0;
	float sventa=0.0;
	int sBoolCatalogo;
	printf("Introduce el nombre del producto:");
	scanf("%s",&sNombre[0]);
	printf("Introduce el precio de coste:");
	scanf("%f",&scoste);
	printf("Introduce el precio de venta:");
	scanf("%f",&sventa);
	printf("Esta en catalogo(si=1,no=0):");
	scanf("%d",&sBoolCatalogo);
	strcpy(temp->nombre, sNombre);
	temp->coste= scoste;
	temp->venta= sventa;
	temp->boolCatalogo= sBoolCatalogo;
	return temp;
}

