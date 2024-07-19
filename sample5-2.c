double
stdDev( double value[], int n ) {
    double average;
    {
        double sum = 0.0;
        for( int i=0; i<n; i++ ) {
            sum += value[i];
        }
        average = sum / (double) n;
    }
    {
        double sumErrSq = 0.0;
        for( int i=0; i<n; i++ ) {
            double err;
            err = value[i]-average;
            sumErrSq += err*err;
        }
        return( sqrt(sumErrSq) );
    }
}
