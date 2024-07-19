int
hasABCD( char str[] ) {
#define FOUND_NONE  0
#define FOUND_A     1
#define FOUND_B     2
#define FOUND_C     3
  int status = FOUND_NONE;
  char *p;

  for( p=str; *p; p++ ) {
    // この続きのコードを書いて関数を完成させよう
