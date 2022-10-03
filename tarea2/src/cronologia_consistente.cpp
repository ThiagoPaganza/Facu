/* 5031022 */

/*
  Módulo de implementación de `cronologia_consistente'.
  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/
#include <stdio.h>

#include "../include/cronologia_consistente.h"
#include "../include/grafo.h"
#include "../include/lista_datos.h"

Cronologia cronologia_consistente(nat n, ListaDatos tipo1, ListaDatos tipo2)
{
  bool existe_cr_consistente = false; 

  Cronologia cr = new evento[2*n]; 

  //crear Grafo G=(V,E) donde V={N1,...,Nn,M1,...,Mn} y E={(a,b) sii a sucedio antes que b}
  Grafo G = crear_grafo(2*n, "dirigido"); //  O(2n) = O(n)

  nat Mi,Nj,Mj,Ni;
  Lista S = crear_lista(); // Conjunto de nodos activos sin aristas incidentes desde otros nodos activos.

  int* cant_aristas = new int[2*n+1]; //cant_aristas_incidentes_desde_nodos_activos

  for (nat i=1; i<=n; i++){ // O(n)
    agregar_vertice( i, G);
    agregar_vertice( i+n, G);

    //tipo3 o trivial, cada persona nace antes de morir ==> (Ni,Mi)
    Ni = i;
    Mi = i+n;
    agregar_arista( Ni, Mi, G);

    cant_aristas[Ni] = 0;
    cant_aristas[Mi] = 1;
  }

  //tipo1, i murio antes de que j nazca ==> (Mi,Nj) 
  while (!es_vacia_lista_datos(tipo1)){ //O(m1)
    Mi = id1(primer_dato(tipo1)) + n;
    Nj = id2(primer_dato(tipo1));

    agregar_arista( Mi, Nj, G);
    tipo1 = resto_datos(tipo1);

    cant_aristas[Nj] ++;
  }

  //tipo2, i j se solaparon ==> (Ni,Mj) y (Nj,Mi) 
  while (!es_vacia_lista_datos(tipo2)){ //O(m2)
    Ni = id1(primer_dato(tipo2));
    Nj = id2(primer_dato(tipo2));
    Mi = Ni + n;
    Mj = Nj + n;

    agregar_arista( Ni, Mj,G);
    agregar_arista( Nj, Mi,G);
    tipo2 = resto_datos(tipo2);

    cant_aristas[Mj] ++;
    cant_aristas[Mi] ++;
  }

  //  O(m1+m2 = m)

  //Hallar ordenamiento topologico
 
  nat nodo, vecino;
  nat i = 0;  //indice de la cronologia
  Lista ady;

  for (nat j=1; j<=n; j++) {  //  O(n)
    if (cant_aristas[j] == 0) insertar_al_inicio(j, S);
  }

  while ( !es_vacia_lista(S) ){
      nodo = primero(S);
      remover_al_inicio(S);

      if (nodo > n) {
        cr[i].id = nodo - n;
        cr[i].tipo = muere; 
      } else {
        cr[i].id = nodo;
        cr[i].tipo = nace; 
      }
      i++;

      ady = adyacentes(nodo, G);
      for (nat m=1; m <= out_grado(nodo, G); m++){  // para cada nodo vecino del cual es padre ==> O(n_v)
        comienzo(ady);
        vecino = actual(ady);

        cant_aristas[vecino] --;
        if (cant_aristas[vecino] == 0) insertar_al_final( vecino, S);

        remover_al_inicio(ady); //aprovecho y voy borrando las aristas del grafo
      }
  }

  if ( i == 2*n ) existe_cr_consistente = true; // se le pudo dar un orden a todos los nodos.

  destruir_lista(S);  //  O(1) pues sale vacia
  delete [] cant_aristas; //  O(2n) = O(n);
  destruir_grafo(G);  //  O(n + m)
  
  if(existe_cr_consistente) { // si existe, retornarla en cr
    return cr;
  } else {// si no existe, liberar la memoria asociada a cr y retornar NULL   
    delete [] cr;
    return NULL;
  }


  //Ideas o seteo S como todos los Ni en el primer for, y en cada while los saco de S si le entra alguna arista,
  //o hago un for desde 1 hasta n despues de los while chequeando si incidentes desde activos es 0
  //debo chequear para no poner aristas dos veces?
  //ejemplo: tengo 1,2 tipo 1, (M1,N2) y tengo 1 y 2 tipo 2 (M1,N2) 
  //la arista se pondria dos veces. Molesta? Da igual?
} 