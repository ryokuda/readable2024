#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_MAPPABLE_COST 50	// 人と顔の対応づけ可能な最大コスト。要調整

#define INIT_SIZE 256		// 可変サイズ配列のとりあえずのサイズ
#define ABS(val) ((val)>=0?(val):-(val))

void usage(char *cmd) { fprintf( stderr, "usage: %s input-file-name\n", cmd ); }
#define FatalError() { fprintf( stderr, "Fatal error at line %d\n", __LINE__ ); exit(-1); }
char *alloc_and_copy(char *s) { char *d; d = (char *)malloc(strlen(s)+1); strcpy(d,s); return(d); }

/*-------------------------------------
 *	認識された顔の情報
 *------------------------------------*/
struct face {	// 認識された一つの顔情報。入力ファイルの１行に相当
	int jpg_index;		// 0から始まるJPEGファイルの連番
	char *jpg_name;		// JPEGファイル名
	int x, y, r;		// 顔認識の円のxy座標と半径
};

struct face_list {	// 顔情報（face）のリスト。サイズ可変
	int array_size;	// facesにアロケートされた配列のサイズ
	int max_index;	// facesに入っている有効データの数
	struct face **faces;	// 顔情報(face)のポインタを入れる配列
};

struct face *
new_face( int idx, char *name, int x, int y, int r ) {
	struct face *new = (struct face *) malloc( sizeof(struct face) );
	new->jpg_index = idx;
	new->jpg_name = name;
	new->x = x;
	new->y = y;
	new->r = r;
	return( new );
}

#define NUM_FACE( face_list ) ((face_list)->max_index)

	// 顔のリストの要素各々に対するループのマクロ
#define FOR_EACH_FACE_OF_LIST( face, list, index ) \
	for( (index)=0; \
		(face)=(list)->faces[(index)], (index)<NUM_FACE(list); \
		(index)++ )

	// マクロ類は全てポインタ渡しとする
#define COST_BETWEEN_FACES( faceA, faceB ) \
	(	ABS((faceA)->y-(faceB)->y) *5 + \
	 	ABS((faceA)->x-(faceB)->x) )
	// 二つの顔の間のコスト、近いほど値が小さい
	// y座標が異なるとカメラからの距離が異なる可能性あり、
	// ５倍の重み付けをした

void
init_face_list( struct face_list *ptr ) { // 顔情報のリストの初期化
	assert( ptr!=NULL );	// 初期化時
	ptr->array_size = INIT_SIZE;
	ptr->max_index = 0;
	ptr->faces = (struct face **) malloc( ptr->array_size*sizeof(struct face *));
}

void
clear_face_list( struct face_list *ptr ) { // 顔情報のリストを空にする
	assert( ptr!=NULL );
	ptr->max_index = 0;
}

void	// 顔情報のリストに一つの顔情報を追加する
add_one_face_to_list( struct face *face, struct face_list *face_list ) {
	assert( face!=NULL && face_list!=NULL );
	if( NUM_FACE(face_list) >= face_list->array_size ) { // 配列あふれ
		face_list->array_size += INIT_SIZE;
		// 配列のサイズを大きくして領域を取り直す
		face_list->faces = (struct face **) realloc( face_list->faces, face_list->array_size*sizeof(struct face *));
	}
	face_list->faces[NUM_FACE(face_list)] = face;
	face_list->max_index++;
}

void // 顔情報のプリントアウト。デバッグ用
print_face( struct face *face ) {
	fprintf( stderr, "%d %s %d %d %d\n",
		face->jpg_index,
		face->jpg_name,
		face->x, face->y, face->r );
}

void // 顔情報のリストのプリントアウト。デバッグ用
print_face_list( struct face_list *list ) {
	int idx;
	struct face *face;
	FOR_EACH_FACE_OF_LIST( face, list, idx ) {
		print_face( face );
	}
}


/*-------------------------------------
 *	人の情報
 *------------------------------------*/

struct person {		// 人の情報
	int person_index;	// 0から始まる人の連番
	struct face_list face_list;	// 顔情報のポインタの配列
};

struct person_list {	// 人のリスト。サイズ可変
	int array_size;	// アロケートされている配列のサイズ
	int max_index;	// 有効データの数
	struct person **people; // 人情報のポインタの配列
};

#define NUM_FACE_OF_PERSON( p ) ((p)->max_index)
#define NUM_PERSON person_list.max_index	// これまで検出された人の数

	// 人のリストの要素各々に対するループのマクロ、ネストしては使えないので注意
#define FOR_EACH_PERSON_OF_LIST( psn ) \
	for( (loop_index_for_access_person_list)=0; \
		(psn)=person_list.poeple[(loop_index_for_access_person_list)], \
		(loop_index_for_access_person_list)<NUM_PERSON; \
		(loop_index_for_access_person_list)++ )
int loop_index_for_access_person_list;	// 上記のループを回るためのループ変数

struct person_list person_list = { 0, 0, NULL};	// 過去検出された人情報のデータ構造の根っこ

#define PERSON_OF_INDEX( index ) (person_list.people[(index)])  // index番目の人情報

void
init_person_list() {	// 人情報のリストの初期化をする
	person_list.array_size = INIT_SIZE;
	person_list.max_index = 0;
	person_list.people = (struct person **) malloc( person_list.array_size*sizeof(struct person *));
}

struct person *
create_person() {	// 新しい人情報の構造体をアロケートして、person_listに登録する
	struct person *new = (struct person *) malloc( sizeof(struct person) );
	new->person_index = NUM_PERSON; // これまでに検出した人の数が、新しい人情報のindex
	if( NUM_PERSON >= person_list.array_size ) { // 配列あふれ
		person_list.array_size += INIT_SIZE;
		// 配列のサイズを大きくして領域を取り直す
		person_list.people = (struct person **) realloc( person_list.people, person_list.array_size*sizeof(struct person *));
	}
	person_list.people[NUM_PERSON] = new;
	NUM_PERSON++;
	return( new );
}

/*-------------------------------------
 *	顔と人のマッチングをとるためのコスト計算
 *------------------------------------*/
int
calc_cost_between_face_and_person( struct face *new_face, struct person *person ) {
	struct face *past_face;	
	int index;
	int sum_of_cost=0;
	assert( person->face_list.max_index>0 ); // 人情報には少なくとも１以上の顔情報があること

	// 人情報には既に一つ以上の過去の顔情報が登録されている
	// 新しい顔情報と、過去の顔情報とのコストの平均値を計算して返す
	FOR_EACH_FACE_OF_LIST( past_face, &(person->face_list), index ) {
		sum_of_cost += COST_BETWEEN_FACES( new_face, past_face );
	}
	sum_of_cost /= person->face_list.max_index; // 平均値の計算
	return( sum_of_cost );
}


/*-------------------------------------
 *	顔と人のマッチングのコスト表のメンテ
 *------------------------------------*/
int global_num_face;		// コスト表の顔の数
int global_num_person;		// コスト表の人の数
int cost_table_size=0;
int *cost_table;	// 計算したコストを格納するテーブルの記憶領域

#define NO_COST	(-1)	// コストでは無い値。テーブルの各エントリはこの値に初期化される
#define BIG_COST 1000000
#define ALREADY_MATCHED BIG_COST

#define COST_TABLE( face_idx, person_idx ) \
	(cost_table[(face_idx)+(person_idx)*global_num_face])

#define FOR_EACH_FACE_OF_COST_TABLE( face_index ) \
		for( (face_index)=0; (face_index)<global_num_face; (face_index)++ )
#define FOR_EACH_PERSON_OF_COST_TABLE( person_index ) \
		for( (person_index)=0; (person_index)<global_num_person; (person_index)++ )

void
init_cost_table( int num_face, int num_person ) {	// コスト表の初期化
	int needed_size = num_face*num_person;
	int f,p;
	assert( num_face !=0 && num_person != 0 );
	if( needed_size > cost_table_size ) {	//　配列のサイズが足りない！
		if( cost_table_size != 0 ) free( cost_table );
		while( needed_size > cost_table_size ) cost_table_size += INIT_SIZE; // どれだけ大きくすればよいか
		cost_table = (int *) malloc( cost_table_size*sizeof(int *));
	}
	global_num_face = num_face;
	global_num_person = num_person;
	FOR_EACH_PERSON_OF_COST_TABLE( p ) {
		FOR_EACH_FACE_OF_COST_TABLE( f ) {
			COST_TABLE( f, p ) = NO_COST;
		}
	}
}

void
add_one_cost_to_table( int face_idx, int person_idx, int cost ) {
	assert( face_idx<global_num_face && person_idx<global_num_person && cost>=0 );
	COST_TABLE( face_idx, person_idx ) = cost;
}

// テーブルから最小コストの顔と人を探す
int
search_min_cost_from_table( int *face_idx, int *person_idx ) {
	int f, p;
	int min_cost = BIG_COST+1;
	int min_f=-1, min_p=-1;
	FOR_EACH_PERSON_OF_COST_TABLE( p ) {
		FOR_EACH_FACE_OF_COST_TABLE( f ) {
			if( COST_TABLE( f, p ) < min_cost ) {
				min_f = f;
				min_p = p;
				min_cost = COST_TABLE( f, p );
			}
		}
	}
	assert( min_f>=0 && min_p>= 0 && min_cost<=BIG_COST );
	*face_idx = min_f;
	*person_idx = min_p;
	return( min_cost );
}

// マッチング済みのエントリ、次に選ばれないように大きなコストを設定する
void
mark_as_already_macthed( int face_idx, int person_idx ) {
	int f, p;
	FOR_EACH_PERSON_OF_COST_TABLE( p ) {
		COST_TABLE( face_idx, p ) = ALREADY_MATCHED;
	}
	FOR_EACH_FACE_OF_COST_TABLE( f ) {
		COST_TABLE( f, person_idx ) = ALREADY_MATCHED;
	}
}


// このプログラムの非常に大切な核心の部分
// face_list は、新しく読み込んだjpgファイルの顔情報のリスト
// この顔情報の各々を、過去に検出された人情報にマッピングする。
// 過去に検出された人の顔だと判断すれば、その人情報に顔情報を追加
// 新しい人の顔だと判断すれば、新しい人情報を登録する。
void
match_faces_to_poeple( struct face_list *new_face_list )
{
	struct face *new_face;
	int face_index;
	struct person *person;

	if( NUM_FACE(new_face_list) == 0 ) return;	// 新しい顔情報が無ければ何もしない

	if( NUM_PERSON == 0 ) {	// 過去検出された人が一人も居なければ
		FOR_EACH_FACE_OF_LIST( new_face, new_face_list, face_index ) { // 過去の顔情報のループ
			struct person *new_person;
			new_person = create_person(); // 新しい人情報を作る

			// 新しい人情報に、顔情報を登録する
			add_one_face_to_list( new_face, &(new_person->face_list) );
		}
	} else {	// 過去検出された人が一人でもいれば
		// コストテーブルの初期化
		// 新しい顔の数　X 過去に検出された人情報の数　の二次元配列
		init_cost_table( NUM_FACE(new_face_list), NUM_PERSON ); 

		// コストテーブルの全てのエントリにコストを計算して入れる
		FOR_EACH_PERSON_OF_LIST( person ) { // 全ての新しい人情報のループ
			FOR_EACH_FACE_OF_LIST( new_face, new_face_list, face_index ) { // 過去の顔情報のループ
				int cost;
				cost = calc_cost_between_face_and_person( new_face, person );
				add_one_cost_to_table( face_index, person->person_index, cost );
			}
		}

		// デバッグのため、コストテーブルの全てのエントリにコスト情報が入っている
		// ことを確認しておく
		{
			int f, p;
			FOR_EACH_FACE_OF_COST_TABLE( f ) {
				FOR_EACH_PERSON_OF_COST_TABLE( p ) {
					if( COST_TABLE( f, p ) == NOT_COST ) FatalError();
				}
			}
		}

		// コストが小さいものから順番に、新しい顔と、過去の人情報を
		// マッチングさせていく。
		// ただし、コストがパラメータ MAX_MAPPABLE_COST を越えると
		// マッピング終了
		for(;;) {
			int matched_face_idx, matched_person_idx, matched_cost;

			// 最小のコストの顔と人の組み合わせを探す
			matched_cost = search_min_cost_from_table( &matched_face_idx, &matched_person_idx );
			if( matched_cost > MAX_MAPPABLE_COST ) break; // 制限を越えたので終了

			// マッチした人情報に、マッチした顔情報を登録する
			add_one_face_to_list(
				new_face_list->faces[matched_face_idx], // マッチした顔情報
				&(PERSON_OF_INDEX(matched_person_idx)->face_list) // マッチした人情報
			);

			// マッチした人と、顔は、もう選ばれないように大きなコストを設定しておく
			mark_as_already_macthed( matched_face_idx, matched_person_idx );
		}

		// マッピングできなかった顔は、新しい人だと判断して
		// 新しい人情報を登録する
		{
			int f, p;
			FOR_EACH_FACE_OF_COST_TABLE( f ) {
				int small_cost_found=0;
				FOR_EACH_PERSON_OF_COST_TABLE( p ) {
					if( COST_TABLE( f, p ) < ALREADY_MATCHED ) {
						small_cost_found = 1;
						break;
					}
				}
				if( small_cost_found ) { // この顔情報はマッピングされなかった
					int num_person = NUM_PERSON; // デバッグ用
					struct person *new_person;
					new_person = create_person(); // 新しい人情報を作る

					// デバッグのための確認
					{
						assert( (num_person+1)==NUM_PERSON ); // 人が一人増えているはず
						assert( PERSON_OF_INDEX(num_person)==new_person ); // 表に登録されているかチェック
						assert( new_person->person_index==num_person);
					}

					// 新しい人情報に、顔情報を登録する
					add_one_face_to_list(
						new_face_list->faces[f],
						&(new_person->face_list) );
					
				}
			}
		}
	}
		
}


/*-------------------------------------
 *	入力ファイルの読み込み
 *------------------------------------*/
FILE *infile;
struct face_list temp_face_list;

void
init_input_file( char *filename ) {
	if( (infile=fopen(filename,"r")) == NULL ) FatalError();
	init_face_list( &temp_face_list );
}

struct face_list *
read_one_picture() {
	char line[512]; //　１行が512文字を越えないこと
	static int jpg_index=-1;	// JPEGファイルの連番
	char *jpg_name;
	int x, y, r;
	struct face *face;

	// ファイル名を読む
	if( fgets( line, 512, infile ) == NULL ) return( NULL ); // ファイルの終わり
	if( strstr( line, "jpg" ) == NULL ) FatalError();
	jpg_name = alloc_and_copy(line);
	sscanf( line, "%s", jpg_name );
	// fprintf( stderr, "== %s\n", jpg_name );  // デバッグ用

	jpg_index++;
	clear_face_list( &temp_face_list );	// 顔情報のリストをクリア

	// '-'が現れるまで顔情報を読む
	for(;;) {
		if( fgets( line, 512, infile ) == NULL ) FatalError(); // ファイル異常終了
		if( line[0] == '-' ) break;
		sscanf( line, "%d %d %d", &x, &y, &r );
		// fprintf( stderr, "%d %d %d\n", x, y, r );  // デバッグ用
		face = new_face( jpg_index, jpg_name, x, y, r );
		add_one_face_to_list( face, &temp_face_list );
	}
	return( &temp_face_list );
}


/*-------------------------------------
 *	メインループ
 *------------------------------------*/
int
main( int ac, char **av ) {
	struct face_list *face_list;
	if( ac != 2 ) usage( av[0] );
	init_input_file( av[1] );
	init_person_list();
	while( (face_list = read_one_picture()) != NULL ) {
		// print_face_list( face_list );  // デバッグ用
		match_faces_to_poeple( face_list );
	}
}
