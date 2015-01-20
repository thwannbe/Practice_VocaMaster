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
/// 2015/01/13 Suwon Oh finished test algorithm @n
/// 2015/01/14 Suwon Oh test complishment added @n
/// 2015/01/20 Suwon Oh search feature added @n
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
  char* word;               ///< Vocabulary word
  char* meaning;            ///< Vocabulary meaning (in your language)
  char* explain;            ///< Vocabulary additional explanation
  int exp;                  ///< Vocabulary experience gauge
  int level;                ///< Vocabulary level information

public:
  static const int MAX_LEVEL = 5;  ///< Maximum level range
  
  /// @name constructors
  /// @{
  
  /// @brief default constructor
  /// @details Defined for empty Voca instance
  Voca();

  /// @brief constructor having w, m, e, x, and l
  /// @details Defined for creating fill-out Voca instance
  /// @param w word string
  /// @param m meaning string
  /// @param e explanation string
  /// @param x experience score
  /// @param l level point
  Voca(char* w, char* m, char* e, int x, int l);
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

  /// @brief getting explanation
  ///
  /// @retval explanation string
  char* getExplain(void);

  /// @brief getting experience score
  ///
  /// @retval experience score
  int getExp(void);

  /// @brief getting level point
  ///
  /// @retval level point
  int getLevel(void);
  /// @}
  
  /// @name functional attributes
  /// @{

  /// @brief gaining experience score
  /// @details If test success, this word gain experience score @n
  ///          and have a chance to get upper level point.
  void gainScore(void);

  /// @brief losing experience score
  /// @details If test fail, this word lose experience score @n
  ///          and might be lowered its level point.
  void loseScore(void);
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
  
  /// @name private fundamental functional attributes
  /// @{

  /// @brief adding new vocabulary
  ///
  /// @retval true if adding success
  /// @retval false if adding fails
  bool addVoca(void);

  /// @brief initializing vocabulary list
  ///
  /// @retval true if initialization success
  /// @retval false if initialization fail
  bool initList(void);

  /// @brief saving updated data
  ///
  /// @retval true if save success
  /// @retval false if save fail
  bool saveChange(void);

  /// @brief selecting one word
  ///
  /// @retval vocabulary class pointer
  Voca* selectVoca(void);

  /// @brief calculating level penalty and return boolean value
  ///
  /// @retval true if this voca pass level penalty
  /// @retval false if this voca fail level penalty
  bool levelPenalty(Voca* voca);

  /// @brief duplicated checking
  ///
  /// @param str target string
  /// @retval true want to continue adding voca
  /// @retval false stop to add voca
  bool dupCheck(char* str);

  /// @brief finding similar vocabulary
  ///
  /// @param str target string
  /// @retval true if match || similar
  /// @retval false if no match
  bool findSim(char* str);
  /// @}

  /// @name private abstract functional attributes
  /// @{

  /// @brief printing title to console
  void printTitle(void);

  /// @brief printing ending to console
  void printEnd(void);

  /// @brief managing list menu
  ///
  /// @param index starting voca list index
  void manageList(int index);

  /// @brief searching a vocabulary in the list
  void searchVoca(void);
  
  /// @brief vocabulary test function
  ///
  /// @param first flag whether this test is first time
  /// @param correct correct answer counts
  /// @param total total test counts
  void testVoca(bool first, int correct, int total);
  /// @}

public:
  /// @name constructors
  /// @{

  /// @brief constructor having istream pointer
  /// @details Loading previous vocabulary data list @n
  ///          and initialize all member variables.
  VocaEngine(istream *i);
  /// @}

  /// @name destructors
  /// @{

  /// @brief default destructor
  /// @details Saving automatically updated data into disk @n
  ///          and delete all data in memory.
  ~VocaEngine(void);
  /// @}
  
  /// @name abstract control attributes
  /// @{

  /// @brief showing menu to console
  void showMenu(void);

  /// @brief processing menu with user
  /// @details communicating with user through console I/O
  ///
  /// @retval true if program continue
  /// @retval false if program termination
  bool processMenu(void);
  /// @}
};

#endif /* __VOCAMASTER__ */
