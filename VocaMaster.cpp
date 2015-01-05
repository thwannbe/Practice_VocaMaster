////////////////////////////////////////////////////////////////////////////////
///
/// @file VocaMaster.cpp
/// @brief VocaMaster Source File
/// @details Vocabulary Practice Program
/// @author Suwon Oh <suwon@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2014/12/23 Suwon Oh created @n
/// 2014/12/23 Suwon Oh adapted to Doxygen @n
/// 
/// @section purpose_section Purpose
/// Application for self-study
///

#include <cstdlib>
#include "VocaMaster.h"

#define FILENAME "voca.dat"

using namespace std;

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

int main(void) {
  ifstream *iFile = new ifstream(FILENAME); // fixed file name

  VocaEngine *engine = new VocaEngine(iFile);  
  
  bool pExit = false;
  while(!pExit) {
    cout << "[MENU] (1) ADD (2) INIT (3) EXIT" << endl;
    cout << "INPUT : ";

    char input[100];
    char yesOrNo;
    char word[100];
    char mean[100];
    cin >> input;
    
    if (input[0] < '1' || input[0] > '3') {
      cout << "WRONG INPUT" << endl;
      continue;
    }
    
    switch(input[0]) {
      case '1':
        cout << "WORD : ";
        cin >> word;
        cout << "MEAN : ";
        cin >> mean;
        engine->addVoca(word, mean);
        break;
      case '2':
        cout << "Are you sure ? (y,N) ";
        cin >> yesOrNo;
        if (yesOrNo == 'Y' || yesOrNo == 'y') {
          cout << "LIST INITIALIZATION" << endl;
          engine->initList();
        }
        break;
      case '3':
        cout << "PROGRAM TERMINATION" << endl;
        if(engine->saveChange())
          cout << "SAVE DATA..." << endl;
        else
          cout << "NO SAVE" << endl;
        pExit = true;
        break;
      default:
        cout << "WRONG INPUT" << endl;
        break;
    }
  }

  iFile->close();

  return 0;
}

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

VocaEngine::VocaEngine(istream *i)
{
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
      cout << "DATA FILE ERROR" << endl;
      exit(1);
    }
    
    // get mean
    index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      mean[index++] = i->get();
    mean[index] = '\0';
    
    if (!addVoca(word, mean)) {
      cout << "DATA GENERATING ERROR" << endl;
      exit(1);
    }

    if (i->peek() == '$') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() != '%'
      cout << "DATA FILE ERROR" << endl;
      exit(1);
    }

    while (isWhite(i->peek()))
      i->get(); // consume white space
  }
  if (loaded)
    cout << "DATA FILE LOADING COMPLETE" << endl;
  else // no prev data
    cout << "CREATE NEW DATA FILE" << endl;

  if (dirty) // loading prev data not affect dirty bit
    dirty = false;
}

VocaEngine::~VocaEngine()
{
  if (list)
    delete(list);
}

bool VocaEngine::addVoca(char* w, char* m)
{
  Voca *tmp = new Voca(w, m);

  if (!dirty)
    dirty = true;

  if (list->addNode(tmp))
    return true;
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
      char* word = list->getContent(i)->word;
      char* meaning = list->getContent(i)->meaning;
      
      cout << "write " << word << " " << meaning << endl;

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
