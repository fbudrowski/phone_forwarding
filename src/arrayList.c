//
// Created by franek on 18-4-29.
//


#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



typedef struct ArrayListNode {
    size_t pos;
    bool valueToBeFreed;
    void * value;
} ArrayListNode;

typedef struct PhoneNumbers {

    size_t arrSize;
    size_t phNumCount;
    ArrayListNode ** phNums;

} PhoneNumbers;

typedef PhoneNumbers ArrayList;

ArrayList* newArrayList (){
  ArrayList* al = malloc(sizeof(ArrayList));
  if (al == NULL){
    return al;
  }
  al->arrSize = 2;
  al->phNumCount = 0;
  al->phNums = malloc(al->arrSize * sizeof(ArrayListNode*));
  if (al->phNums == NULL){
    free(al);
    return NULL;
  }
  return al;
}

void const * arrayListGet (ArrayList const * al, size_t idx) {
  if (al == NULL || al->phNums == NULL || idx >= al->phNumCount) {
    return NULL;
  }
  return al->phNums[idx]->value;
}

char const * phnumGet (PhoneNumbers const * pnum, size_t idx) {
  return (char const*) arrayListGet(pnum, idx);
}

bool arrayListAdd (ArrayList* al, ArrayListNode* node){
  if (al == NULL){
    return false;
  }
  if (al->arrSize <= al->phNumCount){
    al->arrSize *= 2;
    al->phNums = realloc(al->phNums, (al->arrSize) * sizeof(ArrayListNode*));
    if (al->phNums == NULL){
      return false;
    }
  }
  size_t count = al->phNumCount;

  al->phNums[count] = node;
  node->pos = count;
  al->phNumCount++;
  return true;
}

ArrayListNode* newArrayListNode(char const * text){
  ArrayListNode* node = malloc(sizeof(ArrayListNode));
  if (node == NULL){
    return NULL;
  }
  node->value = malloc(sizeof(char) * (strlen(text) + 1));
  if (node->value == NULL){
    free(node);
    return NULL;
  }
  node->valueToBeFreed = true;
  memcpy(node->value, text, sizeof(char) * (strlen(text) + 1));
  node->pos = 0;
  return node;
}

/**
 * Eliminuje dany node z pamięci.
 * @param aln - node do usunięcia.
 */

void arrayListNodeDelete(ArrayListNode * aln){
  if (aln != NULL){
    //printf("%s\n", (char*)(aln->value));
    if (aln->valueToBeFreed){
      free((char*)(aln->value));
    }
    free(aln);
  }
}

void arrayListDelete (ArrayList * al) {
  if (al == NULL){
    return;
  }
  for (size_t i = 0; i < al->phNumCount; i++){
    arrayListNodeDelete(al->phNums[i]);
  }
  free(al->phNums);
  free(al);
}

void phnumDelete (PhoneNumbers* pnum) {
  arrayListDelete(pnum);
}

void arrayListCutOut (ArrayList * al, ArrayListNode* aln) {
  if (al == NULL) return;
  al->phNums[aln->pos] = al->phNums[al->phNumCount-1];
  al->phNums[aln->pos]->pos = aln->pos;
  al->phNumCount--;
  al->phNums[al->phNumCount] = NULL;

  arrayListNodeDelete(aln);

  if (al->phNumCount > 2 && al->phNumCount * 4 < al->arrSize){
    al->arrSize /= 2;
    al->phNums = realloc(al->phNums, al->arrSize * sizeof(ArrayListNode*));
  }
}

bool isArrayListEmpty (ArrayList * al){
  return (al == NULL || al->phNumCount == 0);
}

/**
 * Porównuje leksykograficznie dwa ArrayListNode'y zawierające stringi (char*) odpowiednio do funkcji qsort.
 * @param n1 Jeden string
 * @param n2 Drugi string
 * @return Pasująca do qsorta wartość porównania.
 */

int stringArrayListNodeCompare(const void *n1, const void *n2){
  bool n1Null = (n1 == NULL || (*((ArrayListNode**)n1))->value == NULL);
  bool n2Null = (n2 == NULL || (*((ArrayListNode**)n2))->value == NULL);
  char* str1 = (char*)(*((ArrayListNode**)n1))->value;
  char* str2 = (char*)(*((ArrayListNode**)n2))->value;
  //printf("Compare %s with %s (%d %d) %d\n", str1, str2, n1Null?1:0, n2Null?1:0, (n1Null || n2Null) ? 0 : strcmp(str1, str2));
  if (n1Null && n2Null){
    return 0;
  }
  if (n1Null){
    return 1;
  }
  if (n2Null){
    return -1;
  }
  return strcmp(str1, str2);
}

void stringArrayListSort (ArrayList * al){
  if (al == NULL || al->phNums == NULL){
    return;
  }
  qsort(al->phNums, al->phNumCount, sizeof(ArrayListNode *), stringArrayListNodeCompare);
  for(size_t i = 0; i < al->phNumCount; i++){
    al->phNums[i]->pos = i;
    //printf("SORT %zu %s\n", al->phNums[i]->pos, (char*)al->phNums[i]->value);
  }

}