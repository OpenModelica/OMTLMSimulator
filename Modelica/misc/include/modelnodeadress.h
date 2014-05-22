// This may look like C code, but it is really -*- C++ -*-

#ifndef _modelnodeadress_h
#define _modelnodeadress_h

#include <cassert>
 
#include <iostream>
//!! \class ModelNodeAdress
//!!
//!!
//!!  Class used to store the "adress" of a model node.</p>
//!!
//!! \author
//!!  Tommy Persson
using std::ostream;
using std::cout;
using std::endl;


class ModelNodeAdress { 

friend ostream& operator<<(ostream& o, const ModelNodeAdress& v) ;
friend class ModelGraph ;

private:

  int id;

  //! The level the object is on.
  int level;

  //! The index for the level.
  int index;

public:

  //! Constructor
  ModelNodeAdress (){
    level = -1 ;
    index = -1 ;
    id = -1;
  } ;

  //! Constructor
  ModelNodeAdress (int l, int i){
    level = l ;
    index = i ;
  } ;

  //! Get the level for the node.
  int get_level () const { return level; };

  //! Get the index for the node.
  int get_index () const { return index; };

  //! Set the level for the node.
  void set_level (int lvl) { level = lvl; };

  //! Set the index for the node.
  void set_index (int i) { index = i; };

  //! Print the address.
  void print () const {
    cout << "ModelNodeAdress: " << level << "    " << index << endl;
  } ;


#if 0
  //! Pack the data.
  void pack () {
    ///    cout << "Packing MNA: ";
    /// print ();
    mppServer.Pack (id);
    mppServer.Pack (level);
    mppServer.Pack (index);
  };

  //! Unpack the data.
  void unpack () {
    mppServer.UnPack (id);
    mppServer.UnPack (level);
    mppServer.UnPack (index);
    ///cout << "UNPacking MNA: ";
    ///print ();
  };
#endif

};


inline ostream& operator<<(ostream& o, const ModelNodeAdress& v){
  o << "(" << v.level << ", " << v.index << ")" ;
  return o ;
}


#endif
