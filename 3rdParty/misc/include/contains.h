#ifndef CONTAINS_H
#define CONTAINS_H
#include <set>
template<class T>
   bool contains(T one,std::set<T> v){
   return v.find(one)!=v.end();
}
// typically use as bool b=contains(6,{7,8,9})
#endif // CONTAINS_H
