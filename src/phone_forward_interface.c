/**@file
 * Terminalowy interfejs użytkownika programu phone_forward.
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <memory.h>
#include "phone_forward.h"
#include "arrayList.h"
//#define DEBUG

/**
 * Stała wskazująca na błąd typu EOF
 */
const size_t EOF_MARK = 991232114;

/**
 * Stała wskazująca na uruchomienie programu w celu weryfikacji wejścia dla celów skryptu.
 */
const size_t VERIFICATION_PROGRAM = 143214123;


bool isdigit12(int x){
  return '0' <= x && x <= '0' + 12 - 1;
}

bool isalnum12(int x){
  return isalpha(x) || isdigit12(x);
}

/**
 * Struktura trzymająca modyfikowalnego i przedłużalnego stringa.
 * Zwalnia się ją poprzez instrukcję free(this->text), free(this).
 */
typedef struct VarLenText{
    /** Właściwy ciąg charów, zawiera @p textCapacity elementów*/
    char * text;
    /** Długość tekstu */
    size_t textSize;
    /** Obecna pojemność tekstu */
    size_t textCapacity;
} VarLenText;

/**
 * Tworzy modyfikowalny string zawierający jeden element - @p starting
 * @param starting Znak do appendowania
 * @return String zawierający jeden znak, lub NULL jeżeli alokacja się nie powiodła
 */
VarLenText * newVarLenText(char starting){
  VarLenText * vlt = malloc(sizeof(VarLenText));
  if (vlt == NULL){
    return NULL;
  }
  vlt->textSize = 1;
  vlt->textCapacity = 100;
  vlt->text = calloc(vlt->textCapacity, sizeof(char));
  if (vlt->text == NULL){
    free(vlt);
    return NULL;
  }
  vlt->text[0] = starting;
  return vlt;
}

/**
 * Sprawdza, czy @p vlt jest pusty.
 * @param vlt string do sprawdzenia
 * @return true, jeżeli @p vlt jest pusty, false wpp.
 */
bool isVarLenTextEmpty(VarLenText * vlt){
  return vlt->textSize == 0;
}

/**
 * Dodaje znak @p toAdd na koniec stringa @p vlt. Jeżeli nastąpił błąd alokacji, czyści vlt do pustego.
 * @param vlt string do zmodyfikowania
 * @param toAdd znak do wpisania na koniec vlt.
 */
void varLenTextAdd(VarLenText * vlt, char toAdd) {
  vlt->text[vlt->textSize] = toAdd;
  vlt->textSize++;
  if (vlt->textSize == vlt->textCapacity){
    vlt->textCapacity *= 2;
    vlt->text = realloc(vlt->text, vlt->textCapacity * sizeof(char));
    if (vlt->text == NULL){
      vlt->textCapacity = vlt->textSize = 0;
    }
    for (size_t i = vlt->textCapacity / 2; i < vlt->textCapacity; i++){
      vlt->text[i] = 0;
    }
  }
  vlt->text[vlt->textSize] = 0;
}

/**
 * Wczytuje tekst komentarza, zaczynąjąc od @p oldChar, kończy w momencie, gdy pojawiają się dwa znaki '$'.
 * Wczytuje również pierwszy znak za komentarzem i ustawia go pod @p oldChar
 * @param oldChar wskaźnik na ostatni wczytany znak. Przy udanym zakończeniu programu wskazuje na spację (' ').
 * @param charIndex wskaźnik do liczby wczytanych znaków.
 * @return status wczytywania: 0 jeżeli udało się wczytać koniec komentarza, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readComment(char * oldChar, size_t * charIndex){
  char last;
  *oldChar = (char) getchar();
  (*charIndex)++;
  if (*oldChar != '$'){
    return *charIndex - 1;
  }
  last = (char) getchar();
  (*charIndex)++;
  if (last == EOF) {
    return EOF_MARK;
  }
  *oldChar = (char) getchar();
  (*charIndex)++;
  if (*oldChar == EOF){
    return EOF_MARK;
  }
  //printf("Comment: %c", *oldChar);
  while(last != '$' || *oldChar != '$') {
    last = *oldChar;
    *oldChar = (char) getchar();
    //printf("%c", *oldChar);
    (*charIndex)++;
    if (*oldChar == EOF) {
      return EOF_MARK;
    }
  }
  *oldChar = ' ';
  return 0;
}

/**
 * Wczytuje białe znaki.
 * @param oldChar wskaźnik na ostatni wczytany znak.
 * @param charIndex wskaźnik do liczby wczytanych znaków.
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readWhitespace(char * oldChar, size_t * charIndex){
  if (*oldChar == '$'){
    return readComment(oldChar, charIndex);
  }
  for (size_t i = 1; true; i++){
    char a = (char)getchar();
    (*charIndex)++;
    while (a == '$'){
      size_t status = readComment(&a, charIndex);
      if (status != 0){
        return status;
      }
    }
    if (a == EOF){
      return EOF_MARK;
    }
    if (!isspace(a)){
      *oldChar = a;
      return 0;
    }
  }
}


/**
 * Wczytuje liczbę do *vlt0, zaczynąjąc od @p oldChar, kończy w momencie, gdy pojawi się inny znak niż cyfra.
 * Wczytuje również pierwszy znak za ostatnią cyfrą i ustawia go jako @p oldChar
 * @param vlt0 - miejsce do zapisu wczytanej liczby.
 * @param oldChar wskaźnik na ostatni wczytany znak.
 * @param charIndex wskaźnik do liczby wczytanych znaków.
 * @return status wczytywania: 0 jeżeli udało się wczytać liczbę, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readNumber(VarLenText ** vlt0, char * oldChar, size_t * charIndex){
  while (*oldChar == '$' || isspace(*oldChar)){
    size_t status = readWhitespace(oldChar, charIndex);
    if (status != 0){
      return status;
    }
  }
  if (!isdigit12(*oldChar)){
    return *charIndex;
  }
  VarLenText * vlt = newVarLenText(*oldChar);
  *vlt0 = vlt;
  for (size_t i = 1; true; i++){
    char a = (char)getchar();
    (*charIndex)++;

    if (!isdigit12(a)){
      *oldChar = a;
      return 0;
    }
    varLenTextAdd(vlt, a);
  }
}

/**
 * Wczytuje identyfikator (zaczynający się literą, składający się ze znaków alfanumerycznych).
 * @param vlt0 wskaźnik do pola z zapisem danej liczby
 * @param oldChar wskaźnik na ostatni wczytany znak
 * @param charIndex wskaźnik do liczby wczytanych znaków.
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readIdentifier(VarLenText ** vlt0, char * oldChar, size_t * charIndex){
  if (!isalpha(*oldChar)){
    return *charIndex;
  }
  VarLenText * vlt = newVarLenText(*oldChar);
  *vlt0 = vlt;
  for (size_t i = 1; true; i++){
    char a = (char)getchar();
    (*charIndex)++;
    if (a == '$' || !isalnum12(a)){
      *oldChar = a;
      return 0;
    }
    varLenTextAdd(vlt, a);
  }
}




/**
 * Para <nazwa, baza przekierowań>.
 * Zwalnianie wraz z usunięciem wskaźników za pomocą deleteMatch.
 */
struct NameForwardMatch{
    /** Nazwa bazy przekierowań. */
    char * name;
    /** Baza przekierowań */
    struct PhoneForward * forward;
};

/**
 * Konstruktor do pary NameForwardMatch (<nazwa, baza przekierowań>).
 * @param name nazwa bazy przekierowań
 * @param forward wskaźnik na bazę przekierowań
 * @return Para elementów <@p name, @p forward>
 */
struct NameForwardMatch * newNameForwardMatch(char * name, struct PhoneForward * forward){
  struct NameForwardMatch * nfm = malloc(sizeof(struct NameForwardMatch));
  nfm->name = name;
  nfm->forward = forward;
  return nfm;
}

/**
 * Usuwa parę NameForwardMatch
 * @param match para do usunięcia
 */
void deleteMatch(struct NameForwardMatch * match){
  free(match->name);
  phfwdDelete(match->forward);
  free(match);
}

/**
 * Struktura przechowująca informacje o stanie programu.
 */
typedef struct ForwardInformation{
    /** Zbiór baz przekierowań */
    ArrayList * forwardList;
    /** Obecnie aktywne przekierowanie (NULL jeżeli nie ma takiego) */
    struct PhoneForward * currentForward;
    /** Liczba wczytanych bajtów (charów) przez program */
    size_t charIndex;
    /** Status programu - domyślnie zero */
    size_t programStatus;
} ForwardInformation;


/**
 * Ustawia bazę o nazwie @p identifier do użytku. Jeżeli taka baza nie istnieje, tworzy pustą i ustawia ją do użytku.
 * @param information dane o stanie programu
 * @param identifier nazwa bazy
 */
void setForward(ForwardInformation * information, char * identifier){
  for (size_t i = 0; i < information->forwardList->phNumCount; i++) {
    struct NameForwardMatch const * rightnow = arrayListGet(information->forwardList, i);
    if (strcmp(rightnow->name, identifier) == 0){
      information->currentForward = rightnow->forward;
      return;
    }
  }
  char * id2 = calloc((strlen(identifier) + 1), sizeof(char));
  memcpy(id2, identifier, strlen(identifier) * sizeof(char));


  struct NameForwardMatch * newMatch = newNameForwardMatch(id2, phfwdNew());
  arrayListAdd(information->forwardList, newArrayListNodeVoid(newMatch));
  information->currentForward = newMatch->forward;
}

/**
 * Usuwa bazę o nazwie @p identifier do użytku. Jeżeli baza była w bezpośrednim użyciu, ustawia obecną bazę na NULL.
 * @param information dane o bazie.
 * @param identifier nazwa bazy do usunięcia
 * @return Status - 0 jeżeli się udało, 1 jeżeli baza o nazwie identifier nie istnieje.
 */
size_t deleteForward(ForwardInformation * information, char * identifier){

  for (size_t i = 0; i < information->forwardList->phNumCount; i++){
    ArrayListNode * rightnow = information->forwardList->phNums[i];
    struct NameForwardMatch * val = rightnow->value;
    if (strcmp(val->name, identifier) == 0){
      if (val->forward == information->currentForward){
        information->currentForward = NULL;
      }
      arrayListCutOut(information->forwardList, rightnow);
      deleteMatch(val);
      return 0;
    }
  }
  return 1;
}

/**
 * Wczytuje instrukcję zaczynającą się literą: są to instrukcje NEW i DEL.
 * @param information informacja o stanie programu
 * @param oldChar wskaźnik na ostatni wczytany znak
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readNEWDELInstruction(ForwardInformation *information, char *oldChar){
  if (information->programStatus == VERIFICATION_PROGRAM){
    return EOF_MARK;
  }
  VarLenText * vlt = NULL;
  size_t instructionIndex = information->charIndex;
  {
    size_t status = readIdentifier(&vlt, oldChar, &information->charIndex);
    if (status != 0) {
      free(vlt);
      return status;
    }
  }
  if (vlt->textSize < 3){
    free(vlt->text);
    free(vlt);
    return instructionIndex;
  }
  if (vlt->textSize == 3 && ((vlt->text[0] == 'N' && vlt->text[1] == 'E' && vlt->text[2] == 'W') ||
      (vlt->text[0] == 'D' && vlt->text[1] == 'E' && vlt->text[2] == 'L'))){
    char * identifier;

    char let0 = *oldChar;
    if (*oldChar == EOF){
      free(vlt->text);
      free(vlt);
      return EOF_MARK;
    }
    VarLenText * vlt2 = NULL;

    while (*oldChar == '$' || isspace(*oldChar)){
      size_t status = readWhitespace(oldChar, &information->charIndex);
      if (status != 0){
        free(vlt->text);
        free(vlt);
        return status;
      }
    }
    let0 = *oldChar;
    size_t status = 0;
    if (isdigit12(let0) && vlt->text[0] == 'D'){
      status = readNumber(&vlt2, oldChar, &information->charIndex);
    }
    else if (isalpha(let0)){
      status = readIdentifier(&vlt2, oldChar, &information->charIndex);
    }
    else{
      status = information->charIndex;
    }
    if (status != 0){
      free(vlt->text);
      free(vlt);
      return status;
    }
    identifier = vlt2->text;
    free(vlt2);
    if (strcmp("NEW", identifier) == 0 || strcmp("DEL", identifier) == 0){
      free(identifier);
      free(vlt->text);
      free(vlt);
      return information->charIndex - 3;
    }

    if (vlt->text[0] == 'N'){
#ifdef DEBUG
      printf("Opening %s\n", identifier);
#endif
      setForward(information, identifier);
      if (information->currentForward == NULL){
        fprintf(stderr, "ERROR NEW %zu\n", instructionIndex);
        free(identifier);
        free(vlt->text);
        free(vlt);
        return EOF_MARK + 1;
      }
    }
    else if (isdigit12(let0)){
#ifdef DEBUG
      printf("Removing redirections of %s\n", identifier);
#endif
      if (information->currentForward == NULL){
        free(identifier);
        free(vlt->text);
        free(vlt);
        fprintf(stderr, "ERROR DEL %zu\n", instructionIndex);
        return EOF_MARK + 1;
      }
      else {
        phfwdRemove(information->currentForward, identifier);
      }
    }
    else {
#ifdef DEBUG
      printf("Erasing %s\n", identifier);
#endif
      size_t status2 = deleteForward(information, identifier);
      if (status2 == 1){
        fprintf(stderr, "ERROR DEL %zu\n", instructionIndex);
        free(identifier);
        free(vlt->text);
        free(vlt);
        return EOF_MARK + 1;
      }

    }
    free(identifier);
    free(vlt->text);
    free(vlt);
  }
  else{
    free(vlt->text);
    free(vlt);
    return instructionIndex;
    //identifier = calloc(strlen(vlt->text + 2) + 3, sizeof(char));
    //memcpy(identifier, vlt->text + 2, sizeof(char) * strlen(vlt->text + 2));
  }
  return 0;
}

/**
 * Wczytuje instrukcję zaczynającą się cyfrą.
 * @param information informacja o stanie programu
 * @param oldChar wskaźnik na ostatni wczytany znak
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readNumberInstruction(ForwardInformation * information, char * oldChar){
  VarLenText * num1 = NULL;
  {
    size_t status = readNumber(&num1, oldChar, &information->charIndex);
    if (status != 0) {
      free(num1);
      return status;
    }
  }
  while ((*oldChar == '$') || isspace(*oldChar)){
    size_t status = readWhitespace(oldChar, &information->charIndex);
    if (status != 0){
      free(num1->text);
      free(num1);
      return status;
    }
  }
  size_t operatorIndex = information->charIndex;
  if (*oldChar == '>') {
    *oldChar = (char) getchar();
    information->charIndex++;
    if (isspace(*oldChar)) {
      size_t status = readWhitespace(oldChar, &information->charIndex);
      if (status != 0) {
        free(num1->text);
        free(num1);
        return status;
      }
    }
    VarLenText *num2 = NULL;
    {
      size_t status = readNumber(&num2, oldChar, &information->charIndex);
      if (status != 0) {
        free(num1->text);
        free(num1);
        free(num2);
        return status;
      }
    }
#ifdef DEBUG
    printf("Add forward %s to %s (%zu)\n", num1->text, num2->text, operatorIndex);
#endif
    bool ok = true;
    if (information->currentForward == NULL){
      ok = false;
    }
    if (ok) {
      ok = phfwdAdd(information->currentForward, num1->text, num2->text);
    }
    if (!ok){
      fprintf(stderr, "ERROR > %zu\n", operatorIndex);
      free(num2->text);
      free(num2);
      free(num1->text);
      free(num1);
      return EOF_MARK + 1;
    }
    free(num2->text);
    free(num2);
  }
  else if (*oldChar == '?') {
    if (information->programStatus == VERIFICATION_PROGRAM){
      free(num1->text);
      free(num1);
      return EOF_MARK;
    }
#ifdef DEBUG
    printf("Querying redirections from %s (%zu)\n", num1->text, operatorIndex);
#endif
    if (information->currentForward == NULL){
      fprintf(stderr, "ERROR ? %zu\n", operatorIndex);

      free(num1->text);
      free(num1);
      return EOF_MARK + 1;
    }
    else {
      PhoneNumbers const *numbers = phfwdGet(information->currentForward, num1->text);
      for (size_t i = 0; phnumGet(numbers, i) != NULL; i++) {
        printf("%s\n", phnumGet(numbers, i));
      }
      //printf("\n");

      phnumDelete(numbers);
    }
    *oldChar = (char)getchar();
    information->charIndex++;
  }
  else{
    free(num1->text);
    free(num1);
    return information->charIndex;
  }
  free(num1->text);
  free(num1);
  return 0;
}

/**
 * Wczytuje instrukcję zaczynającą się znakiem zapytania.
 * @param information informacja o stanie programu
 * @param oldChar wskaźnik na ostatni wczytany znak
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF..
 */
size_t readQuestionAtMarkInstruction(ForwardInformation *information, char *oldChar){
  if (information->programStatus == VERIFICATION_PROGRAM){
    return EOF_MARK;
  }
  char sign0 = *oldChar;
  size_t operatorIndex = information->charIndex;
  *oldChar = (char)getchar();
  information->charIndex++;
  if (isspace(*oldChar)){
    size_t status = readWhitespace(oldChar, &information->charIndex);
    //printf("STATUS %zu\n", status);
    if (status != 0){
      return status;
    }
  }
  VarLenText * vlt = NULL;
  {
    size_t status = readNumber(&vlt, oldChar, &information->charIndex);
    if (status != 0){
      free(vlt);
      return status;
    }
  }
#ifdef DEBUG
  printf("Querying reverses to %s (%zu)\n", vlt->text, operatorIndex);
#endif
  if (information->currentForward == NULL){
    fprintf(stderr, "ERROR %c %zu\n", sign0, operatorIndex);
    free(vlt->text);
    free(vlt);
    return EOF_MARK + 1;
  }
  else{
    if (sign0 == '?'){
      PhoneNumbers const * numbers = phfwdReverse(information->currentForward, vlt->text);
      for(size_t i = 0; phnumGet(numbers, i) != NULL; i++){
        printf("%s\n", phnumGet(numbers, i));
      }
      //printf("\n");

      phnumDelete(numbers);
    }
    else{
      size_t len = 12 < vlt->textSize ? vlt->textSize - 12 : 0;
      size_t answer = phfwdNonTrivialCount(information->currentForward, vlt->text, len);
      printf("%zu\n", answer);
    }
  }
  free(vlt->text);
  free(vlt);
  return 0;
}


/**
 * Wczytuje instrukcję.
 * @param information informacja o stanie programu
 * @param oldChar wskaźnik na ostatni wczytany znak
 * @return status wczytywania: 0 jeżeli udało się wczytać znaki, numer znaku przy błędzie, EOFmark przy EOF.
 */
size_t readInstruction(ForwardInformation * information, char* oldChar){
  if (*oldChar == '\0' || *oldChar == EOF){
    return EOF_MARK + 1;
  }
  if (isspace(*oldChar)){
    size_t status = readWhitespace(oldChar, &information->charIndex);
    if (status != 0){
      if (status == EOF_MARK){
        return EOF_MARK + 1;
      }
      return status;
    }
  }
  if (isalpha(*oldChar)){
    return readNEWDELInstruction(information, oldChar);
  }
  else if (isdigit12(*oldChar)){
    return readNumberInstruction(information, oldChar);
  }
  else if ((*oldChar) == '?' || (*oldChar) == '@'){
    return readQuestionAtMarkInstruction(information, oldChar);
  }
  else if ((*oldChar) == '$'){
    return readComment(oldChar, &information->charIndex);
  }
  else{
    return information->charIndex;
  }
}



/**
 * Funkcja uruchamiająca program.
 * @return Zero.
 */
int main(int argc, char** argv){
  ForwardInformation information;
  information.forwardList = newArrayList();
  information.currentForward = NULL;
  information.charIndex = 1;
  information.programStatus = 0;


  if (argc >= 2 && strcmp("-verskr", argv[1]) == 0){
    setForward(&information, "XD");
    information.programStatus = VERIFICATION_PROGRAM;
  }

  char a = (char)getchar();
  while(true){
    size_t status = readInstruction(&information, &a);
#ifdef DEBUG
    printf("%zu\n", status);
#endif
    if (status != 0){
      if (status == EOF_MARK){
        fprintf(stderr, "ERROR EOF\n");
      }
      else if (status != EOF_MARK + 1){
        fprintf(stderr, "ERROR %zu\n", status);
      }

      for (size_t i = 0; i < information.forwardList->phNumCount; i++){
        ArrayListNode * node = information.forwardList->phNums[i];
        deleteMatch(node->value);
      }
      arrayListDelete(information.forwardList);
      return 0;
    }
  }

}
