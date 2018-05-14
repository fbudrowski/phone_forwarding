#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward.h"

#define MAX_LEN 23


char* getRandomNumber(){
  char* answer = calloc(12, sizeof(char));
  for (size_t j = 0; j < 10; j++){
    answer[j] = (char)'0' + (char)(rand() % 10);
  }
  return answer;
}

int main() {
  char num1[MAX_LEN + 1], num2[MAX_LEN + 1];
  struct PhoneForward *pf;
  struct PhoneNumbers const *pnum;
  char const *num;
  size_t idx;
  bool result;

  pf = phfwdNew();

  strcpy(num1, "123");
  strcpy(num2, "9");
  result = phfwdAdd(pf, num1, num2);
  assert(result);
  (void)result;

  pnum = phfwdGet(pf, "1234");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 94.
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = phfwdGet(pf, "12");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 12.
  phnumDelete((struct PhoneNumbers *)pnum);

  strcpy(num1, "123456");
  strcpy(num2, "777777");
  result = phfwdAdd(pf, num1, num2);
  assert(result);
  (void)result;

  pnum = phfwdGet(pf, "12345");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 945.
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = phfwdGet(pf, "123456");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 777777.
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = phfwdGet(pf, "997");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 997.
  phnumDelete((struct PhoneNumbers *)pnum);



  pnum = phfwdReverse(pf, "987654321");
  idx = 0;
  // Wypisuje:
  // 12387654321
  // 987654321
  while ((num = phnumGet(pnum, idx++)) != NULL)
    printf("%s\n", num);
  phnumDelete((struct PhoneNumbers *)pnum);

  phfwdRemove(pf, "12");

  pnum = phfwdGet(pf, "123456");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 123456.
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = phfwdReverse(pf, "987654321");
  idx = 0;
  // Wypisuje:
  // 987654321
  while ((num = phnumGet(pnum, idx++)) != NULL)
    printf("%s\n", num);
  phnumDelete((struct PhoneNumbers *)pnum);

  result = phfwdAdd(pf, "567", "0");
  assert(result);
  (void)result;

  result = phfwdAdd(pf, "5678", "08");
  assert(result);
  (void)result;

  pnum = phfwdReverse(pf, "08");
  idx = 0;
  // Wypisuje:
  // 08
  // 5678
  while ((num = phnumGet(pnum, idx++)) != NULL)
    printf("%s\n", num);
  phnumDelete((struct PhoneNumbers *)pnum);

  result = phfwdAdd(pf, "A", "1");
  assert(!result);
  (void)result;

  result = phfwdAdd(pf, "1", "A");
  assert(!result);
  (void)result;

  phfwdRemove(pf, "");
  phfwdRemove(pf, NULL);

  pnum = phfwdGet(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = phfwdReverse(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete((struct PhoneNumbers *)pnum);

  pnum = NULL;
  phfwdDelete(pf);
  pf = NULL;


  pf = phfwdNew();
  srand(33421);
  for (size_t i = 0; i < 151001000; i++){
    if (i % 100000 == 0){
      printf("Line %zu:\n", i);
    }
    char* source = getRandomNumber();
    char* target = getRandomNumber();
    phfwdAdd(pf, source, target);
    pnum = phfwdGet(pf, source);
    assert (phnumGet(pnum, 0) != 0);
    phnumDelete((struct PhoneNumbers *)pnum);

    pnum = phfwdReverse(pf, target);
    size_t sameCount = 0;
    for (size_t j = 0; true; j++){
      char const * ans = phnumGet(pnum, j);
      if (ans == NULL){
        break;
      }
      if (strcmp(ans, source) == 0){
        sameCount++;
      }
    }
    phnumDelete((struct PhoneNumbers *)pnum);
    pnum = NULL;
    assert(sameCount > 0);

    free(source);
    free(target);
  }

  phfwdDelete(pf);

  return 0;
}
