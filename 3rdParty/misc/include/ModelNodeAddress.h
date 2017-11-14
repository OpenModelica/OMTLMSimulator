#ifndef MODELNODEADDRESS_H
#define MODELNODEADDRESS_H
 
#include <iostream>
//!! \class ModelNodeAddress
//!!
//!!
//!!  Class used to store the "adress" of a model node.</p>
//!!
//!! \author
//!!  Tommy Persson
using std::ostream;
using std::cout;
using std::endl;


class ModelNodeAddress {

friend ostream& operator<<(ostream& o, const ModelNodeAddress& v) ;
friend class ModelGraph ;

private:



  //! The level the object is on.
  int level;

  //! The index for the level.
  int index;

public:

  //! Constructor
  ModelNodeAddress(){
    level = -1;
    index = -1;

  }

  //! Constructor
  ModelNodeAddress(int l, int i){
    level = l;
    index = i;

  }

  //! Get the level for the node.
  inline int get_level() const { return level; }

  //! Get the index for the node. Can be negative.
  inline int get_index() const { return index; }

  //! Set the level for the node.
  inline void set_level(const int lvl) { level = lvl; }

  //! Set the index for the node.
  inline void set_index(const int i) { index = i; }

  //! Print the address.
  void print() const {
    cout << "ModelNodeAddress: " << level << "    " << index << endl;
  }

};


inline ostream& operator<<(ostream& o, const ModelNodeAddress& v){
  o << "(" << v.level << ", " << v.index << ")" ;
  return o ;
}


#endif
