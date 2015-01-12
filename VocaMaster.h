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

////////////////////////////////////////////////////////////////////////////////
///
/// @brief Vocabulary Entry Class
/// @details In this program, each vocabulary has its own Voca class, which has @n
///          vocabulary meta-data. VocaEngine handle all process using this Voca @n
///          Class. It is a data-based class. 'exp' & 'level' are informations @n
///          about user's frequency of the vocabulary. Higher 'level' means that @n
///          user remember this word well. Answering collectly in test, user can @n
///          gain 'exp' score from VocaEngine, and this score upgrades Voca's level.
/// 

class Voca
{
private:
  char* word;         ///< Vocabulary word
  char* meaning;      ///< Vocabulary meaning (in your language)
  int exp;            ///< Vocabulary experience gauge
  int level;          ///< Vocabulary level information

public:
  /// @name constructors
  /// @{
  
  /// @brief default constructor
  /// @details Defined for empty Voca instance
  Voca();

  /// @brief constructor having w, and m
  /// @details Defined for creating fill-out Voca instance
  /// @param w word string
  /// @param m meaning string
  Voca(char* w, char* m);
  /// @}

  /// @name destructor
  /// @{

  /// @brief default destructor
  /// @details Deleting its strings
  ~Voca();
  /// @}


  /// @name informative attributes
  /// @{
  
  /// @brief getting word string
  ///
  /// @retval word string
  char* getWord(void);

  /// @brief getting meaning string
  ///
  /// @retval meaning string
  char* getMean(void);

  /// @brief getting experience score
  ///
  /// @retval experience score
  int getExp(void);

  /// @brief getting level point
  ///
  /// @retval level point
  int getLevel(void);
  /// @}
};

////////////////////////////////////////////////////////////////////////////////
///
/// @brief Vocabulary Master Program Engine Class
/// @details Controller class which add, show the list, test, or exit program @n
///          using Voca class. Voca list is based on self-implemented List class @n
///          (details on "list.h" at same folder).
///

class VocaEngine
{
private:
  List <Voca*> *list;     ///< Voca class list
  bool dirty;             ///< dirty bit which means an update exists
  
  /// @name private functional attributes
  /// @{

  /// @brief adding new vocabulary
  ///
  /// @retval true if adding success
  /// @retval false if adding fails
  bool addVoca(void);

  bool initList(void);
  bool saveChange(void);
  Voca* selectVoca(void);
  bool dupCheck(char* str);
  
  void printTitle(void);
  void printEnd(void);
  void manageList(int index);
  void testVoca(void);
public:
  VocaEngine(istream *i);
  ~VocaEngine(void);

  void showMenu(void);
  bool processMenu(void);
};

#endif /* __VOCAMASTER__ */
