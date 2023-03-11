//
//  MŮJ VLASTNÍ KÓD Z ROKU 2017
//

#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TEmployee
{
    struct TEmployee         * m_Next;
    struct TEmployee         * m_Bak;
    char                     * m_Name;
} TEMPLOYEE;

#endif /* __PROGTEST__ */

TEMPLOYEE * newEmployee( const char * name, TEMPLOYEE * next )
{
    
    TEMPLOYEE *noob = (TEMPLOYEE*) malloc(sizeof(*noob));
    
    if ( name == NULL ){
        noob->m_Bak = NULL;
        noob->m_Name = NULL;
    }
    
    else {
        char *jmeno = (char*) calloc(strlen(name)+1, sizeof(*jmeno));
        
        strcpy(jmeno, name);
        
        noob->m_Next = next;
        noob->m_Bak = NULL;
        noob->m_Name = jmeno;
    }
    
    return noob;
}


TEMPLOYEE * cloneList( TEMPLOYEE * src )
{
    
    if( src == NULL )
        return NULL;
    
    
    TEMPLOYEE * tmp = src; // slouží pro práci se seznamem, abychom neztratili ukazatel na první prvek seznamu co kopírujeme
    TEMPLOYEE * clone = NULL;
    
    while ( tmp != NULL ) {
        
        clone = (TEMPLOYEE*) malloc(sizeof(*clone)); // vytváříme prázdnou kopii
        
        clone -> m_Next = tmp -> m_Next; // klon ukazuje na druhý prvek originálního seznamu
        tmp -> m_Next = clone;           // první prvek originálního seznamu ukazuje na klon (1 > 1' > 2)
        
        if ( tmp -> m_Name != NULL ){
            char *name = (char*) calloc(strlen(tmp->m_Name)+1, sizeof(*name));   // alokace místa pro "nové" jméno
            strcpy( name, tmp -> m_Name );                      // jméno je zkopírováno z původního prvku do proměnné name
            clone -> m_Name = name;                     // jméno je uloženo do kopie prvku seznamu
        }
        tmp = tmp -> m_Next -> m_Next;              // přeskakujeme o dva ukazatele
        
    }
    
    tmp = src; // RESET proměné tmp zpět na první pozici seznamu
    TEMPLOYEE * first  = tmp -> m_Next;
    
    
    /*
     * ! Upravená část mechanizmu pro kopírování "random pointerů" (m_Bak) a následného rozpojení !
     * !           spojového seznamu byla převzata z: careercup.com/question?id=14417753          !
     */
    
    
    // Kopírování zástupců do nového seznamu
    while ( tmp != NULL ) {
        
        clone = tmp -> m_Next;
        
        // pokud má originál definován vedoucího, zkopíruje se
        if( tmp -> m_Bak != NULL )
            clone -> m_Bak = tmp -> m_Bak -> m_Next;
        else
            clone -> m_Bak = NULL;
        
        tmp = clone -> m_Next;
    }
    
    tmp = src;
    
    // Rozdělení propleteného spojového seznamu na originál a jeho kopii
    while ( tmp != NULL ) {
        
        clone = tmp -> m_Next;
        
        tmp -> m_Next = clone -> m_Next;
        
        if ( clone -> m_Next != NULL )
            clone -> m_Next = clone -> m_Next -> m_Next;
        
        tmp = tmp -> m_Next;
    }
    
    return first;
    
}



void freeList ( TEMPLOYEE * src )
{
    
    if ( src != NULL ){
        
        TEMPLOYEE * tmp;
        
        while ( src -> m_Next != NULL )
        {
            tmp = src -> m_Next;
            free  ( src -> m_Name );
            free  ( src );
            src = tmp;
        }
    }
    if( src != NULL ){
        free  ( src -> m_Name );
        free ( src );
    }
    
}




#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TEMPLOYEE * a, *b;
    char tmp[100];
    
    assert ( sizeof ( TEMPLOYEE ) == 3 * sizeof ( void * ) );
    a = NULL;
    a = newEmployee ( "Peter", a );
    a = newEmployee ( "John", a );
    a = newEmployee ( "Joe", a );
    a = newEmployee ( "Maria", a );
    a -> m_Bak = a -> m_Next;
    a -> m_Next -> m_Next -> m_Bak = a -> m_Next -> m_Next -> m_Next;
    a -> m_Next -> m_Next -> m_Next -> m_Bak = a -> m_Next;
    assert ( a
            && ! strcmp ( a -> m_Name, "Maria" )
            && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    b = cloneList ( a );
    strncpy ( tmp, "Moe", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    strncpy ( tmp, "Victoria", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    strncpy ( tmp, "Peter", sizeof ( tmp ) );
    a = newEmployee ( tmp, a );
    b -> m_Next -> m_Next -> m_Next -> m_Bak = b -> m_Next -> m_Next;
    assert ( a
            && ! strcmp ( a -> m_Name, "Peter" )
            && a -> m_Bak == NULL );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
            && a -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
            && a -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == NULL );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    assert ( b
            && ! strcmp ( b -> m_Name, "Maria" )
            && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
            && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
            && b -> m_Next -> m_Bak == NULL );
    assert ( b -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
            && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( a );
    b -> m_Next -> m_Bak = b -> m_Next;
    a = cloneList ( b );
    assert ( a
            && ! strcmp ( a -> m_Name, "Maria" )
            && a -> m_Bak == a -> m_Next );
    assert ( a -> m_Next
            && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
            && a -> m_Next -> m_Bak == a -> m_Next );
    assert ( a -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
            && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next );
    assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    assert ( b
            && ! strcmp ( b -> m_Name, "Maria" )
            && b -> m_Bak == b -> m_Next );
    assert ( b -> m_Next
            && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
            && b -> m_Next -> m_Bak == b -> m_Next );
    assert ( b -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
            && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next
            && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
            && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
    assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    freeList ( b );
    freeList ( a );
    return 0;
}
#endif /* __PROGTEST__ */
