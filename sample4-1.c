#include <stdio.h>

char *val[] = {
 "o3732",   // 8進数
 "2560",    // 10進数
 "b10011001111",    // 2進数
 "",
};


void
main(){
    int sum=0;
    for( int i=0; *val[i]; i++ ) {
        char *p = val[i];
        int radix = 10;
        switch( *p++ ) {
            case 'b': radix=2; break;
            case 'o': radix=8; break;
            case 'd': radix=10; break;
            default: p--;
        }
        int val=*p - '0';
        while( *(++p) ) {
            val *= radix;
            val += *p - '0';
        }
        sum += val;
    }
    printf( "total=%d\n", sum );
}
