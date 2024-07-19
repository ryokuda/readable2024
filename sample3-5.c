// 
char path[] = "/home/user1/downloads/package.txt"
char *p;
printf( "%s\n", (p=strrchar( path, '/' )) ? p+1 : path );
