#include <cstdlib>
#include <cstdio>
 
#include <iostream>

#include "lightmat.h"
#include "Bstring.h"
#include "PrivPack.h"


int main(int argc, char *argv[]){
    aPackBuf b ;
    Bstring s ;
    doubleVec3   v3 ;
    doubleMat33 m33 ;
    double d1, d2 ;
    double d3[3] ;
    int i1, i2 ;
    int i3[3] ;
    packbuf_size_t p ;
    
    if(argc>1){
	cout << "Just Reading From File: " << Bstring(argv[1]) << endl ;
	b.readFromFile(argv[1]) ;
	
	b.pack(Up, m33  ) ;
	b.pack(Up, s    ) ;
	b.pack(Up, i1   ) ;
	b.pack(Up, v3   ) ;
	b.pack(Up, d1   ) ;
	b.pack(Up, d3, 3) ;
	b.pack(Up, d2   ) ;
	b.pack(Up, i2   ) ;
	b.pack(Up, i3, 3) ;
	
	cout << "s      :" << s      << endl ;
	cout << "v3     :" << v3     << endl ;
	cout << "m33    :" << m33    << endl ;
	cout << "d1     :" << d1     << endl ;
	cout << "d2     :" << d2     << endl ;
	cout << "d3[0]  :" << d3[0]  << endl ;
	cout << "d3[1]  :" << d3[1]  << endl ;
	cout << "d3[2]  :" << d3[2]  << endl ;
	cout << "i1     :" << i1     << endl ;
	cout << "i2     :" << i2     << endl ;
	cout << "i3[0]  :" << i3[0]  << endl ;
	cout << "i3[1]  :" << i3[1]  << endl ;
	cout << "i3[2]  :" << i3[2]  << endl ;
	return 0 ;
    }
    
    
    s = "Freethinkers reject faith as a valid tool of knowledge. Faith is the opposite of reason because reason imposes very strict limits on what can be true, and faith has no limits at all. A Great Escape into faith is no retreat to safety. It is nothing less than surrender. Dan Barker, Losing Faith in Faith: From Preacher to Atheist (Madison, WI: FFRF, 1992), p. 103. " ;

    v3 = doubleVec3(4,5,6) ;
    m33 = doubleMat33(1.5, 2.5, 3.5,
		      4.5, 6.5, 7.5,
		      8.5, 9.5, 0.5) ;
    d1 = 3.1 ;
    d2 = 2.7 ;    
    d3[0] = 0.1 ;
    d3[1] = 0.2 ;
    d3[2] = 0.3 ; 
    i1 = 78 ;
    i2 = 1028 ;
    i3[0] = -3 ;
    i3[1] = -4 ;
    i3[2] = -5 ;

    cout << "s      :" << s      << endl ;
    cout << "v3     :" << v3     << endl ;
    cout << "m33    :" << m33    << endl ;
    cout << "d1     :" << d1     << endl ;
    cout << "d2     :" << d2     << endl ;
    cout << "d3[0]  :" << d3[0]  << endl ;
    cout << "d3[1]  :" << d3[1]  << endl ;
    cout << "d3[2]  :" << d3[2]  << endl ;
    cout << "i1     :" << i1     << endl ;
    cout << "i2     :" << i2     << endl ;
    cout << "i3[0]  :" << i3[0]  << endl ;
    cout << "i3[1]  :" << i3[1]  << endl ;
    cout << "i3[2]  :" << i3[2]  << endl ;
    
    cout << "Start Packing" << endl ;

    b.pack(Down, m33  ) ;
    b.pack(Down, s    ) ;
    b.pack(Down, i1   ) ;
    b.pack(Down, v3   ) ;
    b.pack(Down, d1   ) ;
    b.pack(Down, d3, 3) ;
    b.pack(Down, d2   ) ;
    b.pack(Down, i2   ) ;
    b.pack(Down, i3, 3) ;
    
    cout << "Done Packing" << endl ;
    p = b.Packed() ;
    
#ifdef PP3ARR    
    cout << "Has Packed "
	 << p.ints << " Ints, " 
	 << p.doubles << " Doubles, " 
	 << p.chars << " Chars " << endl ;
#else
    cout << "Has Packed "
	 << p << " Bytes " << endl ;
#endif
    
    
    cout << "Dumping Buffer" << endl ;
    b.print() ;
    
    cout << "Writing To File" << endl ;
    
    b.saveToFile("pp.dump") ;
    
    cout << "Doing Unpacking" << endl ;
    
    b.reset() ;    
    b.pack(Up, m33  ) ;
    b.pack(Up, s    ) ;
    b.pack(Up, i1   ) ;
    b.pack(Up, v3   ) ;
    b.pack(Up, d1   ) ;
    b.pack(Up, d3, 3) ;
    b.pack(Up, d2   ) ;
    b.pack(Up, i2   ) ;
    b.pack(Up, i3, 3) ;
    
    cout << "Done Unpacking" << endl ;
    
    cout << "s      :" << s      << endl ;
    cout << "v3     :" << v3     << endl ;
    cout << "m33    :" << m33    << endl ;
    cout << "d1     :" << d1     << endl ;
    cout << "d2     :" << d2     << endl ;
    cout << "d3[0]  :" << d3[0]  << endl ;
    cout << "d3[1]  :" << d3[1]  << endl ;
    cout << "d3[2]  :" << d3[2]  << endl ;
    cout << "i1     :" << i1     << endl ;
    cout << "i2     :" << i2     << endl ;
    cout << "i3[0]  :" << i3[0]  << endl ;
    cout << "i3[1]  :" << i3[1]  << endl ;
    cout << "i3[2]  :" << i3[2]  << endl ;
    
    cout << "Reading From File" << endl ;
    
    b.readFromFile("pp.dump") ;
       
    cout << "s      :" << s      << endl ;
    cout << "v3     :" << v3     << endl ;
    cout << "m33    :" << m33    << endl ;
    cout << "d1     :" << d1     << endl ;
    cout << "d2     :" << d2     << endl ;
    cout << "d3[0]  :" << d3[0]  << endl ;
    cout << "d3[1]  :" << d3[1]  << endl ;
    cout << "d3[2]  :" << d3[2]  << endl ;
    cout << "i1     :" << i1     << endl ;
    cout << "i2     :" << i2     << endl ;
    cout << "i3[0]  :" << i3[0]  << endl ;
    cout << "i3[1]  :" << i3[1]  << endl ;
    cout << "i3[2]  :" << i3[2]  << endl ;
    
    return 0 ;
}
