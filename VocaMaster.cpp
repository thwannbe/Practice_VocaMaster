////////////////////////////////////////////////////////////////////////////////
///
/// @file VocaMaster.cpp
/// @brief VocaMaster Source File
/// @details Vocabulary Practice Program
/// @author Suwon Oh <suwon@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2014/12/23 Suwon Oh created @n
/// 2014/12/23 Suwon Oh adapted to Doxygen @n
/// 2015/01/12 Suwon Oh updated some functions @n
///
/// @section purpose_section Purpose
/// Application for self-study
///

#include <cstdlib>
#include "VocaMaster.h"

#define FILENAME "voca.dat"
#define VERSION  1.2

using namespace std;

// implemented needed functions

static inline bool isWhite(char c) {
  return (c == '\n' || c == '\t' || c == ' ');
}

static inline int Strlen(char* str) {
  if (str == NULL) {
    cout << "STRING LENGTH NULL ERROR" << endl;
    exit(1);
  }
  
  int i;
  for (i = 0; str[i] != '\0'; i++) {}
  
  return i;
}

static inline void Strcpy(char* dst, char* src) {
  if (dst == NULL || src == NULL) {
    cout << "STRING COPY NULL ERROR" << endl;
    exit(1);
  }

  for (int i = 0; i < Strlen(src); i++)
    dst[i] = src[i];
  
  dst[Strlen(src)] = '\0';
}

static inline bool Strequal(char* str1, char* str2) {
  if (str1 == NULL || str2 == NULL) {
    cout << "STRING NULL ERROR" << endl;
    exit(1);
  }

  if (Strlen(str1) != Strlen(str2))
    return false;

  for (int i = 0; i < Strlen(str1); i++) {
    if (str1[i] != str2[i])
      return false;
  }

  return true;
}
// main function

int main(void) {
  ifstream *iFile = new ifstream(FILENAME);
  VocaEngine *engine = new VocaEngine(iFile);  
  iFile->close();
  
  bool good = true;
  while (good) {
    engine->showMenu();
    good = engine->processMenu();
  }

  if (engine)
    delete (engine);

  return 0;
}

// Voca implementation

Voca::Voca()
{
  word = NULL;
  meaning = NULL;
}

Voca::Voca(char* w, char* m)
{
  word = new char[sizeof(char) * Strlen(w) + 1];
  Strcpy(word, w);

  meaning = new char[sizeof(char) * Strlen(m) + 1];
  Strcpy(meaning, m);
}

Voca::~Voca()
{
  if (word)
    delete(word);
  if (meaning)
    delete(meaning);
}

char* Voca::getWord() {
  return word;
}

char* Voca::getMean() {
  return meaning;
}

int Voca::getExp() {
  return exp;
}

int Voca::getLevel() {
  return level;
}

// VocaEngine implementation

void VocaEngine::printTitle(void) {
  cout << "#######################################################" << endl; // 50 sharps
  cout << "#                     VOCAMASTER" << endl;
  cout << "#                       v." << VERSION << endl;
  cout << "#                  made by Suwon Oh" << endl;
  cout << "#" << endl;  
}

void VocaEngine::printEnd(void) {
  cout << "#" << endl;
  cout << "#######################################################" << endl; // 50 sharps
}

void VocaEngine::manageList(int index) {
  int tag = 1; // new index
  ListNode <Voca*> *cur = list->getNode(index);
  bool hasPrev = (index != 0) ? true : false;
  bool hasNext = false;
 
  if (!cur) { // empty list
    cout << "#" << endl;
    cout << "#             EMPTY LIST" << endl;
    cout << "#" << endl;
    return;
  }

  cout << "#               [ LIST ]" << endl;
  while (cur != list->getHead() && tag <= 10) {
    Voca *curVoca = cur->getContent();
    cout << "#    [" << tag++ << "] ";
    cout << curVoca->getWord() << " - " << curVoca->getMean() << endl;
    cur = cur->getNext();
  }
  
  if (cur != list->getHead() && tag == 11)
    hasNext = true;

  cout << "#" << endl;
  cout << "#             [ LIST MENU ]" << endl;
  cout << "#    (1) DELETE" << endl;
  cout << "#    (2) INITIALIZATION" << endl;
  cout << "#    (3) CANCEL" << endl;
  if (hasPrev && hasNext) {
    cout << "#    (4) <=" << endl;
    cout << "#    (5) =>" << endl;
    cout << "#" << endl;
  } else if (hasPrev) {
    cout << "#    (4) <=" << endl;
  } else if (hasNext) {
    cout << "#    (4) =>" << endl;
  }
  cout << "#" << endl;

  cout << "#    SELECT : ";
  char input[100];
  cin >> input;

  if (input[0] < '1' || input[0] > '3') {
    if (!hasPrev && !hasNext) {
      cout << "#    ERROR : WRONG INPUT" << endl;
      cout << "#" << endl;
      return;
    }
    else if (input[0] == '5' && (!hasPrev || !hasNext)) {
      cout << "#    ERROR : WRONG INPUT" << endl;
      cout << "#" << endl;
      return;
    }
  }

  switch(input[0]) {
    case '1':
      int choice;
      cout << "#    SELECT INDEX [NUMBER] : ";
      cin >> choice;
      if (choice < 1 || choice > tag-1) {
        cout << "#    ERROR : WRONG INDEX" << endl;
        cout << "#" << endl;
      } else {
        list->delNode(index + choice - 1);
        
        if (!dirty)
          dirty = true;

        cout << "#    DATA DELETE" << endl;
        cout << "#" << endl;
      }
      break;
    case '2':
      char answer[100];
      cout << "#    Are you sure ? (y,N) : ";
      cin >> answer;

      if (answer[0] == 'Y' || answer[0] == 'y') {
        cout << "#    LIST INITIIALIZATION" << endl;
        cout << "#" << endl;
        initList();
      }
      break;
    case '3':
      cout << "#" << endl;
      break;
    case '4':
      cout << "#" << endl;
      if (!hasPrev)
        manageList(index + 10);
      else
        manageList(index - 10);
      break;
    case '5':
      cout << "#" << endl;
      manageList(index + 10);
      break;
  }
}

bool VocaEngine::dupCheck(char* str) {
  char *tmp = new char[sizeof(char) * Strlen(str) + 1];
  Strcpy(tmp, str);
  
  for (unsigned int i = 0; i < list->getSize(); i++)
    if (Strequal(list->getContent(i)->getWord(), tmp))
      return true;

  return false;
}

VocaEngine::VocaEngine(istream *i)
{
  printTitle();
  
  bool loaded = false;

  list = new List <Voca*>();
  dirty = false;

  while (!i->eof() && !i->bad() && i->peek() != -1) {
    if (!loaded) // flag on
      loaded = true;

    // fill out previous list
    char word[100];
    char mean[100];

    // get word
    int index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      word[index++] = i->get();
    word[index] = '\0';
    
    if (i->peek() == '%') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() == '$'
      cout << "#    DATA FILE ERROR" << endl;
      exit(1);
    }
    
    // get mean
    index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      mean[index++] = i->get();
    mean[index] = '\0';
    
    if (!list->addNode(new Voca(word, mean))) {
      cout << "#    DATA GENERATING ERROR" << endl;
      exit(1);
    }

    if (i->peek() == '$') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() != '%'
      cout << "#    DATA FILE ERROR" << endl;
      exit(1);
    }

    while (isWhite(i->peek()))
      i->get(); // consume white space
  }
  if (loaded)
    cout << "#    DATA FILE LOADING COMPLETE" << endl;
  else // no prev data
    cout << "#    CREATE NEW DATA FILE" << endl;

  cout << "#" << endl;
}

VocaEngine::~VocaEngine()
{
  if(saveChange())
    cout << "#    SAVE DATA..." << endl;
  else
    cout << "#    NO UPDATE" << endl;

  if (list)
    delete(list);

  printEnd();
}

bool VocaEngine::addVoca()
{
  char word[100];
  char mean[100];

  cout << "#    WORD : ";
  cin >> word;

  if (dupCheck(word)) { // duplicated
    cout << "#    ALREADY EXIST!!" << endl;
    cout << "#" << endl;
    return true;
  }

  cout << "#    MEANING : ";
  cin >> mean;
  cout << "#" << endl;

  if (list->addNode(new Voca(word, mean))) {
    if (!dirty)
      dirty = true;
    return true;
  }

  return false;
}

bool VocaEngine::initList()
{
  if (list)
    delete(list);
  
  list = new List <Voca*>();
  
  if (!dirty)
    dirty = true;
}

bool VocaEngine::saveChange()
{
  ofstream *o = NULL;
  if (dirty) {
    o = new ofstream(FILENAME); 
    for (unsigned int i = 0; i < list->getSize(); i++) {
      char* word = list->getContent(i)->getWord();
      char* meaning = list->getContent(i)->getMean();
      
      cout << "#    write " << word << " " << meaning << endl;

      int pnt = 0;
      while(word[pnt] != '\0')
        o->put(word[pnt++]);
      // put token
      o->put('%');

      pnt = 0;
      while(meaning[pnt] != '\0')
        o->put(meaning[pnt++]);
      o->put('$');
    }
    
    o->close();

    return true;
  } else {
    return false;
  }
  return false;
}

void VocaEngine::showMenu()
{
  cout << "#               [ MENU ]" << endl;
  cout << "#    (1) ADD" << endl;
  cout << "#    (2) LIST" << endl;
  cout << "#    (3) TEST" << endl;
  cout << "#    (4) EXIT" << endl;
  cout << "#" << endl;
}

bool VocaEngine::processMenu()
{
  bool good = true; // ret val

  cout << "#    SELECT : ";

  char input[100];
  char yesOrNo;
  char word[100];
  char mean[100];
  cin >> input;

  if (input[0] < '1' || input[0] > '4') {
    cout << "#    WRONG INPUT" << endl;
    cout << "#" << endl;
    return true;
  }

  switch(input[0]) {
    case '1':
      if (addVoca()) {
        good = true;
      } else {
        cout << "#    ERROR : ADD ERROR" << endl;
        cout << "#" << endl;
        good = false;
      }
      break;
    case '2':
      manageList(0);
      break;
    /*
    case '3':
      cout << "PROGRAM TERMINATION" << endl;
      if(engine->saveChange())
        cout << "SAVE DATA..." << endl;
      else
        cout << "NO SAVE" << endl;
      pExit = true;
      break;
    */
    case '4':
      cout << "#    PROGRAM TERMINATION" << endl;
      cout << "#" << endl;
      good = false;
      break;
  }

  return good;
}
