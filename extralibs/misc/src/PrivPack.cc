#include "PrivPack.h"
#include <cstdlib>
 
#include <iostream>

#include <cassert>
#include <cstdio>
#include "portability.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



/*
 *

Revision 1.2  1996/03/11 16:20:14  patno
Fixed malloc delete mix.

Revision 1.1.1.1  1996/02/08 16:14:39  patno
Imported misc

Revision 1.7  1995/11/22 15:34:38  patno
Fixed the RCS headers

Revision 1.6  1995/11/22 15:34:01  patno
Fixed the RCS headers

 *
 */


aPackBuf::aPackBuf(){

#ifdef PP3ARR
    bpt.iptr   = 0 ;
    bpt.dptr   = 0 ;
    bpt.cptr   = 0 ;
    bpt.fptr   = 0 ;
    bpt_1.iptr = 0 ;    
    bpt_1.dptr = 0 ;    
    bpt_1.cptr = 0 ;    
    bpt_1.fptr = 0 ;    
    allocedTheSize.ints = 0 ;
    allocedTheSize.doubles = 0 ;
    allocedTheSize.chars = 0 ;
    allocedTheSize.floats = 0 ;
    default_inc.ints = DEFAULT_INC_SIZE ;    
    default_inc.doubles = DEFAULT_INC_SIZE ;    
    default_inc.chars = DEFAULT_INC_SIZE ;    
    default_inc.floats = DEFAULT_INC_SIZE ;    
    way = NoWay ;
#else
    bpt   = 0 ;
    bpt_1 = 0 ;    
    allocedTheSize = 0 ;
    default_inc = DEFAULT_INC_SIZE ;
#endif
}

aPackBuf::aPackBuf(const size_t size, const size_t defsize = DEFAULT_INC_SIZE){

        
#ifdef PP3ARR
    
    packbuf_size_t s ;
    s.ints = size/sizeof(int) ;
    s.doubles = size/sizeof(double) ;
    s.chars   = size/sizeof(char) ;
    s.floats   = size/sizeof(float) ;
    
    allocedTheSize.ints = 0 ;
    allocedTheSize.doubles = 0 ;
    allocedTheSize.chars = 0 ;
    allocedTheSize.floats = 0 ;
    way = NoWay ;

    useSize(s) ;
    
    default_inc.ints = defsize/sizeof(int) ;
    default_inc.doubles = defsize/sizeof(double) ;
    default_inc.chars   = defsize/sizeof(char) ;
    default_inc.floats   = defsize/sizeof(float) ;
    
    if(default_inc.ints<=0)
	default_inc.ints = 128 ;
    if(default_inc.doubles<=0)
	default_inc.doubles = 128 ;
    if(default_inc.floats<=0)
	default_inc.floats = 128 ;
    if(default_inc.chars<=0)
	default_inc.chars = 128 ;
    
#else
    bpt_1   = (bufptr) malloc(size*sizeof(buftyp)) ;
    assert(bpt_1 != 0) ;
    for(int i=0; i<(int)size; i++) bpt_1[i] = 0 ;
    
    bpt = bpt_1 ;
    allocedTheSize = size;
    
    if(defsize < (sizeof(double) - sizeof(int))){
	cerr << "ERROR in Pack Buffert, default size inc."
	     << " may not be smaller than " 
	     << (sizeof(double) - sizeof(int))
		    << endl ;
	assert(0) ;
    }        
    default_inc = defsize ;
#endif

}


aPackBuf::~aPackBuf(){
#if 0

#ifdef PP3ARR
    if(bpt_1.iptr) free(bpt_1.iptr) ;
    if(bpt_1.dptr) free(bpt_1.dptr) ;
    if(bpt_1.cptr) free(bpt_1.cptr) ;
    if(bpt_1.fptr) free(bpt_1.fptr) ;
#else
    free(bpt_1) ;
#endif

#endif
}



#ifdef PP3ARR
aPackBuf&
aPackBuf::operator=(const aPackBuf & ){
    cerr << "This is not implemented yet.2" << endl ;
    assert(0) ;
    return *this ;
}
#else
aPackBuf&
aPackBuf::operator=(const aPackBuf & a){
    int i ;
    if(this != &a){
	if(allocedTheSize != a.allocedTheSize){
	    allocedTheSize = a.allocedTheSize ;
	    free((void *)bpt_1) ;
	    bpt_1 = (bufptr) malloc(allocedTheSize*sizeof(buftyp)) ;
	    bpt = bpt_1 ;	    
	}
	int tmp = a.bpt - a.bpt_1 ;
	
	for(i=0; i<tmp; i++)
	    bpt_1[i] = a.bpt_1[i] ;	
	bpt = bpt_1 + tmp ; // move the assigned pointer
	
    }
    return *this ;
}
#endif


void aPackBuf::print(){
#ifdef PP3ARR
    int i ;
    for(i=0; i<(bpt.iptr-bpt_1.iptr); i++)
	cerr << bpt_1.iptr[i] << " " ;
    cerr << endl ;

    for(i=0; i<(bpt.dptr-bpt_1.dptr); i++)
	cerr << bpt_1.dptr[i] << " " ;
    cerr << endl ;

    for(i=0; i<(bpt.cptr-bpt_1.cptr); i++)
	cerr << bpt_1.cptr[i] << " " ;
    cerr << endl ;    

    for(i=0; i<(bpt.fptr-bpt_1.fptr); i++)
	cerr << bpt_1.fptr[i] << " " ;
    cerr << endl ;
#else
    for(int i=0; i<(int)(bpt - bpt_1); i++) cerr << bpt_1[i] ;
    cerr << endl ;
#endif
}


void aPackBuf::print(packbuf_size_t x){
#ifdef PP3ARR
    int i ;
    for(i=0; i<x.ints; i++)
	cerr << bpt_1.iptr[i] << " " ;
    cerr << endl ;

    for(i=0; i<x.doubles; i++)
	cerr << bpt_1.dptr[i] << " " ;
    cerr << endl ;

    for(i=0; i<x.chars; i++)
	cerr << bpt_1.cptr[i] << " " ;
    cerr << endl ;    

    for(i=0; i<x.floats; i++)
	cerr << bpt_1.fptr[i] << " " ;
    cerr << endl ;    
#else
    for(int i=0; i<(int)(bpt - bpt_1); i++) cerr << bpt_1[i] ;
    cerr << endl ;
#endif
}


void aPackBuf::setPtr(bufptr a){
    bpt_1 = a  ;
}

packbuf_size_t aPackBuf::Packed(){
#ifdef PP3ARR
    return ( 
	packbuf_size_s( 
	    (int)(bpt.iptr - bpt_1.iptr), 
	    (int)(bpt.dptr - bpt_1.dptr),
	    (int)(bpt.cptr - bpt_1.cptr),
	    (int)(bpt.fptr - bpt_1.fptr)
	    )
	) ;
#else
    return (size_t)(bpt - bpt_1)  ;
#endif
}

packbuf_size_t aPackBuf::AllocedSize(){
    return ( allocedTheSize ) ;
} 

void aPackBuf::get_more_mem(packbuf_type_tag_s typ, int len){ 
#ifdef PP3ARR
    size_t size ;
    size_t alloced_for_type ;
    int definc ;
    Bstring styp ;
    unsigned int tmp_run ;    
    
    switch(typ){
    case INT_TYPE:
    {
	
	size = sizeof(int);
	alloced_for_type = allocedTheSize.ints ; 
	definc = default_inc.ints ;
	tmp_run = (unsigned int)(bpt.iptr - bpt_1.iptr) ; 	

	if(definc < len) definc = len ;    
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem typ=INT " << ", len= " << len << ", alloced= " << alloced_for_type << ", tmprun= " << tmp_run << endl ;	
#endif
	if(alloced_for_type > (len + tmp_run)) return ;	
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem needs more, allocs= " << (definc + alloced_for_type)*size  << endl ;
#endif	
	bpt_1.iptr =  (int *)realloc((void *)bpt_1.iptr, 
				     (definc + alloced_for_type)*size) ;
	bpt.iptr = bpt_1.iptr + tmp_run ;
	allocedTheSize.ints += definc ; 
	
    }
    break ;
    case DOUBLE_TYPE:
    {
	size = sizeof(double);
	alloced_for_type = allocedTheSize.doubles ; 
	definc = default_inc.doubles ;
	tmp_run =  (unsigned int)(bpt.dptr - bpt_1.dptr) ; 

	if(definc < len) definc = len ;    
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem typ=DOUBLE" << ", len= " << len << ", alloced= " << alloced_for_type << ", tmprun= " << tmp_run << endl ;	
#endif
	if(alloced_for_type > (len + tmp_run)) return ;	
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem needs more, allocs= " << (definc + alloced_for_type)*size  << endl ;
#endif	
	bpt_1.dptr =  (double *)realloc((void *)bpt_1.dptr, 
					(definc + alloced_for_type)*size) ;
	bpt.dptr = bpt_1.dptr + tmp_run ;
	allocedTheSize.doubles += definc ; 


    }
	break ;


    case FLOAT_TYPE:
    {
	size = sizeof(float);
	alloced_for_type = allocedTheSize.floats ; 
	definc = default_inc.floats ;
	tmp_run =  (unsigned int)(bpt.fptr - bpt_1.fptr) ; 

	if(definc < len) definc = len ;    
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem typ=FLOAT" << ", len= " << len << ", alloced= " << alloced_for_type << ", tmprun= " << tmp_run << endl ;	
#endif
	if(alloced_for_type > (len + tmp_run)) return ;	
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem needs more, allocs= " << (definc + alloced_for_type)*size  << endl ;
#endif	
	bpt_1.fptr =  (float *)realloc((void *)bpt_1.fptr, 
					(definc + alloced_for_type)*size) ;
	bpt.fptr = bpt_1.fptr + tmp_run ;
	allocedTheSize.floats += definc ; 


    }
	break ;




    case CHAR_TYPE:
    {
	size = sizeof(char);
	alloced_for_type = allocedTheSize.chars ; 
	tmp_run =  (unsigned int)(bpt.cptr - bpt_1.cptr) ; 
	definc = default_inc.chars ;
	
	if(definc < len) definc = len ;    
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem typ=CHAR" << ", len= " << len << ", alloced= " << alloced_for_type << ", tmprun= " << tmp_run << endl ;	
#endif
	if(alloced_for_type > (len + tmp_run)) return ;	
#ifdef TALK_PRIVPACK
	cerr << "PP:get_more_mem needs more, allocs= " << (definc + alloced_for_type)*size  << endl ;
#endif	
	bpt_1.cptr = (char *)realloc((void *)bpt_1.cptr, 
				     (definc + alloced_for_type)*size) ;
	bpt.cptr = bpt_1.cptr + tmp_run ;
	allocedTheSize.chars += definc ; 
	
    }
	break ;
    default:
	assert(0) ;
    }	
    
    return ;



#if 0    
    p_1 =  realloc((void *)p_1, 
		   (definc + alloced_for_type)*size) ;
    
    p = (void *)((unsigned int)p_1 + tmp_run*size) ; 
    
    switch(typ){
    case INT_TYPE:
	allocedTheSize.ints += definc ; 
	bpt.iptr = (int *)p ; bpt_1.iptr = (int *)p_1 ;
	break ;
    case DOUBLE_TYPE:
	allocedTheSize.doubles += definc ; 
	bpt.dptr = (double *)p ; bpt_1.dptr = (double *)p_1 ;
	break ;
    case CHAR_TYPE:
	allocedTheSize.chars += definc ; 
	bpt.cptr = (char *)p ; bpt_1.cptr = (char *)p_1 ;
	break ;
    case FLOAT_TYPE:
	allocedTheSize.floats += definc ; 
	bpt.fptr = (float *)p ; bpt_1.fptr = (float *)p_1 ;
	break ;
    default:
	assert(0) ;
    }	     
#endif
    
#else
    size_t tmp_run ;    
    tmp_run = (size_t)(bpt - bpt_1) ;    
    bpt_1 = (bufptr) realloc((void *)bpt_1, 
			     (default_inc + allocedTheSize)*sizeof(buftyp)) ;
    
    bpt = bpt_1 + tmp_run ;        
    allocedTheSize += default_inc ;
#endif    
}

void aPackBuf::get_more_mem(const int msize){
    packbuf_size_t tmp =  default_inc ;
    
#ifdef PP3ARR
    default_inc.ints    += msize ;
    default_inc.doubles += msize ;
    default_inc.chars   += msize ;
    default_inc.floats += msize ;
#else
    default_inc += msize ;
#endif    
    get_more_mem(CHAR_TYPE, 0) ;
    default_inc = tmp ;
}


void aPackBuf::useSize(const packbuf_size_t size){
#ifdef PP3ARR    
    
#ifdef TALK_PRIVPACK
    cerr << "PP:useSize "
	 << size.ints << " "
	 << size.doubles << " "
	 << size.chars << " " 
	 <<  size.floats << endl ;
    
#endif
    
    get_more_mem(INT_TYPE, size.ints) ;   
    get_more_mem(DOUBLE_TYPE, size.doubles) ;    
    get_more_mem(CHAR_TYPE, size.chars) ;
    get_more_mem(FLOAT_TYPE, size.floats) ;
    
#else
    if(allocedTheSize == 0){
        bpt_1   = (char *) malloc (size *sizeof(char)) ;
        assert(bpt_1 != 0) ;    
        bpt = bpt_1 ;
        allocedTheSize = size ; 
    } else {
        if(allocedTheSize > size){
            bpt = bpt_1 ; 
        } else {
            free( bpt_1 );
            bpt_1   = (char *) malloc (size *sizeof(char)) ;
            assert(bpt != 0) ;  
            allocedTheSize = size ;         
            bpt = bpt_1 ; 
        }
    }
#endif
}


int aPackBuf::saveToFile(const char *filename){
#ifdef PP3ARR
    int noints, nodoubles, nochars, nofloats;
    int ret = -1 ;
    noints = (size_t)(bpt.iptr-bpt_1.iptr) ;
    nodoubles = (size_t)(bpt.dptr-bpt_1.dptr) ;
    nofloats = (size_t)(bpt.fptr-bpt_1.fptr) ;
    nochars   = (size_t)(bpt.cptr-bpt_1.cptr) ;
    int f ;
    
    f = open(filename, O_WRONLY | O_CREAT, 0644 ) ;
    if(f == -1)
	return -1 ;

    noints /= sizeof(int) ;
    nodoubles /= sizeof(double) ;
    nochars /= sizeof(char) ;
    nofloats /= sizeof(float) ;

#ifdef TALK_PRIVPACK    
    cerr << "aPackBuf::saveToFile writes "
	 << noints << " ints, "
	 << nodoubles << " doubles, "
	 << nochars << " chars," 
	 << nofloats << " floats" 
         << endl ;
#endif
    
    if(write(f, (void *)&noints, sizeof(int)) != sizeof(int)) goto ppstferror ; 
    if(write(f, (void *)&nodoubles, sizeof(int)) != sizeof(int)) goto ppstferror ; 
    if(write(f, (void *)&nochars,   sizeof(int)) != sizeof(int)) goto ppstferror ;
    if(write(f, (void *)&nofloats,   sizeof(int)) != sizeof(int)) goto ppstferror ;
    
    if(write(f, (void *)bpt_1.iptr,
             sizeof(int)*noints) != noints*(int)(sizeof(int)))
	goto ppstferror ;
    
    if(write(f, (void *)bpt_1.dptr,
             sizeof(double)*nodoubles) != nodoubles*(int)(sizeof(double)))
	goto ppstferror ;
        
    if(write(f, (void *)bpt_1.cptr,
              sizeof(char)*nochars) != nochars*(int)(sizeof(char)))
	goto ppstferror ; 

    if(write(f, (void *)bpt_1.fptr,
              sizeof(float)*nofloats) != nofloats*(int)(sizeof(float)))
	goto ppstferror ; 
    ret = 0 ;
    
ppstferror:
    if(ret) cerr << "Failed write to file" << endl ;
    close(f) ;
    return ret ;
    
#else
    assert(0) ; return 1 ;
#endif
}


int aPackBuf::saveToFile(const char *filename, packbuf_size_t x){
#ifdef PP3ARR
    int noints, nodoubles, nochars, nofloats,  ret = -1 ;    
    noints = x.ints ;
    nodoubles = x.doubles ;
    nochars   = x.chars ;
    nofloats = x.floats ;
    int f ;
    
    f = open(filename, O_WRONLY | O_CREAT, 0644) ;
    if(f == -1)
	return -1 ;

    noints /= sizeof(int) ;
    nodoubles /= sizeof(double) ;
    nochars /= sizeof(char) ;
    nofloats /= sizeof(float) ;

#ifdef TALK_PRIVPACK    
    cerr << "aPackBuf::saveToFile writes "
	 << noints << " ints, "
	 << nodoubles << " doubles, "
	 << nochars << " chars,"
	 << nofloats << " floats"
         << endl ;
#endif
    
    if(write(f, (void *)&noints, sizeof(int)) != sizeof(int)) goto ppstferror ; 
    if(write(f, (void *)&nodoubles, sizeof(int)) != sizeof(int)) goto ppstferror ; 
    if(write(f, (void *)&nochars,   sizeof(int)) != sizeof(int)) goto ppstferror ;
    if(write(f, (void *)&nofloats,   sizeof(int)) != sizeof(int)) goto ppstferror ;
    
    if(write(f, (void *)bpt_1.iptr,
             sizeof(int)*noints) != noints*(int)(sizeof(int)))
	goto ppstferror ;
    
    if(write(f, (void *)bpt_1.dptr,
             sizeof(double)*nodoubles) != nodoubles*(int)(sizeof(double)))
	goto ppstferror ;
        
    if(write(f, (void *)bpt_1.cptr,
              sizeof(char)*nochars) != nochars*(int)(sizeof(char)))
	goto ppstferror ; 

    if(write(f, (void *)bpt_1.fptr,
              sizeof(float)*nofloats) != nofloats*(int)(sizeof(float)))
	goto ppstferror ; 
    ret = 0 ;
    
ppstferror:
    if(ret) cerr << "Failed write to file" << endl ;
    close(f) ;
    return ret ;
    
#else
    assert(0) ; return 1 ;
#endif
}



int aPackBuf::readFromFile(const char *filename){
#ifdef PP3ARR
    int ret = -1 ;        
    packbuf_size_t use ;
    int f ;
    
    f = open(filename, O_RDONLY) ;
    if(f == -1)
	return -1 ;
    
    if(read(f, (void *)&use.ints, sizeof(int)) != sizeof(int))
	goto ppwfferror ; 
    if(read(f, (void *)&use.doubles, sizeof(int)) != sizeof(int))
	goto ppwfferror ; 
    if(read(f, (void *)&use.chars, sizeof(int)) != sizeof(int))
	goto ppwfferror ; 
    if(read(f, (void *)&use.floats, sizeof(int)) != sizeof(int))
	goto ppwfferror ; 
    
#ifdef TALK_PRIVPACK
    cerr << "aPackBuf::readFromFile writes "
	 << use.ints << " ints, "
	 << use.doubles << " doubles, "
	 << use.chars << " chars,"
	 << use.floats << " floats "
	 << endl ;
#endif    
    
    useSize(use) ;
    reset() ;
    
    if(read(f, (void *)bpt_1.iptr,
            sizeof(int)*use.ints) != use.ints*(int)(sizeof(int)))
	goto ppwfferror ; 
    
    if(read(f, (void *)bpt_1.dptr,
            sizeof(double)*use.doubles) != use.doubles*(int)(sizeof(double)))
	goto ppwfferror ; 
    if(read(f, (void *)bpt_1.cptr,
            sizeof(char)*use.chars) != use.chars*(int)(sizeof(char)))
	goto ppwfferror ; 
    if(read(f, (void *)bpt_1.fptr,
            sizeof(float)*use.floats) != use.floats*(int)(sizeof(float)))
	goto ppwfferror ; 
    
    ret = 0 ;
    
ppwfferror:
    close(f) ;
    return ret ;
#else
    assert(0) ;  return 1 ;
#endif
}


//! Packs a  vector<double>.  BZ1374
void aPackBuf::pk(const vector<double> &x){
    pk(static_cast<int>(x.size()));
    direct_arr_pk(&x[0],x.size());
}
//! Unpacks a  vector<double>.  BZ1374
void aPackBuf::upk(vector<double> &x){
    int len;
    upk(len);
    x.resize(len);
    direct_arr_upk(&x[0],x.size());
}
//! Packs a  vector<double>.  BZ1374
void aPackBuf::pk(const vector<vector<double> > &x){
    pk(static_cast<int>(x.size()));
    for(size_t i=0;i<x.size();i++){
        pk(x.at(i));
    }
}
//! Unpacks a  vector<double>.  BZ1374
void aPackBuf::upk(vector<vector<double> > &x){
    int len;
    upk(len);
    x.resize(len);
    for(size_t i=0;i<x.size();i++){
        upk(x.at(i));
    }
}
