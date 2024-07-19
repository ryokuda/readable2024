int
hasABCD( char str[] ) {
  int aFound = 0;
  int bFound = 0;
  int cFound = 0;
  char *p;

  for( p=str; *p; p++ ) {
    if( cFound ) {
      if( *p == 'd' ) return /*true*/ 1;
    } else if( bFound ) {
      if( *p == 'c' ) cFound = 1;
    } else if( aFound ) {
      if( *p == 'b' ) bFound = 1;
    } else {
      if( *p == 'a' ) aFound = 1;
    }
  }
  return /*false*/ 0;
}
