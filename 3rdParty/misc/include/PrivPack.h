#ifndef PrivPack_h
#define PrivPack_h
#include <cassert>
#include <ostream>
#include <vector>

#include "double33.h"
#include "double33s.h"
#include "double3.h"
#include "doubleN33.h"
#include "doubleN33s.h"
#include "doubleN3.h"
#include "doubleN.h"
#include "double2.h"
#include "intN.h"
#include "portable_long.h"
#include "portable_longdouble.h"

#include "Bstring.h"
#include "newmat.h"

#if defined(TALK_THREADED_PACK_IN_MASTER) && defined(USE_THREADED_PACK_IN_MASTER)
#include "abasic.h"
#endif


#ifdef PRINT_DIRECT_ARR_PK_BACKTRACE
  #include <execinfo.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <cxxabi.h>
#endif

static const double MAX_ALLOWED_NUMBER_TO_BE_PACKED = 1e100;

/*
 *
 
 Revision 1.2  1996/03/08 16:33:19  patno
 Some commenting of the code.

 Revision 1.1.1.1  1996/02/08 16:14:40  patno
 Imported misc

 Revision 1.6  1995/11/28 13:00:09  patno
 added direct_arr_(u)pk methods for (un)packing arrays. 


 Changed data area index for intN, doubleN, doubl3N, doubleN33
 Dag Fritzson 2004-12-06

 *
 */

// This define will also fix hetrogen in Communicator.h
#define HETROGEN

#define SKF_PACK

//#define TALK_PRIVPACK
// #define TALK_ON_FILE_PRIVPACK


#ifdef HETROGEN
#define PP3ARR
#endif

#ifdef SKF_PACK
enum PackType { Down, Up, NoWay };
#endif


/*
   <cd> aPackBuf

   <s> Functionality
   <p>
   A class to pack integers floats doubles etc. into a buffer. It is 
   intended to be a <q>light</q> class, no extra features than 
   necessary is implemented.
   <p>
   An instance of this class can pack data into a buffer. The
   buffer is of type <f>buftyp</f> (must essentially be char, otherwise
   alignement problems will occur). The buffer is kept internally 
   within the object.
   It can pack integers, floats, doubles, longdoubles, double3, double33, and
   Bstring. Also packing and unpacking of arrays of integers, floats, doubles and longdoubles
   are supported. A buffer will not allocate any memory for the application, 
   that is, if an array should be upacked, the pointer should point enough 
   memory to be unpacked.   
   
   <p>
   We align the pointer so that we can cast lvalue to the
   appropirate type. Due to the alignment we may have to allocate some
   <q>extra</q> memory. This is taken care of by the operators and the methods. 
   Note that to pack <code>int x; double y ;</code> will require 16 bytes 
   and not 12. 
   
   <s> Summary of usage.
   <p>
   The accual (un)packing is done with the operators <op> >> </op> and 
   <op> << </op>, or with 
   the methods <m>pk(...)</m> and <m>upk(...)</m>.
    <longcode>
    aPackBuf bp ;

    // For packing data
    bp << x ;
    for(i=0;i<10;i++)
         bp << Y[i] ;

    // alt.  
    bp.pk(x) ;
    for(i=0;i<10;i++)
         bp.pk(Y[i]);

    // and for the unpacking :

    bp >> x ;
    for(i=0;i<10;i++)
         bp >> Y[i] ;

    // alt.  
    bp.upk(x) ;
    for(i=0;i<10;i++)
         bp.upk(Y[i]);
    </longcode>
   <p>
   Performance : with alignment and inlining a doubleFuncCy2 spline
   object requires 1088 bytes and will be packed in 115 us on a 
   Sparc 10. with no alignment and no inlining the time becomes
   402 us, but the size of the buffer has shrink to 1044 bytes
   
<author>
<aname> Patrik Nordling

*/

///      Patrik Nordling PELAB L-ping 13/10-95
///          with thanks to Mikael Pettersson


enum packbuf_type_tag_s {
    INT_TYPE,
    DOUBLE_TYPE,
    CHAR_TYPE,
    FLOAT_TYPE,
    LONG_DOUBLE_TYPE
} ;

#ifdef PP3ARR

struct packbuf_size_s {    
    int ints ;
    int doubles ;
    int chars ;      
    int floats ;
    int longdoubles;
    
    packbuf_size_s(){ints=0; doubles=0; chars=0; floats=0;longdoubles=0; } ;
    packbuf_size_s(const int i, const int d, const int c, const int f, const int ld){
        ints=i; doubles=d; chars=c; floats=f;longdoubles=ld;
    } ;
#if 0    
    packbuf_size_s operator-(const packbuf_size_s &v){
        return packbuf_size_s(-v.doubles, -v.doubles, -v.chars, -v.floats, -v.longdoubles) ;
    }
#endif
    int bytes(){
    return(ints*sizeof(int) + doubles*sizeof(double) + chars*sizeof(char) + floats*sizeof(float)+ longdoubles*sizeof(longdouble)) ;
    }

    inline packbuf_size_s & operator=(packbuf_size_s &a){
      if(this != &a){
	ints    = a.ints ;
	doubles = a.doubles ;
	chars   = a.chars ;
	floats  = a.floats ;
    longdoubles=a.longdoubles;
      }
      return *this ;      
    }
    
} ;
struct buftyp_s {
    int    *iptr ;
    double *dptr ;
    char   *cptr ;    
    float  *fptr ;
    longdouble*ldptr;
    buftyp_s(){iptr=0; dptr=0; cptr=0; fptr=0;ldptr=0; } ;
} ;

typedef struct packbuf_size_s packbuf_size_t ;
typedef struct buftyp_s  bufptr ;

#else

/// the type of the buffer
typedef char    buftyp ;
typedef buftyp* bufptr  ;
typedef int packbuf_size_t ;

#endif



#define ALIGN_DBL_INT 
#define DEFAULT_INC_SIZE 128

class aPackBuf {
    friend std::ostream& operator<<(std::ostream& o, const aPackBuf& v) ;
    friend std::ostream& operator<<(std::ostream& o, const aPackBuf* v) ;

    
    //! Will point at the begining of the buffer.
    bufptr bpt_1 ;
    
    //! Will be incremented for every byte that is packed, the <b>running</b>
    //! pointer.
    bufptr bpt ;          
    
    //! Returns how much that is currently allocated for the buffer.
    packbuf_size_t allocedTheSize ; 
    
    //! The increment of the buffer when it owerflows and <b>realloc(3C)</b>
    //! must be called.
    packbuf_size_t default_inc ;

    //! A variable that caches the direction of the packing, used when 
    //! using pack() which doesnot take a direction argument.
    PackType way ;
    
#ifndef PP3ARR
    //! Aligns the pointer with respect of the size of the next object to be
    //! packed next.
    inline bufptr align(bufptr a, size_t size){
	return (bufptr) (((unsigned long)a + (size-1))&(~(size-1))) ;
    } ;
#endif
    

#ifdef PP3ARR
    inline bufptr alignoc(bufptr a, packbuf_type_tag_s typ, int len=1){	
	size_t size ;
	int alloced_for_type ;
	void *p, *p_1 ;
	switch(typ){
	case INT_TYPE:
	    size = sizeof(int);
	    alloced_for_type = allocedTheSize.ints ; 
	    p = a.iptr; p_1 = bpt_1.iptr ;
	    break ;
	case DOUBLE_TYPE:
	    size = sizeof(double);
	    alloced_for_type = allocedTheSize.doubles ; 
	    p = a.dptr; p_1 = bpt_1.dptr ;
	    break ;
	case FLOAT_TYPE:
	    size = sizeof(float);
	    alloced_for_type = allocedTheSize.floats ; 
	    p = a.fptr; p_1 = bpt_1.fptr ;
	    break ;
	case CHAR_TYPE:
	    size = sizeof(char);
	    alloced_for_type = allocedTheSize.chars ; 
	    p = a.cptr; p_1 = bpt_1.cptr ;
	    break ;
    case LONG_DOUBLE_TYPE:
        size = sizeof(longdouble);
        alloced_for_type = allocedTheSize.longdoubles ;
        p = a.ldptr; p_1 = bpt_1.ldptr ;
        break ;

	default:
        size = 0;  // to avoid warnings
        alloced_for_type = 0;
        p = 0;
        p_1 = 0;
	    assert(0) ;
	}	
	if( ( ((size_t)p - (size_t)p_1) + size*len) > (size_t)alloced_for_type){
	    get_more_mem(typ, len) ;
	    return bpt ;
	} else {
	    return a ;
	}
    } ;


#else
    //! Aligns the pointer with respect of the size to packed next and
    //! allocates more memory if nessecary.
    inline bufptr alignoc(bufptr a,  size_t size){	
	bufptr tmp = align(a, size) ;	
	if( ((size_t)(tmp - bpt_1) + size) > allocedTheSize ){
	    get_more_mem() ;
	    return align(bpt, size) ;
	} else {
	    return align(a, size) ;
	}
    } ;
    
    inline bufptr alignoclen(bufptr a, size_t size, int len){	
	bufptr tmp = align(a, size) ;	
	if( ((size_t)(tmp - bpt_1) + size*len) > allocedTheSize ){
	    get_more_mem(size*len) ;
	    return align(bpt, size) ;
	} else {
	    return align(a, size) ;
	}
    } ;
#endif
    // Aligns the pointer with respect of the size to packed next and
    // allocates more memory if nessecary, this when packing an array.
    
    
    //! Allocates more memory for the buffer.
    void get_more_mem(packbuf_type_tag_s typ, int len) ;

    //! Allocates more memory for the buffer \a msize  bytes is
    //! allocated.
    void get_more_mem(const int msize=DEFAULT_INC_SIZE) ;


    //!  The number of items we have received.
    packbuf_size_t received ;
        
public:    
    
    //! Constructor.
    aPackBuf() ;
    
    //! Constructor, will allocate \a size  bytes for the buffer.
    explicit aPackBuf(const packbuf_size_t &size) ;
    
    //! Constructor, will allocate \a size  bytes in the buffer, 
    //! and sets the default increment size to \a defsize .
    aPackBuf(const size_t size, const size_t defsize) ;
    
    //! Destructor.
    ~aPackBuf() ;
    
    //! Copies the buffer and moves the copied running pointer. 
    aPackBuf & operator=(const aPackBuf & a) ;
    
#ifndef PP3ARR
    //! For accesing the byte \a i .
    inline char operator[](const int i){
	return ( bpt_1[i] ) ; }
#endif
    
    //! Prints the whole buffer.
    void print() ;
  
    //! Prints the whole buffer.
    void print(packbuf_size_t) ;

    //! Method for changing the casched packing direction.
    inline void setPackDir(PackType x){
      way = x ;
    }

    //! Method for changing the casched packing direction.
    inline PackType getPackDir(){
      return way ;
    }
  
    template<class T>void packEnum(T & v) {
        static_assert(sizeof(T)==sizeof(int),"This type has not same size as int, therefore packEnum cannot be applied.");
        if(getPackDir()==Down) pk(static_cast<int>(v));
        else {
          int tmp;
          upk(tmp);
          v=static_cast<T>(tmp);
        }
    }
  
    int saveToFile(const char* ) ;
    int saveToFile(const char*, packbuf_size_t) ;

    int readFromFile(const char* ) ;
    
    //! Tell the buffer it will need a certain size.
    void useSize(const packbuf_size_t &) ;
    
    //! Resets the <b>running</b> pointer, reuses the allocated memory.
    inline void reset(){ bpt = bpt_1 ; } ;
    
    //! Obtain the buffer pointer.
    inline bufptr getPtr(){ return bpt_1 ; } ;
    
    //! Set the buffer pointer to \a a .
    void setPtr(bufptr a) ;
    
    //! Returns the number of items between the running pointer
    //! and the start pointer.
    packbuf_size_t Packed() ;
    
    void SetReceived(packbuf_size_t r){
       received = r ;
    }
    
    bool UnPackedAllData();

    //! The number of size that is allocated.
    packbuf_size_t AllocedSize() ;

    //! Deallocates then memory allocated.
    inline void free_mem(){
#ifdef PP3ARR
	free(bpt_1.iptr) ; allocedTheSize.ints    = 0 ;
	free(bpt_1.dptr) ; allocedTheSize.doubles = 0 ;
	free(bpt_1.fptr) ; allocedTheSize.floats = 0 ;
	free(bpt_1.cptr) ; allocedTheSize.chars   = 0 ;
    free(bpt_1.ldptr) ; allocedTheSize.longdoubles = 0;
	bpt_1.iptr = NULL ;
	bpt_1.dptr = NULL ;
	bpt_1.fptr = NULL ;
	bpt_1.cptr = NULL ;	
    bpt_1.ldptr = NULL ;
#else
	free(bpt_1) ; allocedTheSize = 0 ;
	bpt_1 = NULL ;
#endif
    } ;

    /* <msd type=public> 
     Methods for packing data, pk means pack, and upk means
     unpack. The methods are overloaded to handle the basic types:
     int, float, double, longdouble, double3, double33, and Bstring.
    */
	
    /* <msd type=public> 
       The first set of methods are called basic, these are the 
       one that really packs. More fancy types, such as doubleVec, (no floatVec)
       uses these basic pack functions.
    */

#ifdef PP3ARR

    //! Packs an integer.
    inline void pk(const int &x){ 
#ifdef TALK_PRIVPACK
        cerr << "PP:packing int=" << x << endl ;
#endif
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        Bstring tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
        tmp = Bstring("PP: packing int=") + ToStr(x);
        write_to_thread_log(tmp.c_str());
#endif
        bpt = alignoc(bpt, INT_TYPE) ;
        *bpt.iptr = x ;
	bpt.iptr ++ ;
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
#endif
    }
    
    //! Unpacks an integer.
    inline void upk(int &x){ 
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.ints>0);
#endif      
	x = *(bpt.iptr) ;
	bpt.iptr++ ;
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking int=" << x << endl ;
#endif
    }

    //! Packs an integer.
    inline void pk(const unsigned int &x){
#ifdef TALK_PRIVPACK
        cerr << "PP:packing unsigned int=" << x << endl ;
#endif
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        Bstring tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
        tmp = Bstring("PP: packing int=") + ToStr(x);
        write_to_thread_log(tmp.c_str());
#endif
        bpt = alignoc(bpt, INT_TYPE) ;
        *bpt.iptr = (int)x;
    bpt.iptr ++ ;
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
#endif
    }

    //! Unpacks an integer.
    inline void upk(unsigned int &x){
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.ints>0);
#endif
    x = *((unsigned int*)bpt.iptr) ;
    bpt.iptr++ ;
#ifdef TALK_PRIVPACK
    cerr << "PP:unpacking int=" << x << endl ;
#endif
    }

    union LongParts {
        struct Parts_ { int l, h; } Parts;
        long64 theLong;
    };

    //! Packs a long64
    inline void pk(const long64 &x){ 
        LongParts l;
        l.theLong = x;
        pk(l.Parts.l);
        pk(l.Parts.h);
#if defined(TALK_PRIVPACK) || defined(TALK_PRIVPACK_DOUBLE)
        cerr << "PP:packing long64=" << x << endl ;
#endif
    }

    //! Unpacks a long64
    inline void upk(long64 &x){ 
        LongParts l;
        upk(l.Parts.l);
        upk(l.Parts.h);
        x = l.theLong;
#if defined(TALK_PRIVPACK) || defined(TALK_PRIVPACK_DOUBLE)
	cerr << "PP:unpacking long64=" << x << endl ;
#endif
    }

    //! Packs an double.
    inline void pk(const double &x){ 
#ifdef TALK_PRIVPACK
        cerr << "PP:packing double=" << x << endl ;
#endif
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        Bstring tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
        tmp = Bstring("PP: packing double=") + ToStr(x) + Bstring("-");
        write_to_thread_log(tmp.c_str());
#endif  
#ifdef DEBUGFLG_PRIVPACK_FABS
        double __x = x;
        assert(fabs(__x)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
        bpt = alignoc(bpt, DOUBLE_TYPE) ;
	*bpt.dptr = x ;
	bpt.dptr++ ;
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
#endif
    }

    //! Packs an float.
   inline void pk(const float &x){ 
#ifdef TALK_PRIVPACK
	cerr << "PP:packing float=" << x << endl ;
#endif
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        Bstring tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
        tmp = Bstring("PP: packing float=") + ToStr(x);
        write_to_thread_log(tmp.c_str());
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
        bpt = alignoc(bpt, FLOAT_TYPE) ;
	*bpt.fptr = x ;
	bpt.fptr++ ;
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
        tmp = "BF sz= " + ToStr(Packed().bytes());
        write_to_thread_log(tmp.c_str());
#endif
    }




   //! Packs an longdouble.
  inline void pk(const longdouble &x){
#ifdef TALK_PRIVPACK
   cerr << "PP:packing longdouble=" << x << endl ;
#endif
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
       Bstring tmp = "BF sz= " + ToStr(Packed().bytes());
       write_to_thread_log(tmp.c_str());
       tmp = Bstring("PP: packing longdouble=") + ToStr(x);
       write_to_thread_log(tmp.c_str());
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
               assert(fabs(x)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
       bpt = alignoc(bpt, LONG_DOUBLE_TYPE) ;
   *bpt.ldptr = x ;
   bpt.ldptr++ ;
#if defined(USE_THREADED_PACK_IN_MASTER) && defined(TALK_THREADED_PACK_IN_MASTER)
       tmp = "BF sz= " + ToStr(Packed().bytes());
       write_to_thread_log(tmp.c_str());
#endif
   }


    
    //! Unpacks an double.
    inline void upk(double &x){ 
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.doubles>0);
#endif      
	x = *bpt.dptr ;
	bpt.dptr++ ;
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking double=" << x << endl ;
#endif
    }

    //! Unpacks an float.
    inline void upk(float &x){ 
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.floats>0);
#endif      
	x = *bpt.fptr ;
	bpt.fptr++ ;
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking float=" << x << endl ;
#endif
    }

    //! Unpacks a float.
    inline void upk(longdouble &x){
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.longdoubles>0);
#endif
    x = *bpt.ldptr ;
    bpt.ldptr++ ;
#ifdef TALK_PRIVPACK
    cerr << "PP:unpacking longdouble=" << x << endl ;
#endif
    }


    //! Packs three double.
    inline void pk(const double& x1, const double& x2, const double& x3)
    {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing three double=" << x1 << ", " << x2 << ", " << x3 << endl;
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x1)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
                assert(fabs(x2)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
                assert(fabs(x3)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
	bpt = alignoc(bpt, DOUBLE_TYPE, 3) ;
	*bpt.dptr = x1 ; bpt.dptr++ ;
	*bpt.dptr = x2 ; bpt.dptr++ ;
	*bpt.dptr = x3 ; bpt.dptr++ ;
    }

    //! Packs two double.
    inline void pk(const double& x1, const double& x2)
    {
#ifdef TALK_PRIVPACK
        cerr << "PP:packing two double=" << x1 << ", " << x2 << endl;
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x1)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
                assert(fabs(x2)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
        bpt = alignoc(bpt, DOUBLE_TYPE, 2) ;
        *bpt.dptr = x1 ; bpt.dptr++ ;
        *bpt.dptr = x2 ; bpt.dptr++ ;
    }

    //! Packs two float.
    inline void pk(const float& x1, const float& x2)
    {
#ifdef TALK_PRIVPACK
        cerr << "PP:packing two float=" << x1 << ", " << x2  << endl;
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
        assert(fabs(x1)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
        assert(fabs(x2)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);

#endif
        bpt = alignoc(bpt, FLOAT_TYPE, 2) ;
        *bpt.fptr = x1 ; bpt.fptr++ ;
        *bpt.fptr = x2 ; bpt.fptr++ ;

    }


//! Packs three float.  //BZ536F
    inline void pk(const float& x1, const float& x2, const float& x3)
    {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing three float=" << x1 << ", " << x2 << ", " << x3 << endl;
#endif
#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x1)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
                assert(fabs(x2)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
                assert(fabs(x3)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
	bpt = alignoc(bpt, FLOAT_TYPE, 3) ;
	*bpt.fptr = x1 ; bpt.fptr++ ;
	*bpt.fptr = x2 ; bpt.fptr++ ;
	*bpt.fptr = x3 ; bpt.fptr++ ;
    }


    //! Unpacks three double.
    inline void upk(double& x1, double& x2, double& x3)
    {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.doubles>2);
#endif      
	x1 = *bpt.dptr ; bpt.dptr++ ;
	x2 = *bpt.dptr ; bpt.dptr++ ;
	x3 = *bpt.dptr ; bpt.dptr++ ;
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking double3=" << x1 << ", " << x2 << ", " << x3 << endl;
#endif
    }

    //! Unpacks two double.
    inline void upk(double& x1, double& x2)
    {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.doubles>1);
#endif
        x1 = *bpt.dptr ; bpt.dptr++ ;
        x2 = *bpt.dptr ; bpt.dptr++ ;
#ifdef TALK_PRIVPACK
        cerr << "PP:unpacking double2=" << x1 << ", " << x2 << endl;
#endif
    }


    //! Unpacks two float.//BZ536F
      inline void upk(float& x1, float& x2)
      {
  #ifdef PRIVPACK_DBG
        packbuf_size_t a = Packed();
        assert(a.floats>1);
  #endif
      x1 = *bpt.fptr ; bpt.fptr++ ;
      x2 = *bpt.fptr ; bpt.fptr++ ;

  #ifdef TALK_PRIVPACK
      cerr << "PP:unpacking float3=" << x1 << ", " << x2 << endl;
  #endif
      }

  //! Unpacks three float.//BZ536F
    inline void upk(float& x1, float& x2, float& x3)
    {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.floats>2);
#endif      
	x1 = *bpt.fptr ; bpt.fptr++ ;
	x2 = *bpt.fptr ; bpt.fptr++ ;
	x3 = *bpt.fptr ; bpt.fptr++ ;
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking float3=" << x1 << ", " << x2 << ", " << x3 << endl;
#endif
    }





    //! Packs an Bstring.    
    inline void pk(const Bstring &x){
#ifdef TALK_PRIVPACK
	cerr << "PP:packing Bstring=" << x << endl ;
#endif
	int N =  static_cast<int> ( x.length() ); 
	int i ; 
	bpt = alignoc(bpt, CHAR_TYPE, N+1) ;
	for(i=0; i<N; i++){
		
		//*bpt.cptr=x[i] ;
		*bpt.cptr=x.at(i) ;
	    bpt.cptr++ ;
	}	
	*bpt.cptr = '\0' ;
	bpt.cptr++ ;
    }
    
    
    //! Unpacks an Bstring.    
    inline void upk(Bstring &x){
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
#endif
       	x = bpt.cptr ;    
	bpt.cptr += x.length() + 1 ;
#ifdef PRIVPACK_DBG
        assert(a.chars>x.length());
#endif
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking Bstring=" << x << endl ;
#endif
    }
    
    //! Packs an array of integers, the \a len  in not packed. 
    inline void direct_arr_pk(const int *x, const int len) {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing int arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    bpt = alignoc(bpt, INT_TYPE, len) ;	
	    for(int i=0; i<len; i++){
		*bpt.iptr = x[i] ;
		bpt.iptr++ ;
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
	
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif				
    }
    
    //! Unpacks an array of integers, the \a len  in not unpacked. 
    inline void direct_arr_upk(int *x, const int len) {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.ints>len-1);
#endif      
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking int arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    for(int i=0; i<len; i++){
		x[i] = *bpt.iptr ;
		bpt.iptr++ ;
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif
    }

    //! Packs an array of long64, the \a len  in not packed. 
    inline void direct_arr_pk(const long64 *x, const int len) {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing long64 arr, len=" << len << endl;
#endif
        
        direct_arr_pk(reinterpret_cast<const int*>(x), len * 2);
    }

    //! Unpacks an array of long64
    inline void direct_arr_upk(long64 *x, const int len) { 
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking long64 arr, len=" << len << endl;
#endif
        
        direct_arr_upk(reinterpret_cast<int*>(x), len * 2);
    }

    //! Packs an array of doubles, the \a len  in not packed. 
    inline void direct_arr_pk(const double *x, const int len) {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing double arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    bpt = alignoc(bpt, DOUBLE_TYPE, len) ;
	    for(int i=0; i<len; i++){

#ifdef DEBUGFLG_PRIVPACK_FABS
                double __x_tmp=x[i];
                assert(fabs(__x_tmp)<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif

		*bpt.dptr = x[i] ;
		bpt.dptr++ ;
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif
    }


    //! Unpacks an array of doubles, the \a len  in not unpacked. 
    inline void direct_arr_upk(double *x, const int len) {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.doubles>len-1);
#endif      
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking double arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    for(int i=0; i<len; i++){
		x[i] = *bpt.dptr ;
		bpt.dptr++ ;

#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x[i])<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif
    }


    //! Packs an array of floats, the \a len  in not packed. 
  inline void direct_arr_pk(const float *x, const int len) {
#ifdef TALK_PRIVPACK
	cerr << "PP:packing float arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    bpt = alignoc(bpt, FLOAT_TYPE, len) ;
	    for(int i=0; i<len; i++){

#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x[i])<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif

		*bpt.fptr = x[i] ;
		bpt.fptr++ ;
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif
    }


    //! Unpacks an array of floats, the \a len  in not unpacked. 
    inline void direct_arr_upk(float *x, const int len) {
#ifdef PRIVPACK_DBG
	packbuf_size_t a = Packed();
	assert(a.floats>len-1);
#endif
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking float arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    for(int i=0; i<len; i++){

#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x[i])<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif

		x[i] = *bpt.fptr ;
		bpt.fptr++ ;
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif

	
    }






    //! Packs an array of longdoubles, the \a len  in not packed.
  inline void direct_arr_pk(const longdouble *x, const int len) {
#ifdef TALK_PRIVPACK
    cerr << "PP:packing longdouble arr, len=" << len << ", vals=" ;
#endif
    if(len>0){
        bpt = alignoc(bpt, LONG_DOUBLE_TYPE, len) ;
        for(int i=0; i<len; i++){

#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x[i])<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif

        *bpt.ldptr = x[i] ;
        bpt.ldptr++ ;
#ifdef TALK_PRIVPACK
        cerr << x[i] << " " ;
#endif
        }
    }
#ifdef TALK_PRIVPACK
    cerr << endl ;
#endif
    }


    //! Unpacks an array of longdoubles, the \a len  in not unpacked.
    inline void direct_arr_upk(longdouble *x, const int len) {
#ifdef PRIVPACK_DBG
    packbuf_size_t a = Packed();
    assert(a.longdoubles>len-1);
#endif
#ifdef TALK_PRIVPACK
    cerr << "PP:unpacking longdouble arr, len=" << len << ", vals=" ;
#endif
    if(len>0){
        for(int i=0; i<len; i++){

#ifdef DEBUGFLG_PRIVPACK_FABS
                assert(fabs(x[i])<MAX_ALLOWED_NUMBER_TO_BE_PACKED);
#endif

        x[i] = *bpt.ldptr ;
        bpt.ldptr++ ;
#ifdef TALK_PRIVPACK
        cerr << x[i] << " " ;
#endif
        }
    }
#ifdef TALK_PRIVPACK
    cerr << endl ;
#endif


    }












    //! Packs an array of chars, the \a len  in not packed. 
    inline void direct_arr_pk(const char *x, const int len) {
#ifdef PRINT_DIRECT_ARR_PK_BACKTRACE


            cerr << "stack trace:  --- In case no output is present remember to add  -rdynamic to the Makefile. --- " << endl;

            // storage array for stack trace address data
            void* addrlist[63+1];

            // retrieve current stack addresses
            int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

            if (addrlen == 0) {
                printf("  <empty, possibly corrupt>\n");
                return;
            }

            // resolve addresses into strings containing "filename(function+address)",
            // this array must be free()-ed
            char** symbollist = backtrace_symbols(addrlist, addrlen);

            // allocate string which will be filled with the demangled function name
            size_t funcnamesize = 256;
            char* funcname = (char*)malloc(funcnamesize);

            // iterate over the returned symbol lines. skip the first, it is the
            // address of this function.
            for (int i = 1; i < addrlen; i++)
            {
                char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

                // find parentheses and +address offset surrounding the mangled name:
                // ./module(function+0x15c) [0x8048a6d]
                for (char *p = symbollist[i]; *p; ++p)
                {
                    if (*p == '(')
                        begin_name = p;
                    else if (*p == '+')
                        begin_offset = p;
                    else if (*p == ')' && begin_offset) {
                        end_offset = p;
                        break;
                    }
                }

                if (begin_name && begin_offset && end_offset
                    && begin_name < begin_offset)
                {
                    *begin_name++ = '\0';
                    *begin_offset++ = '\0';
                    *end_offset = '\0';

                    // mangled name is now in [begin_name, begin_offset) and caller
                    // offset in [begin_offset, end_offset). now apply
                    // __cxa_demangle():

                    int status;
                    char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
                    if (status == 0) {
                        funcname = ret; // use possibly realloc()-ed string
                        cerr << "  " << symbollist[i] << " : " << funcname << "+" << begin_offset << endl;
                    }
                    else {
                        // demangling failed. Output function name as a C function with
                        // no arguments.
                        cerr << "  " << symbollist[i] << " : " << begin_name << "()+" << begin_offset << endl;
                    }
                }
                else
                {
                    // couldn't parse the line? print the whole line.
                    cerr << "  " << symbollist[i] << endl;
                }
            }

            free(funcname);
            free(symbollist);
#endif
#ifdef TALK_PRIVPACK
	cerr << "PP:packing char arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    bpt = alignoc(bpt, CHAR_TYPE, len) ;	
	    for(int i=0; i<len; i++){
		*bpt.cptr = x[i] ;
		bpt.cptr++;		
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif

    }
    
//! Unpacks an array of chars, the \a len  in not unpacked. 
inline void direct_arr_upk(char *x, const int len) {
#ifdef PRIVPACK_DBG
      packbuf_size_t a = Packed();
      assert(a.chars>len-1);
#endif      
#ifdef TALK_PRIVPACK
	cerr << "PP:unpacking char arr, len=" << len << ", vals=" ;
#endif
	if(len>0){
	    for(int i=0; i<len; i++){
		x[i] = *bpt.cptr ;
		bpt.cptr++ ;		
#ifdef TALK_PRIVPACK
		cerr << x[i] << " " ;
#endif				
	    }
	}
#ifdef TALK_PRIVPACK
	cerr << endl ;
#endif
    }


    
#else    

    //! Packs an integer.
    inline void pk(const int &x){ 
	bpt = alignoc(bpt, sizeof(int)) ;	
	*(int *)bpt = x ;
	bpt += sizeof(int) ;
    } ;

    //! Unpacks an integer.
    inline void upk(int &x){ 
	bpt = align(bpt, sizeof(int)) ;
	x = *(int *)bpt ;
	bpt += sizeof(int) ;
    } ;

    //! Packs a double.    
    inline void pk(const double &x){ 
	bpt = alignoc(bpt, sizeof(double)) ;
	*(double *)bpt = x ;
	bpt += sizeof(double) ;
    } ;

    //! Unpacks a double.    
    inline void upk(double &x){ 
	bpt = align(bpt, sizeof(double)) ;
	x = *(double *)bpt ;
	bpt += sizeof(double) ;
    } ;
    

    //! Packs a float.    
   inline void pk(const float &x){ 
	bpt = alignoc(bpt, sizeof(float)) ;
	*(float *)bpt = x ;
	bpt += sizeof(float) ;
    } ;

    //! Unpacks a float.    
    inline void upk(float &x){ 
	bpt = align(bpt, sizeof(float)) ;
	x = *(float *)bpt ;
	bpt += sizeof(float) ;
    } ;
    

    //! Packs a longdouble.
   inline void pk(const longdouble &x){
    bpt = alignoc(bpt, sizeof(longdouble)) ;
    *(longdouble *)bpt = x ;
    bpt += sizeof(longdouble) ;
    } ;

    //! Unpacks a longdouble.
    inline void upk(longdouble &x){
    bpt = align(bpt, sizeof(longdouble)) ;
    x = *(longdouble *)bpt ;
    bpt += sizeof(longdouble) ;
    } ;


    //! Packs three double.
    inline void pk(const double& x1, const double& x2, const double& x3)
    {
	bpt = alignoclen(bpt, sizeof(double), 3) ;
	*(double *)bpt = x1; bpt += sizeof(double) ; 
	*(double *)bpt = x2; bpt += sizeof(double) ; 
	*(double *)bpt = x3; bpt += sizeof(double) ; 
    } ;


    //! Unpacks three double.
    inline void upk(double& x1, double& x2, double& x3)
    {
	bpt = align(bpt, sizeof(double)) ; 
	x1 = *(double *)bpt ; bpt += sizeof(double) ; 
	x2 = *(double *)bpt ; bpt += sizeof(double) ; 
	x3 = *(double *)bpt ; bpt += sizeof(double) ; 
    } ;

    //! Packs two double.
    inline void pk(const double& x1, const double& x2)
    {
        bpt = alignoclen(bpt, sizeof(double), 2) ;
        *(double *)bpt = x1; bpt += sizeof(double) ;
        *(double *)bpt = x2; bpt += sizeof(double) ;
    } ;


    //! Unpacks two double.
    inline void upk(double& x1, double& x2)
    {
        bpt = align(bpt, sizeof(double)) ;
        x1 = *(double *)bpt ; bpt += sizeof(double) ;
        x2 = *(double *)bpt ; bpt += sizeof(double) ;
    } ;


    //! Packs an Bstring.    
    inline void pk(const Bstring &x){

	int N = x.length() ; 
	int i ;    
	
	if( ((int)(bpt - bpt_1) + N + 1) > (int)allocedTheSize )
	    get_more_mem(N+1) ;
	
	for(i=0; i<N; i++){
	    *bpt=x(i) ;
	    (void)(bpt)++ ;
	}
	
	*bpt= '\0' ;
	(void)(bpt)++ ;
    } ;
    

    //! Unpacks an Bstring.    
    inline void upk(Bstring &x){
	x = bpt ;    
	bpt += x.length() + 1 ;
    } ;
    
    //! Packs an array of integers, the \a len  in not packed. 
    inline void direct_arr_pk(const int *x, const int len) {
	if(len>0){
	    bpt = alignoclen(bpt, sizeof(int), len) ;	
	    for(int i=0; i<len; i++){
		*(int *)bpt = x[i] ;
		bpt += sizeof(int) ;		
	    }
	}
    } ;

    //! Unpacks an array of integers, the \a len  in not unpacked. 
    inline void direct_arr_upk(int *x, const int len) {
	if(len>0){
	    bpt = align(bpt, sizeof(int)) ; 
	    for(int i=0; i<len; i++){
		x[i] = *(int *)bpt ;
		bpt += sizeof(int) ;		
	    }
	}
    } ;
    
    //! Packs an array of doubles, the \a len  in not packed. 
    inline void direct_arr_pk(const double *x, const int len) {
	if(len>0){
	    bpt = alignoclen(bpt, sizeof(double), len) ;
	    for(int i=0; i<len; i++){
		*(double *)bpt = x[i] ;
		bpt += sizeof(double) ; 
	    }
	}
    } ;
    
    //! Unpacks an array of doubles, the \a len  in not unpacked. 
    inline void direct_arr_upk(double *x, const int len) {
	if(len>0){
	    bpt = align(bpt, sizeof(double)) ; 
	    for(int i=0; i<len; i++){
		x[i] = *(double *)bpt ;
		bpt += sizeof(double) ; 
	    }
	}
    } ;

    //! Packs an array of chars, the \a len  in not packed. 
    inline void direct_arr_pk(const char *x, const int len) {
	if(len>0){
	    bpt = alignoclen(bpt, sizeof(char), len) ;	
	    for(int i=0; i<len; i++){
		*bpt = x[i] ;
		bpt++;		
	    }
	}
    } ;
    
    //! Unpacks an array of chars, the \a len  in not unpacked. 
    inline void direct_arr_upk(char *x, const int len) {
	if(len>0){
	    bpt = align(bpt, sizeof(char)) ; 
	    for(int i=0; i<len; i++){
		x[i] = *bpt;
		bpt++;		
	    }
	}
    } ;




    //! Packs an array of floats, the \a len  in not packed. 
    inline void direct_arr_pk(const float *x, const int len) {
	if(len>0){
	    bpt = alignoclen(bpt, sizeof(float), len) ;
	    for(int i=0; i<len; i++){
		*(float *)bpt = x[i] ;
		bpt += sizeof(float) ; 
	    }
	}
    } ;
    
    //! Unpacks an array of floats, the \a len  in not unpacked. 
    inline void direct_arr_upk(float *x, const int len) {
	if(len>0){
	    bpt = align(bpt, sizeof(float)) ; 
	    for(int i=0; i<len; i++){
		x[i] = *(float *)bpt ;
		bpt += sizeof(float) ; 
	    }
	}
    } ;



    //! Packs an array of longdoubles, the \a len  in not packed.
    inline void direct_arr_pk(const longdouble *x, const int len) {
    if(len>0){
        bpt = alignoclen(bpt, sizeof(longdouble), len) ;
        for(int i=0; i<len; i++){
        *(longdouble *)bpt = x[i] ;
        bpt += sizeof(longdouble) ;
        }
    }
    } ;

    //! Unpacks an array of longdoubles, the \a len  in not unpacked.
    inline void direct_arr_upk(longdouble *x, const int len) {
    if(len>0){
        bpt = align(bpt, sizeof(longdouble)) ;
        for(int i=0; i<len; i++){
        x[i] = *(longdouble *)bpt ;
        bpt += sizeof(longdouble) ;
        }
    }
    } ;

    //! Packs an array of chars, the \a len  in not packed. 
    inline void direct_arr_pk(const char *x, const int len) {
	if(len>0){
	    bpt = alignoclen(bpt, sizeof(char), len) ;	
	    for(int i=0; i<len; i++){
		*bpt = x[i] ;
		bpt++;		
	    }
	}
    } ;
    
    //! Unpacks an array of chars, the \a len  in not unpacked. 
    inline void direct_arr_upk(char *x, const int len) {
	if(len>0){
	    bpt = align(bpt, sizeof(char)) ; 
	    for(int i=0; i<len; i++){
		x[i] = *bpt;
		bpt++;		
	    }
	}
    } ;


#endif
    
    /* <msd type=public> 
       These are the fancy-overloading packing functions that uses
       the basic packing functions.
    */
    
    //! Packs a double3.
    inline void pk(const double3& x){ 
	pk(x.XX, x.YY, x.ZZ) ;
    } ;

    //! Unpacks a double3.
    inline void upk(double3& x){    
	upk(x.XX, x.YY, x.ZZ) ;
    } ;

    //! Packs a double2.
    inline void pk(const double2& x){
        pk(x.XX, x.YY) ;
    }

    //! Unpacks a double2.
    inline void upk(double2& x){
        upk(x.XX, x.YY) ;
    }

    //! Packs a float2.
    inline void pk(const float2& x){
    pk(x.XX, x.YY) ;
    } ;

    //! Packs a float3.
    inline void pk(const float3& x){ 
	pk(x.XX, x.YY, x.ZZ) ;
    } ;
    
    //! Unpacks a float2.
    inline void upk(float2& x){
    upk(x.XX, x.YY) ;
    } ;

    //! Unpacks a float3.
    inline void upk(float3& x){    
	upk(x.XX, x.YY, x.ZZ) ;
    } ;
    


    //! Packs a double33.
    inline void pk(const double33 &x){ 
	direct_arr_pk(x.x, 9) ;
    } ;
    
    //! Unpacks a double33.
    inline void upk(double33 &x){    
	direct_arr_upk(x.x, 9) ;
    } ;
    
    //! Packs a double33s.
    inline void pk(const double33s &x){ 
	pk(x.x11, x.x22, x.x33) ;
	pk(x.x12, x.x23, x.x13) ;
    };

   
    
    //! Unpacks a double33s.
    inline void upk(double33s &x){    
	upk(x.x11, x.x22, x.x33) ;
	upk(x.x12, x.x23, x.x13) ;
    };

    //! Packs a float33s.  BZ536F
    inline void pk(const float33s &x){ 
	pk(x.x11, x.x22, x.x33) ;
	pk(x.x12, x.x23, x.x13) ;
    };

    //! Unpacks a float33s.  BZ536F
    inline void upk(float33s &x){    
	upk(x.x11, x.x22, x.x33) ;
	upk(x.x12, x.x23, x.x13) ;
    };


    //! Packs a  vector<double>.
    void pk(const std::vector<double> &x);

    //! Unpacks a  vector<double>.
    void upk(std::vector<double> &x);

    //! Packs a vector of vector<double>.
    void pk(const std::vector<std::vector<double> > &x);

    //! Unpacks a vector of vector<double>.
    void upk(std::vector<std::vector<double> > &x);


    //! Packs a  vector<char>.
    void pk(const std::vector<char> &x);

    //! Unpacks a  vector<char>.
    void upk(std::vector<char> &x);

    //! Packs a vector of vector<char>.
    void pk(const std::vector<std::vector<char> > &x);

    //! Unpacks a vector of vector<char>.
    void upk(std::vector<std::vector<char> > &x);


    /* <msd type=public> 
     Operators for packing and unpacking. */

#if 1
    //! Packs an integer.
    inline void operator<<(const int &x){
	pk(x) ; } ;

    //! Packs an unsigned integer.
    inline void operator<<(const unsigned int &x){
    pk(x) ; } ;

    //! Packs a double.
    inline void operator<<(const double &x){
	pk(x) ; } ;

    //! Packs a float.
     inline void operator<<(const float &x){
	pk(x) ; } ;
    
     //! Packs a longdouble.
      inline void operator<<(const longdouble &x){
     pk(x) ; } ;

    //! Packs a double3.
    inline void operator<<(const double3 &x){
    	pk(x) ; } ;
    
    //! Packs a double33.
    inline void operator<<(const double33 &x){
    	pk(x) ; } ;

    //! Packs a double33s.
    inline void operator<<(const double33s &x){
    	pk(x) ; } ;
    
    //! Packs an Bstring.
    inline void operator<<(const Bstring &x){
    	pk(x) ; } ;
    
    //! Unpacks an integer.
    inline void operator>>(int &x){
    upk(x) ; } ;

    //! Unpacks an unsigned integer.
    inline void operator>>(unsigned int &x){
    upk(x) ; } ;

    //! Unpacks a double.
    inline void operator>>(double &x){
	upk(x) ; } ;
    
    //! Unpacks a float.
    inline void operator>>(float &x){
	upk(x) ; } ;

    //! Unpacks a longdouble.
    inline void operator>>(longdouble &x){
    upk(x) ; } ;
    
    //! Unpacks a double3.
    inline void operator>>(double3 &x){
    	upk(x) ; } ;
    
    //! Unpacks a double33.
    inline void operator>>(double33 &x){
    	upk(x) ; } ;
    
    //! Unpacks a double33s. //BZ445-1
    inline void operator>>(double33s &x){
    	upk(x) ; } ;
    
    //! Unpacks an Bstring.
    inline void operator>>(Bstring &x){
    	upk(x) ; } ;
#endif


#ifdef SKF_PACK    
    //! Packs or unpacks an array of doubles, given a direction \a p .
    //! The size \a len  in not packed or unpacked.
    inline void pack(const PackType p, double *x, const int len){
	if(p==Down)
	    direct_arr_pk(x, len) ;
	else
	    direct_arr_upk(x, len) ;
    } ;

    //! Packs or unpacks an array of float, given a direction \a p .
    //! The size \a len  in not packed or unpacked.
   inline void pack(const PackType p, float *x, const int len){
	if(p==Down)
	    direct_arr_pk(x, len) ;
	else
	    direct_arr_upk(x, len) ;
    } ;

   //! Packs or unpacks an array of longdouble, given a direction \a p .
   //! The size \a len  in not packed or unpacked.
  inline void pack(const PackType p, longdouble *x, const int len){
   if(p==Down)
       direct_arr_pk(x, len) ;
   else
       direct_arr_upk(x, len) ;
   } ;


    //! Packs or unpacks an array of integers, given a direction \a  p  .
    //! The size \a len  in not packed or unpacked.
    inline void pack(const PackType p, int *x, const int len){
	if(p==Down)
	    direct_arr_pk(x, len) ;
	else
	    direct_arr_upk(x, len) ;
    } ;


    //! Packs or unpacks an array of chars, given a direction \a  p  .
    //! The size \a len  in not packed or unpacked.
    inline void pack(const PackType p, char *x, const int len){
	if(p==Down)
	    direct_arr_pk(x, len) ;
	else
	    direct_arr_upk(x, len) ;
    } ;


    //! Packs or unpacks an doubleN, given a direction \a p .
    //! The size \a len  is not packed or unpacked.
    inline void pack(const PackType p, doubleN& x){
        if(p==Down)
            direct_arr_pk(&x.data[0], x.len) ;
        else
            direct_arr_upk(&x.data[0], x.len) ;
    } ;

    //! Packs or unpacks an intN, given a direction \a  p  .
    //! The size \a len  is not packed or unpacked.
    inline void pack(const PackType p, intN& x) {
        if(p==Down)
            direct_arr_pk(&x.data[0], x.len) ;
        else
            direct_arr_upk(&x.data[0], x.len) ;
    } ;
    
    //! (Un)packs an integer.
    inline void pack(PackType p, int &x){
	if(p==Down)
	    pk(x) ;
    else upk(x) ; } ;

    //! (Un)packs an unsigned integer.
    inline void pack(PackType p, unsigned int &x){
    if(p==Down)
        pk(x) ;
    else upk(x) ; } ;

    

    //! (Un)packs a double.
    inline void pack(PackType p, double &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;


    //! (Un)packs a longdouble.
    inline void pack(PackType p, longdouble &x){
    if(p==Down)
        pk(x) ;
    else upk(x) ; } ;

    //! (Un)packs a float.
    inline void pack(PackType p, float &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;
    
    //! (Un)packs a double3.
    inline void pack(PackType p, double3 &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;
    
    //! (Un)packs a float3.
    inline void pack(PackType p, float3 &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;

    //! (Un)packs a double2.
    inline void pack(PackType p, double2 &x){
        if(p==Down)
            pk(x) ;
        else upk(x) ; }

    //! (Un)packs a float2.
    inline void pack(PackType p, float2 &x){
    if(p==Down)
        pk(x) ;
    else upk(x) ; } ;

    //! (Un)packs a double33.
    inline void pack(PackType p, double33 &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;

    //! (Un)packs a double33s.
    //! //BZ445-1
    inline void pack(PackType p, double33s &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;

  //! (Un)packs a float33s.
    inline void pack(PackType p, float33s &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;
  


    //! (Un)packs a doubleN3.
    //! The size \a len  is not packed or unpacked.
    inline void pack(PackType p, doubleN3& x){
      // This implementation will change when lightmat change and
      // we can access the complete data area as an array.
      if(p==Down) {
  	for(int i=0; i<x.len; i++)
  	  pk(x.data[i]) ;
  	
      } else {
  	for(int i=0; i<x.len; i++)
  	  upk(x.data[i]) ;
      }
    };

    //! (Un)packs a doubleN33.
    //! The size \a len  is not packed or unpacked.
    inline void pack(PackType p, doubleN33& x){
        // This implementation will change when lightmat change and
        // we can access the complete data area as an array.
        if(p==Down) {
            for(int i=0; i<x.len; i++)
                pk(x.data[i]) ;

        } else {
            for(int i=0; i<x.len; i++)
                upk(x.data[i]) ;
        }
    };
   
    //! (Un)packs a doubleN33s.
    //! The size \a len  is not packed or unpacked.
    //! //BZ445-1
    inline void pack(PackType p, doubleN33s& x){
        // This implementation will change when lightmat change and
        // we can access the complete data area as an array.
        if(p==Down) {
            for(int i=0; i<x.len; i++)
                pk(x.data[i]) ;

        } else {
            for(int i=0; i<x.len; i++)
                upk(x.data[i]) ;
        }
    };



    //! (Un)packs an Bstring.
    inline void pack(PackType p, Bstring &x){
	if(p==Down)
	    pk(x) ;
	else upk(x) ; } ;



    inline void pack(double *x, const int len){
        assert(way != NoWay) ;
    if(way==Down) direct_arr_pk(x, len) ;
    else direct_arr_upk(x, len) ;
    } ;

    inline void pack(float *x, const int len){
        assert(way != NoWay) ;
	if(way==Down) direct_arr_pk(x, len) ;
	else direct_arr_upk(x, len) ; 
    } ;

    inline void pack(longdouble *x, const int len){
        assert(way != NoWay) ;
    if(way==Down) direct_arr_pk(x, len) ;
    else direct_arr_upk(x, len) ;
    } ;

    inline void pack(char *x, const int len){
        assert(way != NoWay) ;
	if(way==Down) direct_arr_pk(x, len) ;
	else direct_arr_upk(x, len) ;
    } ;

    inline void pack(int *x, const int len){
        assert(way != NoWay) ;
	if(way==Down) direct_arr_pk(x, len) ;
	else direct_arr_upk(x, len) ;
    } ;

    inline void pack(doubleN& x){
        assert(way != NoWay) ;
        if(way==Down) direct_arr_pk(&x.data[0], x.len) ;
        else direct_arr_upk(&x.data[0], x.len) ;
    } ;

    inline void pack(intN& x){
        assert(way != NoWay) ;
        if(way==Down) direct_arr_pk(&x.data[0], x.len) ;
        else direct_arr_upk(&x.data[0], x.len) ;
    } ;
    
    inline void pack(int &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;

    inline void pack(unsigned int &x){
        assert(way != NoWay) ;
    if(way==Down) pk(x) ;
    else upk(x) ;
    } ;

    inline void pack(long64 &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;


    //! (Un)packs a bool.
    inline void pack( bool &x){
        assert(way != NoWay) ;
        int y;
        if(way==Down) {
            y=x;
            pk(y) ;
        }
        else { 
            upk(y) ;
            x=(y!=0);
        }
    } ;



    inline void pack(double &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;
 

    inline void pack(longdouble &x){
        assert(way != NoWay) ;
    if(way==Down) pk(x) ;
    else upk(x) ;
    } ;

    inline void pack(float &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;
    
    inline void pack(double3 &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;

    inline void pack(double2 &x){
        assert(way != NoWay) ;
        if(way==Down) pk(x) ;
        else upk(x) ;
    }

    inline void pack(float3 &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;
    
    inline void pack(float2 &x){
        assert(way != NoWay) ;
    if(way==Down) pk(x) ;
    else upk(x) ;
    } ;

    inline void pack(double33 &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;

    inline void pack(double33s &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;
  
    inline void pack(float33s &x){ // BZ536F
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;
  
  


    inline void pack(doubleN3& x){
      assert(way != NoWay) ;
      if(way==Down) {
  	for(int i=0; i<x.len; i++)
  	  pk(x.data[i]) ;
  	
      } else {
  	for(int i=0; i<x.len; i++)
  	  upk(x.data[i]) ;
      }
    };

    inline void pack(doubleN33& x){
        assert(way != NoWay) ;
        if(way==Down) {
            for(int i=0; i<x.len; i++)
                pk(x.data[i]) ;

        } else {
            for(int i=0; i<x.len; i++)
                upk(x.data[i]) ;
        }
    };

    inline void pack(doubleN33s& x){
        assert(way != NoWay) ;
        if(way==Down) {
            for(int i=0; i<x.len; i++)
                pk(x.data[i]) ;

        } else {
            for(int i=0; i<x.len; i++)
                upk(x.data[i]) ;
        }
    };

    inline void pack(Bstring &x){
        assert(way != NoWay) ;
	if(way==Down) pk(x) ;
	else upk(x) ; 
    } ;


    //! Pack a Newmat matrix.
    void pack(GeneralMatrix& x) {

#ifdef DEBUGFLG
        int nrows   = x.Nrows();
        int ncols   = x.Ncols();
        int tag     = x.Tag();
        int storage = x.Storage();

        pack(ncols);
        pack(tag);
        pack(storage);
        pack(nrows);

        // It is not a bug to pack an empty array (with unallocated memory). The pack functions check for the size/length.
        // This case can still be tested for, but then under other flags than DEBUGFLG.
        // DF 2015-04-09
        if (storage!=0 && way==Down) assert(x.Store());
#endif
        // Pack functions are not allowed to re-size. The target must have correct dimensions. DF 2014-11-21

#ifdef DEBUGFLG	
        assert(nrows==x.Nrows());
        assert(ncols==x.Ncols());
        assert(tag==x.Tag());
        assert(storage==x.Storage());
#endif

        pack(x.Store(), x.Storage());

#ifdef DEBUGFLG
        assert(x.Nrows()*x.Ncols()>=x.Storage());
        if (storage!=0) assert(x.Store());
#endif

    }


    //! Pack a Newmat matrix.
    void pack(Matrix& x) {

#ifdef DEBUGFLG
        int nrows   = x.Nrows();
        int ncols   = x.Ncols();
        int tag     = x.Tag();
        int storage = x.Storage();

        pack(nrows);
        pack(ncols);
        pack(tag);
        pack(storage);

        // It is not a bug to pack an empty array (with unallocated memory). The pack functions check for the size/length.
        // This case can still be tested for, but then under other flags than DEBUGFLG.
        // DF 2015-04-09
        if (storage!=0 && way==Down) assert(x.Store());
#endif

        // Pack functions are not allowed to re-size. The target must have correct dimensions. DF 2014-11-21

#ifdef DEBUGFLG
        assert(nrows==x.Nrows());
        assert(ncols==x.Ncols());
        assert(tag==x.Tag());
        assert(storage==x.Storage());
#endif

        pack(x.Store(), x.Storage());


#ifdef DEBUGFLG
        assert(x.Nrows()*x.Ncols()>=x.Storage());
        if (storage!=0) assert(x.Store());
#endif

    }


    //! Pack vector of doubles, together with its length
    void pack(std::vector< double >  & x){
        assert(way != NoWay) ;
        if(way==Down) pk(x) ;
        else upk(x) ;
    };

    //! Pack vector of vector of doubles, together with its length
    void pack(std::vector<std::vector< double > > & x){
        assert(way != NoWay) ;
        if(way==Down) pk(x) ;
        else upk(x) ;
    };

    //! Pack vector of charss, together with its length
    void pack(std::vector< char >  & x){
        assert(way != NoWay) ;
        if(way==Down) pk(x) ;
        else upk(x) ;
    };

    //! Pack vector of vector of charss, together with its length
    void pack(std::vector< std::vector< char > > & x){
        assert(way != NoWay) ;
        if(way==Down) pk(x) ;
        else upk(x) ;
    };

#ifndef PP3ARR
    void checkPoint(PackType p){
	int tst ;
	if(p == Down) {
	    bufptr a = bpt ;
	    tst = (int)(align(a, sizeof(int)) - bpt_1) + (int) sizeof(int) ;
	    pk(tst) ;
	} else {
	    upk(tst) ;	    
	    assert(tst == Packed()) ;
	}	
    }
#endif
    // For checking consistency in the packing and unpacking. The
    // sending side packs a number (i.e. the number of bytes it has packed) 
    // On the reciving side then we ought to have the same number of bytes
    // when unpacking the buffer.
    
    
#endif    

};    


inline std::ostream& operator<<(std::ostream& o, aPackBuf *v){
  switch(v->getPackDir()){
  case Up:
    o << " Up " ;
    break;
  case Down:
    o << " Down " ;
    break;
  case NoWay:
    o << " NoWay " ;
    break;
  default:assert(false);
  }    
  return o ;
}
 
#endif  // PrivPack_h
