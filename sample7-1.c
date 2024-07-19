// 得点の score[ ] の降順に、順位 rank[ ] をつける。nは配列のサイズ。
// 実例： n=4, score=[ 70, 93, 95, 93 ] なら rank=[ 4, 2, 1, 2 ] とする
void putRanking( int score[], int rank[], int n ) 
{
    int hist[101], acc[101];

    // 配列 hist[101] の、hist[ s ] の値が得点 s を取った人数になるようにする。 
    .......

    // 配列 acc[101]の、acc[ s ] の値が、得点 s よりも高い得点を取った人数になるようにする。
    .......

    // acc[ score[ i ] ] の値に1を加えた値を、rank[ i ] に入れる。
    .......

}
