// ループイテレータに明確な名前を付ける（2.2節）
struct class {
    int num_students;
    char *names[];
} classes[10];

for( i=0; i<10; i++ ) {
    printf( "class %d\n", i );
    for( j=0; j<classes[i].num_students; j++ ) {
        printf( "No.%d %s\n", j, classes[i].names[j] );
    }
}