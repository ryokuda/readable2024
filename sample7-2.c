// 得点の score[ ] の降順に、順位 rank[ ] をつける。nは配列のサイズ。
// 実例： n=4, score=[ 70, 93, 95, 93 ] なら rank=[ 4, 2, 1, 2 ] とする
void putRanking( int score[], int rank[], int n ) 
{
    int hist[101], acc[101];

    // 配列 hist[101] の、hist[ s ] の値が得点 s を取った人の数になるようにする。 
    for( int s=0; s<=100; s++ ) hist[s] = 0;
    for( int i=0; i<n; i++ ) hist[score[i]]++;

    // 配列 acc[101]の、acc[ s ] の値が、得点 s よりも高い得点を取った人数になるようにする。
    acc[100] = 0;
    for( int s=99; s>=0; s-- ) acc[s] = acc[s+1]+hist[s+1];

    // acc[ score[ i ] ] の値に1を加えた値を、rank[ i ] に入れる。
    for( int i=0; i<n; i++ ) rank[i] = acc[score[i]]+1;

}
