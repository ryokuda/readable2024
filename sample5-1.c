double
stdDev( double value[], int n ) {
    int i;
    double val1, val2;
    val1 = 0.0;
    for( i=0; i<n; i++ ) {
        val1 += value[i];
    }
    val1 /= (double) n; // average
    val2 = 0.0;
    for( i=0; i<n; i++ ) {
        val2 += (value[i]-val1)*(value[i]-val1);
    }
    val2 /= (double) n;
    return( sqrt(val2) );
}
