
#include <stdio.h>
#include <float.h>


double sail_a, sail_b;


int min( int * a, int * b )
{

    if ( *a < *b || *b < 0 )
        return *a;
    else
        return *b;
}


int getResult( double side_a, double side_b, double * overlap)
{

    int a = 1, b = 1;
    double count;
    
    
    // ------- Side A -------
    count = sail_a - side_a;
    
    if ( count > 0 )
    {
        count = count / (side_a - *overlap) - (10000 * DBL_EPSILON);
    
        // double -> int (lowerbound) -> double ( 4 comparison )
        if ( (double)(int)count < count )
            a++;
    
        a += (int)count;
    }
    // ----------------------
    
    
    // ------- Side B -------
    count = sail_b - side_b;
    
    if ( count > 0 )
    {
        count = count / (side_b - *overlap) - (10000 * DBL_EPSILON);
    
        if ( (double)(int)count < count )
            b++;
    
        b += (int)count;
    }
    // ----------------------
    
    return a * b;
    
}





int main()
{
  
    double fab_a, fab_b;
    double overlap = 0;
    
    
    printf("Velikost latky:\n");
    if ( scanf("%lf %lf", &fab_a, &fab_b) != 2 )
    {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    // zero or negative fabric size is bad
    if ( fab_a <= 0 || fab_b <= 0 )
    {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    
    
    printf("Velikost plachty:\n");
    if ( scanf("%lf %lf", &sail_a, &sail_b) != 2 )
    {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    // zero or negative sail size is bad
    if ( sail_a <= 0 || sail_b <= 0 )
    {
        printf("Nespravny vstup.\n");
        return 1;
    }
    
    
    
    // if fabric is smaller than sail
    if ( !(fab_a >= sail_a && fab_b >= sail_b) && !(fab_a >= sail_b && fab_b >= sail_a) )
    {
        
        printf("Prekryv:\n");
        
        // invalid input or negative overlap size
        if ( scanf("%lf", &overlap) != 1 || overlap < 0 )
        {
            printf("Nespravny vstup.\n");
            return 1;
        }
        
        if ( (overlap >= fab_a && fab_a < sail_a) || (overlap >= fab_b && fab_b < sail_b) )
        {
            printf("Nelze vyrobit.\n");
            return 0;
        }
        
    }
    else
    {
        printf("Pocet kusu latky: 1\n");
        return 0;
    }
    
    
    int result_a = getResult(fab_a, fab_b, &overlap);
    int result_b = getResult(fab_b, fab_a, &overlap);
    
    
    printf( "Pocet kusu latky: %d\n", min(&result_a, &result_b) );
    
    
    return 0;
    
}
