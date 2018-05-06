//
// Created by franek on 18-5-1.
//

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "arrayList.h"


int main(){
  ArrayListNode* aln1 = newArrayListNode("HWDPJP100\%\n");
  assert(strcmp(aln1->value, "HWDPJP100\%\n") == 0);
  printf("%s\n", (char*)aln1->value);

  ArrayList* al = newArrayList();
  assert(isArrayListEmpty(al));

  arrayListAdd(al, aln1);
  arrayListAdd(al, newArrayListNode("JP100\%HWDP\n"));
  printf("%s %zu\n", phnumGet(al, 1), al->phNums[1]->pos);
  assert(strcmp(phnumGet(al, 1), "JP100\%HWDP\n") == 0);
  printf("%s %zu\n", phnumGet(al, 0), al->phNums[0]->pos);
  assert(strcmp(phnumGet(al, 0), "HWDPJP100\%\n") == 0);
  arrayListAdd(al, newArrayListNode("Angie White"));

  stringArrayListSort(al);

  printf("%s\n", phnumGet(al, 0));
  assert(strcmp(phnumGet(al, 0), "Angie White") == 0);
  printf("%zu\n", aln1->pos);
  assert(aln1->pos == 1);



  printf("%zu %zu\n", al->phNumCount, al->arrSize);
  arrayListCutOut(al, aln1);
  printf("%zu %zu\n", al->phNumCount, al->arrSize);
  assert(strcmp(phnumGet(al, 1), "JP100\%HWDP\n") == 0);
  assert(isArrayListEmpty(al) == false);

  arrayListDelete(al);



}