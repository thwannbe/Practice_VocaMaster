////////////////////////////////////////////////////////////////////////////////
///
/// @file VocaMaster.h
/// @brief VocaMaster Header File
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

#ifndef __VOCAMASTER__
#define __VOCAMASTER__

#include <iostream>
#include <fstream>
#include "list.h"

using namespace std;

class Voca
{
private:
  char* word;
  char* meaning;
  int exp;
  int level;

public:
  Voca();
  Voca(char* w, char* m);
  ~Voca();

  char* getWord(void);
  char* getMean(void);
  int getExp(void);
  int getLevel(void);
};

class VocaEngine
{
private:
  List <Voca*> *list;
  bool dirty;
  
  bool addVoca(void);
  bool initList(void);
  bool saveChange(void);
  int testVoca(void);
 
  void printTitle(void);
  void printEnd(void);
  void manageList(int index);
  bool dupCheck(char* str);
public:
  VocaEngine(istream *i);
  ~VocaEngine(void);

  void showMenu(void);
  bool processMenu(void);
};

#endif /* __VOCAMASTER__ */
