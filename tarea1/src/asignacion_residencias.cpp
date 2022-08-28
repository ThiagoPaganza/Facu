/* 50250037 */

#include "../include/asignacion_residencias.h"
#include "../include/pila.h"
#include "../include/asignacion.h"
#include <cstddef>

Asignacion asignarResidencias(nat m, nat* C, nat n, nat** hPrefs, nat** ePrefs)
{
    Asignacion salida = crear_asignacion();
    Pila hosLibres = crear_pila();
    nat * next = new nat[m];
    int * current = new int[n];
    nat ** ranking = new nat*[n];
    for (nat i = 0; i < m; i++){
      apilar(i, hosLibres);
      next[i] = 0;
    }
    for (nat i = 0; i < n; i++){
      current[i] = -1;
      ranking[i] = new nat[m];
      for (nat j = 0; j < m; j++){
        ranking[i][ePrefs[i][j]] = j;
      }
    }
    int hosActual;
    hosActual = cima(hosLibres);
    nat estActual = hPrefs[hosActual][next[hosActual]];
    while (C[hosActual] > 0){
      if (current[estActual] == -1){
        current[estActual] = hosActual;
        C[hosActual]--;
        if (C[hosActual] == 0) desapilar(hosLibres);
      }
      else{
        if (ranking[estActual][hosActual] < ranking[estActual][current[estActual]]){ //menos es mejor rankeado
          C[current[estActual]]++;
          C[hosActual]--;
          if (C[hosActual] == 0) desapilar(hosLibres);
          apilar(current[estActual], hosLibres);
          current[estActual] = hosActual;
        }
      }
      next[hosActual]++;
      if (next[hosActual] >= n || es_vacia_pila(hosLibres)) break;
      else {
        hosActual = cima(hosLibres);
        estActual = hPrefs[hosActual][next[hosActual]];
      }
    }
    par newPar;
    for (nat i = 0; i < n; i++){
      newPar.eid = i;
      newPar.hid = current[i];
      insertar_par(newPar, salida);

      delete[] ranking[i];
    }
    destruir_pila(hosLibres);
    delete[] next;
    delete[] current;
    delete[] ranking;
    return salida;
}
