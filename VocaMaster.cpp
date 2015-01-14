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
/// 2015/01/13 Suwon Oh finished test algorithm @n
/// 2015/01/14 Suwon Oh test complishment added @n
///
/// @section purpose_section Purpose
/// Application for self-study
///

#include <cstdlib>
#include <ctime>
#include "VocaMaster.h"

#define FILENAME      "voca.dat"
#define VERSION       1.2
#define TRACE_SAVE    0

using namespace std;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief Self-implemented multi-handy inline functions
/// @details In this program, there are many functions handling string @n
///          but for not using string header library, most string handling @n
///          functions are created based on how it works basically.
///

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

static inline int StrToInt(char* str) {
  int ret = 0;

  for (int i = 0; i < Strlen(str); i++)
    ret = (ret * 10) + (int)(str[i] - '0');

  return ret;
}

static inline char* IntToStr(int num) {
  int tmp = num;
  int length = 0;
  
  if (num == 0) {
    char *ret = new char[2];
    ret[0] = '0'; ret[1] = '\0';
    return ret;
  }

  while (tmp > 0) {
    tmp /= 10;
    length++;
  }

  char *buf = new char[sizeof(char) * length + 1];
  
  int index = length - 1;
  for (tmp = num; tmp > 0; tmp /= 10)
    buf[index--] = (char)(tmp % 10 + '0');
  buf[length] = '\0';

  return buf;
}

////////////////////////////////////////////////////////////////////////////////
///
/// @brief main function
///

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

////////////////////////////////////////////////////////////////////////////////
///
/// @brief Voca class functions implementation
///

Voca::Voca() : exp(0), level(1)
{
  word = NULL;
  meaning = NULL;
  explain = NULL;
}

Voca::Voca(char* w, char* m, char* e, int x, int l) : exp(x), level(l)
{
  word = new char[sizeof(char) * Strlen(w) + 1];
  Strcpy(word, w);

  meaning = new char[sizeof(char) * Strlen(m) + 1];
  Strcpy(meaning, m);

  explain = new char[sizeof(char) * Strlen(e) + 1];
  Strcpy(explain, e);
}

Voca::~Voca()
{
  if (word)
    delete(word);
  if (meaning)
    delete(meaning);
  if (explain)
    delete(explain);
}

char* Voca::getWord() {
  return word;
}

char* Voca::getMean() {
  return meaning;
}

char* Voca::getExplain() {
  return explain;
}

int Voca::getExp() {
  return exp;
}

int Voca::getLevel() {
  return level;
}

void Voca::gainScore() {
  exp += (MAX_LEVEL - level + 1) * 10; // MAX_LEVEL should be lower than 10

  if (exp >= 100) { // maximum exp is 100
    if (level < MAX_LEVEL) {
      exp -= 100;
      level++;
    } else {
      exp = 100;
    }
  }
}

void Voca::loseScore() {
  exp -= level * 10; // MAX_LEVEL should be lower than 10

  if (exp < 0) {
    if (level == 1) {
      exp = 0;
    } else {
      exp += 100;
      level--;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
///
/// @brief VocaEngine class private fundamental functions implementation
///

bool VocaEngine::addVoca()
{
  char word[100];
  char mean[100];
  char explain[100];

  cout << "#    WORD : ";
  cin >> word;

  if (dupCheck(word)) { // duplicated
    cout << "#    ALREADY EXIST!!" << endl;
    cout << "#" << endl;
    return true;
  }

  cout << "#    MEANING : ";
  cin >> mean;

  cout << "#    EXPLAIN : "; // can be null
  cin >> explain;
  cout << "#" << endl;

  if (list->addNode(new Voca(word, mean, explain, 0, 1))) {
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
      char* explain = list->getContent(i)->getExplain();
      char* exp_str = IntToStr(list->getContent(i)->getExp());
      char* level_str = IntToStr(list->getContent(i)->getLevel());
      
#if TRACE_SAVE
      cout << "#    write " << word << " " << meaning << " "
           << explain << " " << exp_str << " " << level_str << endl;
#endif

      int pnt = 0;
      while(word[pnt] != '\0') {
        o->put(word[pnt++]);
      }
      o->put('%');

      pnt = 0;
      while(meaning[pnt] != '\0') {
        o->put(meaning[pnt++]);
      }
      o->put('%');

      pnt = 0;
      while(explain[pnt] != '\0') {
        o->put(explain[pnt++]);
      }
      o->put('%');

      pnt = 0;
      while(exp_str[pnt] != '\0') {
        o->put(exp_str[pnt++]);
      }
      o->put('%');

      pnt = 0;
      while(level_str[pnt] != '\0') {
        o->put(level_str[pnt++]);
      }
      o->put('$');
    }
    
    o->close();
    
    return true;
  } else {
    return false;
  }
  return false;
}

Voca* VocaEngine::selectVoca() {
  srand(time(0));
  int index = rand() % list->getSize();

  while (!levelPenalty(list->getContent(index)))
    index = rand() % list->getSize();

  return list->getContent(index);
}

bool VocaEngine::levelPenalty(Voca* voca) {
  srand(time(0));
  int coefficient = (rand() % voca->MAX_LEVEL) + 1; // 1 ~ MAX_LEVEL
  
  if ((voca->getLevel() - coefficient) <= 0)
    return true; // means select
  else
    return false; // means unselect
}

bool VocaEngine::dupCheck(char* str) {
  char *tmp = new char[sizeof(char) * Strlen(str) + 1];
  Strcpy(tmp, str);
  
  for (unsigned int i = 0; i < list->getSize(); i++)
    if (Strequal(list->getContent(i)->getWord(), tmp))
      return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
///
/// @brief VocaEngine class private abstract functions implementation
///

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

void VocaEngine::testVoca(bool first, int correct, int total) {
  int cor = correct;

  if (list->getSize() == 0) {
    cout << "#" << endl;
    cout << "#             EMPTY LIST" << endl;
    cout << "#" << endl;
    return;
  }
  
  if (first) {
    cout << "#                [ TEST ]" << endl;
    cout << "#    Write appropriate word matching following meanings." << endl;
  }
  cout << "#" << endl;
  Voca *one = selectVoca();
  cout << "#    " << one->getMean() << " : ";
  
  char buf[100]; cin >> buf;
  char *answer = new char[sizeof(char) * Strlen(buf) + 1];
  Strcpy(answer, buf);

  if (Strequal(one->getWord(), answer)) {
    cout << "#    COLLECT!" << endl;
    one->gainScore();
    cor++;
    cout << "#" << endl;
  } else {
    cout << "#    WRONG!" << endl;
    if (one->getExplain() != NULL)
      cout << "#    COLLECT ANSWER IS [" << one->getWord()
           << " -- " << one->getExplain() << "]" << endl;
    else
      cout << "#    COLLECT ANSWER IS [" << one->getWord() << "]" << endl;
    one->loseScore();
    cout << "#" << endl;
  }

  if (!dirty)
    dirty = true;
  
  cout << "#    (1) NEXT TEST (2) EXIT" << endl;
  cout << "#    SELECT : ";
  char sel[100]; cin >> sel;

  if (sel[0] != '1') {
    if (sel[0] != '2') {
      cout << "#    WRONG INPUT" << endl;
      cout << "#" << endl;
    }
    cout << "#" << endl;
    cout << "#               [ TEST RESULT ]" << endl;
    cout << "#    TOTAL PROBLEM  : " << total+1 << endl;
    cout << "#    CORRECT ANSWER : " << cor << endl;
    cout << "#    WRONG ANSWER   : " << total+1 - cor << endl;
    cout << "#    SUCCESS RATE   : " << ((cor * 100) / (total + 1)) << "%" << endl;
    cout << "#" << endl;
  } else {
    testVoca(false, cor, total+1);
  }
}

////////////////////////////////////////////////////////////////////////////////
///
/// @brief VocaEngine class constructors and destructors implementation
///

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
    char explain[100];
    char exp_buf[100];
    char level_buf[100];
    int exp, level;

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
    
    if (i->peek() == '%') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() == '$'
      cout << "#    DATA FILE ERROR" << endl;
      exit(1);
    }

    // get explain
    index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      explain[index++] = i->get();
    explain[index] = '\0';
    
    if (i->peek() == '%') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() == '$'
      cout << "#    DATA FILE ERROR" << endl;
      exit(1);
    }

    // get exp
    index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      exp_buf[index++] = i->get();
    exp_buf[index] = '\0';
    exp = StrToInt(exp_buf);
    
    if (i->peek() == '%') {
      i->get(); // consume token
    } else { // i->eof() || i->bad() || i->peek() == '$'
      cout << "#    DATA FILE ERROR" << endl;
      exit(1);
    }

    // get level
    index = 0;
    while (i->peek() != '%' && i->peek() != '$' && !i->eof() && !i->bad())
      level_buf[index++] = i->get();
    level_buf[index] = '\0';
    level = StrToInt(level_buf);

    if (!list->addNode(new Voca(word, mean, explain, exp, level))) {
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

////////////////////////////////////////////////////////////////////////////////
///
/// @brief VocaEngine class public abstract control functions implementation
///

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
      good = true;
      break;
    case '3':
      testVoca(true, 0, 0);
      good = true;
      break;
    case '4':
      cout << "#    PROGRAM TERMINATION" << endl;
      cout << "#" << endl;
      good = false;
      break;
  }

  return good;
}
