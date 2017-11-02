// Test-program for Bstring

#include <cassert>
#include <cstring>

#include "Bstring.h"
#include "Btokenizer.h"

int main()
{
    // Test empty strings first
    Bstring empty("");
    Bstring cempty(empty);
    Bstring ccempty(cempty);

    assert( empty.size() == 0 );
    assert( cempty.size() == 0 );
    assert( ccempty.size() == 0 );
    assert( ccempty == empty );

    ccempty = empty;
    assert( ccempty.size() == 0 );
    assert( ccempty == empty );

    // Test non empty strings
    Bstring t("My test string");
    Bstring tt(t);
    Bstring ttt("My test string", 3);
    Bstring tttt('M');

    assert( t == "My test string" );
    assert( tt == "My test string" );
    assert( ttt == "My " );
    assert( tttt == "M" );

    t = "  " + t + "  +++   ";

    t.strip();
    assert( t == "  My test string  +++" );

    t.strip( Bstring::trailing, '+' );
    assert( t == "  My test string  " );

    t.strip(Bstring::both);
    assert( t == "My test string" );

    t.toLower();
    assert( t == "my test string" );
    
    t.toUpper();
    assert( t == "MY TEST STRING" );
    
    assert( t.compareTo("MY TEST STRING") == 0 );
    assert( t.compareTo("Not MY TEST STRING") < 0 );
    assert( t.compareTo("Is MY TEST STRING") > 0 );
    assert( t.compareTo("my test string") < 0 );
    
    const char* cd = t.data();
    char d[8];
    for( int i=0 ; i < 7 ; i++ )
        d[i] = cd[i];
    d[7] = 0x00;
    
    assert( strcmp( d, "MY TEST" ) == 0 );
    
    assert( t(0, 7) == "MY TEST" );

    for( int i = 0 ; i < 7 ; i++ )
        assert( t(i) == d[i] );
        
    assert( t.index( "TEST", 0 ) == 3 );
    assert( t.index( "test", 0 ) == string::npos );
    assert( t.index( "test", 0, Bstring::ignoreCase) );
    
    assert( t.contains("TEST") );
    assert( !t.contains("test") );
    assert( t.contains("test", Bstring::ignoreCase) );
        
    assert( t.first('S') == 5 );
    assert( t.last('S') == 8 );
  
    assert( !t.isNull() );
    assert( Bstring("").isNull() );
    
    assert( t.isAscii() );

    //tt += char(-128);
    //assert( !tt.isAscii() );
    
    t.prepend( "Is " );
    assert( t == "Is MY TEST STRING" );
    
    t.prepend( "ABCD", 2 );
    assert( t == "ABIs MY TEST STRING" );

    t.remove(0, 5);
    assert( t == "MY TEST STRING" );
        
    t.resize(20);
    assert( t.length() == 20 );
    
    t.strip();


    

    
    t.replaceOnce("TEST", "Test");
    assert( t == "MY Test STRING" );
    
    Btokenizer tok(t);
    t = tok();
    tt = tok();
    ttt = tok();
    tttt = tok();
    
    assert( t == "MY" );
    assert( tt == "Test" );
    assert( ttt == "STRING" );
    assert( tttt == "" );

    t = "My$Test*String";
    Btokenizer tok2(t);
    t = tok2("$*");
    tt = tok2("$*");
    ttt = tok2("$*");
    tttt = tok2("$*");

    assert( t == "My" );
    assert( tt == "Test" );
    assert( ttt == "String" );
    assert( tttt == "" );

    cout << "Test OK" << endl;

    return 0;
}
