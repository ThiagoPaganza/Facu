/*
  Módulo de implementación de `cronologia_consistente'.

  Laboratorio de Programación 3.
  InCo-FIng-UDELAR
*/
#include <stdio.h>

#include "../include/cronologia_consistente.h"
#include"../include/grafo.h"
#include"../include/lista.h"

Cronologia cronologia_consistente(nat n, ListaDatos tipo1, ListaDatos tipo2)
{
  bool existe_cr_consistente = false; 
  Cronologia cr = new evento[2*n]; 
  nat b_i,d_i,b_j,d_j;
  nat current, next;
  nat i = 0;
  Lista adyacencias;
  Grafo G = crear_grafo(2*n, "dirigido");
  int* cantAristas = new int[2*n+1];
  for (nat i = 1; i <= n; i++){
    agregar_vertice(i + n, G);
    agregar_vertice(i, G);
    b_i = i;
    d_i = i + n;
    agregar_arista(b_i, d_i, G);
    cantAristas[b_i] = 0;
    cantAristas[d_i] = 1;
  }
  while (!es_vacia_lista_datos(tipo2)){
    b_j = id2(primer_dato(tipo2));
    b_i = id1(primer_dato(tipo2));
    d_j = b_j + n;
    d_i = b_i + n;
    agregar_arista(b_j, d_i,G);
    agregar_arista(b_i, d_j,G);
    tipo2 = resto_datos(tipo2);
    cantAristas[d_i]++;
    cantAristas[d_j]++;
  }
  while (!es_vacia_lista_datos(tipo1)){
    b_j = id2(primer_dato(tipo1));
    d_i = id1(primer_dato(tipo1)) + n;
    agregar_arista(d_i, b_j, G);
    tipo1 = resto_datos(tipo1);
    cantAristas[b_j]++;
  }
  Lista S = crear_lista();
  for (nat k = 1; k <= n; k++) if (cantAristas[k] == 0) insertar_al_inicio(k, S); 
  while (!es_vacia_lista(S)){
      current = primero(S);
      remover_al_inicio(S);
      if (current <= n) {
        cr[i].tipo = nace; 
        cr[i].id = current;
      } else {
        cr[i].tipo = muere; 
        cr[i].id = current - n;
      }
      adyacencias = adyacentes(current, G);
      i++;
      for (nat k = 1; k <= out_grado(current, G); k++){
        comienzo(adyacencias);
        next = actual(adyacencias);
        cantAristas[next]--;
        if (cantAristas[next] == 0) insertar_al_final(next, S);
        remover_al_inicio(adyacencias);
      }
  }
  if (i == 2*n) existe_cr_consistente = true;
  destruir_grafo(G);
  destruir_lista(S);
  delete [] cantAristas;
  if(existe_cr_consistente) {
    return cr;
  } else {  
    delete [] cr;
    return NULL;
  }
} 
