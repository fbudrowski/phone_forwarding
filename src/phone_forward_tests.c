/* Ten plik włączamy jako pierwszy i dwa razy, aby sprawdzić, czy zawiera
 * wszystko, co jest potrzebne. */
#include "phone_forward.h"
#include "phone_forward.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Możliwe wyniki testu */
#define PASS 0
#define FAIL 1
#define WRONG_TEST 2

/* Liczba elementów tablicy x */
#define SIZE(x) (sizeof(x) / sizeof(x)[0])

/* Początek testu */
#define INIT(p)                        \
  struct PhoneForward *p = phfwdNew(); \
  if (p == NULL)                       \
    return FAIL

/* Koniec testu */
#define CLEAN(p)  \
  phfwdDelete(p); \
  return PASS;

/* Wypełnienie numeru znakiem */
#define FILL(p, from, to, c)           \
  do {                                 \
    for (int _k = from; _k < to; ++_k) \
      p[_k] = c;                       \
    p[to] = '\0';                      \
  } while (0)

/* Skopiowanie numeru */
#define COPY(dst, src) \
  strcpy(dst, src)

/* Oczekiwane jednakowe numery */
#define C(x, y)            \
  do {                     \
    if (strcmp(x, y) != 0) \
      return FAIL;         \
  } while (0)

/* Oczekiwany zerowy wynik funkcji */
#define Z(f)       \
  do {             \
    if ((f) != 0)  \
      return FAIL; \
  } while (0)

/* Oczekiwany niezerowy wynik funkcji */
#define N(f)       \
  do {             \
    if ((f) == 0)  \
      return FAIL; \
  } while (0)

/* Oczekiwany numer n jako i-ty w strukturze PhoneNumbers */
#define R(p, i, n) \
  C(phnumGet(p, i), n)

/* Oczekiwany brak i-tego numeru w strukturze PhoneNumbers */
#define Q(p, i) \
  Z(phnumGet(p, i))

/* Oczekiwana pusta struktura PhoneNumbers */
#define E(f)                      \
  do {                            \
   struct PhoneNumbers const *_p; \
   N(_p = f);                     \
   Q(_p, 0);                      \
   Q(_p, 1);                      \
   Q(_p, 9999);                   \
   phnumDelete(_p);               \
  } while (0)

/* Oczekiwane przekierowanie z A na B */
#define CHECK(p, A, B)             \
  do {                             \
    struct PhoneNumbers const *_p; \
    N(_p = phfwdGet(p, A));        \
    R(_p, 0, B);                   \
    Q(_p, 1);                      \
    phnumDelete(_p);               \
  } while (0)

/* Oczekiwane odwrotne przekierowania z A */
#define RCHCK(p, A, ...)                     \
  do {                                       \
    struct PhoneNumbers const *_p;           \
    N(_p = phfwdReverse(p, A));              \
    char const *_r[] = {__VA_ARGS__};        \
    for (size_t _k = 0; _k < SIZE(_r); ++_k) \
      R(_p, _k, _r[_k]);                     \
    Q(_p, SIZE(_r));                         \
    phnumDelete(_p);                         \
  } while (0)

/** TESTY **/

/* Tylko utworzenie i usunięcie struktury */
static int empty_struct(void) {
  INIT(pf);
  CLEAN(pf);
}

/* Brak przekierowań */
static int no_forward(void) {
  char n[2];

  INIT(pf);

  n[1] = '\0';
  for (n[0] = '9'; n[0] >= '0'; --n[0])
    phfwdRemove(pf, n);
  for (n[0] = '0'; n[0] <= '9'; ++n[0])
    CHECK(pf, n, n);
  for (n[0] = '9'; n[0] >= '0'; --n[0])
    RCHCK(pf, n, n);

  CLEAN(pf);
}

/* Błędne argumenty funkcji */
static int wrong_arguments(void) {
  INIT(pf);

  Z(phfwdAdd(pf, "123", ""));
  Z(phfwdAdd(pf, "123", "a"));
  Z(phfwdAdd(pf, "123", " "));
  Z(phfwdAdd(pf, "123", "9b"));
  Z(phfwdAdd(pf, "123", "@"));
  Z(phfwdAdd(pf, "123", "/"));
  Z(phfwdAdd(pf, "123", ":"));
  Z(phfwdAdd(pf, "123", ";"));
  Z(phfwdAdd(pf, "123", ">"));
  Z(phfwdAdd(pf, "123", "0?"));
  Z(phfwdAdd(pf, "", "123"));
  Z(phfwdAdd(pf, "c", "123"));
  Z(phfwdAdd(pf, " ", "123"));
  Z(phfwdAdd(pf, "8d", "123"));
  Z(phfwdAdd(pf, "/", "123"));
  Z(phfwdAdd(pf, ":", "123"));
  Z(phfwdAdd(pf, ";", "123"));
  Z(phfwdAdd(pf, "1>", "123"));
  Z(phfwdAdd(pf, "?", "123"));
  Z(phfwdAdd(pf, "@", "123"));
  Z(phfwdAdd(pf, "0", "0"));

  E(phfwdGet(pf, ""));
  E(phfwdGet(pf, "e"));
  E(phfwdGet(pf, " "));
  E(phfwdGet(pf, "7f"));
  E(phfwdGet(pf, "%"));
  E(phfwdGet(pf, "/"));
  E(phfwdGet(pf, "%"));
  E(phfwdGet(pf, ":"));
  E(phfwdGet(pf, ";"));
  E(phfwdGet(pf, "?"));
  E(phfwdGet(pf, ">"));
  E(phfwdGet(pf, "@"));

  E(phfwdReverse(pf, ""));
  E(phfwdReverse(pf, "e"));
  E(phfwdReverse(pf, " "));
  E(phfwdReverse(pf, "7f"));
  E(phfwdReverse(pf, "/"));
  E(phfwdReverse(pf, "%"));
  E(phfwdReverse(pf, ":"));
  E(phfwdReverse(pf, ";"));
  E(phfwdReverse(pf, ">"));
  E(phfwdReverse(pf, "@"));

  phfwdRemove(pf, "");
  phfwdRemove(pf, "g");
  phfwdRemove(pf, " ");
  phfwdRemove(pf, "6h");
  phfwdRemove(pf, "/");
  phfwdRemove(pf, "%");
  phfwdRemove(pf, ":");
  phfwdRemove(pf, ";");
  phfwdRemove(pf, ">");
  phfwdRemove(pf, "@");

  CLEAN(pf);
}

/* Niepsucie bazy przez błędne wywołania funkcji  */
static int breaking_struct(void) {
  char a[2], b[2];

  INIT(pf);

  a[1] = b[1] = '\0';
  for (a[0] = '0', b[0] = '9'; a[0] <= '9'; ++a[0], --b[0])
    N(phfwdAdd(pf, a, b));

  Z(phfwdAdd(pf, "1", "A"));
  Z(phfwdAdd(pf, "2D", "0"));
  Z(phfwdAdd(pf, "B", "2"));
  phfwdRemove(pf, "C");

  for (a[0] = '0', b[0] = '9'; a[0] <= '9'; ++a[0], --b[0]) {
    CHECK(pf, a, b);
    if (a[0] < b[0])
      RCHCK(pf, b, a, b);
    else
      RCHCK(pf, b, b, a);
  }

  CLEAN(pf);
}

#define LEN1 1000
#define LEN2 4500
#define LEN3 1500
#define LEN4 5000

/* Długie numery */
static int long_numbers(void) {
  char n1[LEN1 + 1], n2[LEN2 + 1], n3[LEN3 + 1], n4[LEN4 + 1];

  FILL(n1, 0, LEN1, '1');
  FILL(n2, 0, LEN1, '1');
  FILL(n2, LEN1, LEN2, '2');
  FILL(n3, 0, LEN3, '3');
  FILL(n4, 0, LEN3, '3');
  FILL(n4, LEN3, LEN4, '2');

  INIT(pf);

  N(phfwdAdd(pf, n1, n3));
  CHECK(pf, n2, n4);
  RCHCK(pf, n4, n2, n4);

  CLEAN(pf);
}

#define LEN9 20
#define COUNT9 9

/* Dużo numerów w strukturze PhoneNumbers */
static int many_numbers(void) {
  char n9[COUNT9 * COUNT9][LEN9 + 1];
  struct PhoneNumbers const *pn;

  for (int i = 0; i < COUNT9; ++i) {
    for (int j = 0; j < COUNT9; ++j) {
      FILL(n9[COUNT9 * i + j], 0, LEN9 / 2, '0' + i);
      FILL(n9[COUNT9 * i + j], LEN9 / 2, LEN9, '0' + j);
    }
  }

  INIT(pf);

  for (int i = 0; i < COUNT9 * COUNT9; ++i)
    N(phfwdAdd(pf, n9[i], "9"));
  N(pn = phfwdReverse(pf, "9"));
  for (int i = 0; i < COUNT9 * COUNT9; ++i)
    R(pn, i, n9[i]);
  R(pn, COUNT9 * COUNT9, "9");
  Q(pn, COUNT9 * COUNT9 + 1);
  phnumDelete(pn);

  CLEAN(pf);
}

#define LEN5 5

/* Kopiowanie wartości do wnętrza struktury */
static int copy_arguments(void) {
  static const char p1[] = "123";
  static const char p2[] = "456";
  char p3[LEN5 + 1], p4[LEN5 + 1];

  INIT(pf);

  COPY(p3, p1);
  COPY(p4, p2);

  N(phfwdAdd(pf, p3, p4));

  FILL(p3, 0, LEN5, '\0');
  FILL(p4, 0, LEN5, '\0');

  CHECK(pf, "12345", "45645");

  CLEAN(pf);
}

/* Dwie instancje struktury */
static int two_structs(void) {
  INIT(pf1);
  INIT(pf2);

  N(phfwdAdd(pf1, "1", "5"));
  N(phfwdAdd(pf2, "2", "6"));

  CHECK(pf1, "1", "5");
  CHECK(pf1, "2", "2");
  CHECK(pf2, "1", "1");
  CHECK(pf2, "2", "6");

  phfwdDelete(pf1);
  CLEAN(pf2);
}

/* Delete ze wskaźnikiem NULL */
static int delete_null(void) {
  phnumDelete(NULL);
  CLEAN(NULL);
}

/* Zachowanie wyniku po zmianie struktury */
static int persistent_results(void) {
  struct PhoneNumbers const *png;
  struct PhoneNumbers const *pnr;

  INIT(pf);

  N(phfwdAdd(pf, "3", "4"));

  N(png = phfwdGet(pf, "33"));
  N(pnr = phfwdReverse(pf, "43"));

  N(phfwdAdd(pf, "3", "5"));
  phfwdRemove(pf, "3");
  phfwdDelete(pf);

  R(png, 0, "43");
  Q(png, 1);
  phnumDelete(png);

  R(pnr, 0, "33");
  R(pnr, 1, "43");
  Q(pnr, 2);
  phnumDelete(pnr);

  return PASS;
}

/* Nadpisanie przekierowania */
static int forward_overwrite(void) {
  INIT(pf);

  CHECK(pf, "000", "000");

  N(phfwdAdd(pf, "0", "5"));
  CHECK(pf, "000", "500");

  N(phfwdAdd(pf, "0", "7"));
  CHECK(pf, "000", "700");

  CLEAN(pf);
}

/* Usunięcie przekierowania */
static int remove_forward(void) {
  INIT(pf);

  N(phfwdAdd(pf, "0", "5"));
  CHECK(pf, "000", "500");

  phfwdRemove(pf, "0");
  CHECK(pf, "000", "000");

  CLEAN(pf);
}

/* Proste przekierowanie odwrotne */
static int simple_reverse(void) {
  INIT(pf);

  N(phfwdAdd(pf, "2", "4"));
  N(phfwdAdd(pf, "23", "4"));
  RCHCK(pf, "434", "2334", "234", "434");

  CLEAN(pf);
}

/* Różne operacje na strukturze */
static int various_ops(void) {
  INIT(pf);

  N(phfwdAdd(pf, "123", "9"));
  CHECK(pf, "1234", "94");
  N(phfwdAdd(pf, "123456", "777777"));
  CHECK(pf, "12345", "945");
  CHECK(pf, "123456", "777777");
  CHECK(pf, "997", "997");
  RCHCK(pf, "987654321", "12387654321", "987654321");
  phfwdRemove(pf, "12");
  CHECK(pf, "123456", "123456");
  RCHCK(pf, "987654321", "987654321");
  N(phfwdAdd(pf, "567", "0"));
  N(phfwdAdd(pf, "5678", "08"));
  RCHCK(pf, "08", "08", "5678");
  N(phfwdAdd(pf, "12", "123"));
  CHECK(pf, "123", "1233");
  N(phfwdAdd(pf, "2", "4"));
  N(phfwdAdd(pf, "23", "4"));
  RCHCK(pf, "434", "2334", "234", "434");

  phfwdDelete(pf);
  N(pf = phfwdNew());

  N(phfwdAdd(pf, "123", "76"));
  CHECK(pf, "1234581", "764581");
  CHECK(pf, "7581", "7581");
  RCHCK(pf, "7581", "7581");

  CLEAN(pf);
}

#define xfrom   00000
#define xto     99999
#define div0    1
#define format0 "%05u"
#define div1    10
#define format1 "%04u"
#define div2    100
#define format2 "%03u"
#define div3    1000
#define format3 "%02u"

/* Dużo operacji na strukturze */
static int many_ops() {
  char b1[16], b2[16];

  INIT(pf);

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0, i);
    sprintf(b2, format2, i / div2);
    N(phfwdAdd(pf, b1, b2));
  }

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0 "123", i);
    sprintf(b2, format2 "123", i / div2);
    CHECK(pf, b1, b2);
  }

  for (unsigned i = xfrom / div1; i <= xto / div1; ++i) {
    sprintf(b1, format1, i);
    CHECK(pf, b1, b1);
  }

  for (unsigned i = xfrom / div3; i <= xto / div3; ++i) {
    sprintf(b1, format3, i);
    RCHCK(pf, b1, b1);
  }

  for (unsigned i = xfrom / div2; i <= xto / div2; ++i) {
    sprintf(b1, format2, i);
    phfwdRemove(pf, b1);
  }

  for (unsigned i = xfrom / div0; i <= xto / div0; ++i) {
    sprintf(b1, format0, i);
    CHECK(pf, b1, b1);
    RCHCK(pf, b1, b1);
  }

  CLEAN(pf);
}

#define LONG_LEN 250000

/* Wydajność implementacji dla bardzo długich numerów */
static int very_long(void) {
  char *base, b[3];

  INIT(pf);
  N(base = malloc(sizeof(char) * (LONG_LEN + 3)));

  for (int i = 0; i < LONG_LEN; ++i)
    base[i] = '0' + i % 10;
  base[LONG_LEN + 2] = '\0';
  b[2] = '\0';
  for (int i = 0; i <= 99; ++i) {
    b[0] = '0' + i / 10;
    b[1] = '0' + i % 10;
    base[LONG_LEN]     = '0' + i % 10;
    base[LONG_LEN + 1] = '0' + i / 10;
    N(phfwdAdd(pf, base, b));
  }
  for (int i = 0 ; i <= 99; ++i) {
    b[0] = '0' + i / 10;
    b[1] = '0' + i % 10;
    base[LONG_LEN]     = '0' + i % 10;
    base[LONG_LEN + 1] = '0' + i / 10;
    CHECK(pf, base, b);
    if (i < 2)
      RCHCK(pf, b, b, base);
    else
      RCHCK(pf, b, base, b);
  }

  free(base);
  CLEAN(pf);
}

#define BIG_COUNT 10000

/* Wydajność implementacji przy kasowaniu dużej liczby przekierowań */
static int many_remove(void) {
  char *base;

  INIT(pf);
  N(base = malloc(sizeof(char) * (BIG_COUNT + 1)));

  for (int i = 0; i < BIG_COUNT; ++i)
    base[i] = '0' + i % 10;
  for (int i = BIG_COUNT; i > 1; --i) {
    base[i] = '\0';
    N(phfwdAdd(pf, base, "0"));
  }
  N(phfwdAdd(pf, "123456789", "0"));
  phfwdRemove(pf, "0");
  RCHCK(pf, "0123456789", "0123456789", "123456789123456789");

  free(base);
  CLEAN(pf);
}

#define yfrom   0000
#define yto     9999
#define yformat "%04u"
#define REPEAT_COUNT 27

/* Wydajność implementacji dla dodawania i usuwania przekierowań */
static int add_remove(void) {
  char b1[8], b2[16];

  INIT(pf);

  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    N(phfwdAdd(pf, b1, b2));
  }
  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    for (unsigned j = 0; j < REPEAT_COUNT; ++j) {
      phfwdRemove(pf, b1);
      N(phfwdAdd(pf, b1, b2));
    }
  }
  for (unsigned i = yfrom; i <= yto; ++i) {
    sprintf(b1, yformat, i);
    sprintf(b2, yformat yformat, i, i);
    CHECK(pf, b1, b2);
    RCHCK(pf, b2, b1, b2);
  }

  CLEAN(pf);
}

/** URUCHAMIANIE TESTÓW **/

typedef struct {
  char const *name;
  int (*function)(void);
} test_list_t;

#define TEST(t) {#t, t}

static const test_list_t test_list[] = {
  TEST(empty_struct),
  TEST(no_forward),
  TEST(wrong_arguments),
  TEST(breaking_struct),
  TEST(long_numbers),
  TEST(many_numbers),
  TEST(copy_arguments),
  TEST(two_structs),
  TEST(delete_null),
  TEST(persistent_results),
  TEST(forward_overwrite),
  TEST(remove_forward),
  TEST(simple_reverse),
  TEST(various_ops),
  TEST(many_ops),
  TEST(very_long),
  TEST(many_remove),
  TEST(add_remove),
};

int main(int argc, char *argv[]) {
  if (argc != 2)
    return WRONG_TEST;

  for (size_t i = 0; i < SIZE(test_list); ++i)
    if (strcmp(argv[1], test_list[i].name) == 0)
      return test_list[i].function();

  return WRONG_TEST;
}
