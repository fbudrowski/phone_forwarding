#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "phone_forward.h"

#define MAX_LEN 23

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
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "12");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 12.
  phnumDelete(pnum);

  strcpy(num1, "123456");
  strcpy(num2, "777777");
  result = phfwdAdd(pf, num1, num2);
  assert(result);
  (void)result;

  pnum = phfwdGet(pf, "12345");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 945.
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "123456");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 777777.
  phnumDelete(pnum);

  pnum = phfwdGet(pf, "997");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 997.
  phnumDelete(pnum);



  pnum = phfwdReverse(pf, "987654321");
  idx = 0;
  // Wypisuje:
  // 12387654321
  // 987654321
  while ((num = phnumGet(pnum, idx++)) != NULL)
    printf("%s\n", num);
  phnumDelete(pnum);

  phfwdRemove(pf, "12");

  pnum = phfwdGet(pf, "123456");
  printf("%s\n", phnumGet(pnum, 0)); // Wypisuje 123456.
  phnumDelete(pnum);

  pnum = phfwdReverse(pf, "987654321");
  idx = 0;
  // Wypisuje:
  // 987654321
  while ((num = phnumGet(pnum, idx++)) != NULL)
    printf("%s\n", num);
  phnumDelete(pnum);

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
  phnumDelete(pnum);

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
  phnumDelete(pnum);

  pnum = phfwdReverse(pf, "A");
  assert(phnumGet(pnum, 0) == NULL);
  phnumDelete(pnum);

  phfwdDelete(pf);

  pnum = NULL;
  phnumDelete(pnum);
  pf = NULL;
  phfwdDelete(pf);

  return 0;
}
