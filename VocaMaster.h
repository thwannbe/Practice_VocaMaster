////////////////////////////////////////////////////////////////////////////////
///
/// @file VocaMaster.h
/// @brief VocaMaster Header File
/// @details Vocabulary Practice Program
/// @author Suwon Oh <suwon@csap.snu.ac.kr>
/// @section changelog Change Log
/// 2014/12/23 Suwon Oh created @n
/// 2014/12/23 Suwon Oh adapted to Doxygen @n
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
public:
  char* word;
  char* meaning;

  Voca();
  Voca(char* w, char* m);
  ~Voca();
};

class VocaEngine
{
public:
  List <Voca*> *list;
  bool dirty;

public:
  VocaEngine(istream *i);
  ~VocaEngine(void);
  
  bool addVoca(char* w, char* m);
  bool delVoca(char* w);
  bool initList(void);
  bool saveChange(void);
  int testVoca(void);
};

#endif /* __VOCAMASTER__ */
