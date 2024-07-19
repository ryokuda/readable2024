#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
//最終中間ファイル
#define MacroCell(results,x,y) ((results)[(y)*arrayX+(x)])
#define MacroCelll(resul,x,y) ((resul)[(y)*arX+(x)])
int arrayX=0;
//2点間の距離を測定
int kyori(double a,double b){
 double s=sqrt(pow(a,2.0)+pow(b,2.0));

 int ans=0;
 if(s==0){
    ans=0;
   }else if(s>0 && s<11){
    ans=1;
   }else if(s>10 && s<21){
    ans=2;
   }else if(s>20 && s<31){
    ans=3;
   }else if(s>30 && s<41){
    ans=4;
   }else if(s>40 && s<51){
    ans=5;
   }else if(s>50 && s<63){
    ans=6;
   }else if(s>=63){
    ans=100;
   }
 return ans;
}

//半径の距離
int kyori_r(int a,int b){
 int i=0;
 int s=abs(a-b);
 if(s==0){
  i=0;
 }else if(s>0 && s<3){
  i=1;
 }else if(s>2 && s<6){
  i=2;
 }else if(s>5 && s<11){
  i=3;
 }else if(s>=11){
  i=100;
 }
 return i;
}

int kyo( int **a, int *b,int *c,int **results,int flag)
{
  int i=0,j=0,av=0,bv=0,o=0,y=0,jj=0,pc=1;
  int ans=0;
  int *resul;
  int arX=0;
  int arY=0;
  int min=0,remin=0;
  int maxl=0;
  //printf( "kyo arrayX=%d\n", arrayX );
  for(i=0;MacroCell(*a,0,i);i++){
    //printf("%d\n",MacroCell(*a,0,i));
    //printf( "kyo arrayX=%d\n", arrayX );
    av++;
  }av--;maxl=i;
  for(i=0;b[i*2+0]!=0;i++){
    bv++;
  }
  arX=bv;
  arY=av;
  if((resul=(int *)calloc((arX*arY),sizeof(int)))!=NULL){
    //printf("%d %d\n",av,bv);    
    //コスト表作成
    for(i=0;i<av;i++){
      for(j=0;j<bv;j++){
        //printf("%4d %4d %4d %4d %4d %4d\n",MacroCell(*a,0,i+1),b[j*2+0],MacroCell(*a,1,i+1),b[j*2+1],MacroCell(*a,2,i+1),c[j]);
        ans=kyori(fabs(MacroCell(*a,0,i+1)-b[j*2+0]),fabs(MacroCell(*a,1,i+1)-b[j*2+1]))+kyori_r(MacroCell(*a,2,i+1),c[j]);
        MacroCelll(resul,j,i)=ans;
      }//jの終わり
    }//iの終わり
    /*
    for(i=0;i<av;i++){
      for(j=0;j<bv;j++){
        printf("=%4d ",MacroCelll(resul,j,i));
      }printf("\n");
    }
    */
//コスト表から安いコストを選別する
    for(i=0;i<(av);i++){
      min=MacroCelll(resul,0,i);y=0;
      for(j=0;j<(bv);j++){
        if(min>MacroCelll(resul,j,i)){
          min=MacroCelll(resul,j,i);
          y=j;//行番号を記録
          //座標更新
          //MacroCell(*a,0,i)=b[j*2+0];//x
          //MacroCell(*a,1,i)=b[j*2+1];//y
          //MacroCell(*a,2,i)=c[j];//r
        }
      }//jの終わり
      //一致したものがあればそれ以外を最大コストにする
      if(min<100){
        /*for(o=3;o<flag+3;o++){
          MacroCell(*a,o,i+1)=3;
        }*/
        MacroCell(*a,flag+3,i+1)=1;
        for(o=0;o<(av);o++){
          MacroCelll(resul,y,o)=200;
        }
      }
    }//iの終わり
    /*
    for(i=0;i<av;i++){
    for(j=0;j<bv;j++){
    printf("%3d ",MacroCelll(resul,j,i));
    }printf("\n");
    }
    */
    /*for(i=0;i<15;i++){
    for(j=0;j<arrayX;j++){
    printf("%4d ",MacroCell(*a,j,i));
    }printf("\n");
    }*/
    //一通り回したのちに例外が見つかった場合
    for(i=0;i<(bv);i++){
      remin=MacroCelll(resul,i,0);
      for(j=0;j<(av);j++){
        if(remin>MacroCelll(resul,i,j)){
          remin=MacroCelll(resul,i,j);
        }
      }//jの終わり
      //もし100台の値があれば新しく登録
      if(remin<200){
        for(o=3;o<flag+3;o++){
          MacroCell(*a,o,arY+pc)=3;
        }
        MacroCell(*a,0,arY+pc)=b[i*2+0];//x
        MacroCell(*a,1,arY+pc)=b[i*2+1];//y
        MacroCell(*a,2,arY+pc)=c[i];//r
        MacroCell(*a,flag+3,arY+pc)=1;
        pc++;
      }
    }//iの終わり
    /*for(i=0;i<arY*2;i++){
    for(j=0;j<arrayX;j++){
    printf("%4d ",MacroCell(*a,j,i));
    }printf("\n");
    }*/
  }//endif
  free(resul);
  return 0;
}

int mysubstr( char *t, const char *s, int pos, int len )
{
    if( pos < 0 || len < 0 || len > strlen(s) )
        return -1;
    for( s += pos; *s != '\0' && len > 0; len-- )
        *t++ = *s++;
    *t = '\0';
    return 0;
}

int cntup(int *ans,int *arrayY){
 FILE *fp;
 char *fname = "6_11.txt";
 char s[100],title[15];
 //int ans[500];
 int cnt=0,i=0,x=0,y=0;
 fp = fopen( fname, "r" );
 if( fp == NULL ){
   printf( "%sファイルが開けません\n", fname );
   return -1;
 }
 while( fgets( s, 100, fp ) != NULL ){
  if((strstr(s,".jpg"))!=NULL){
   mysubstr(title,s,0,8);
   x++;
   //printf( "%s\n",title );
  }else if((strstr(s,"-"))!=NULL){
   if(cnt>0){
 //   printf("%d\n",cnt);
    ans[i]=cnt;i++;
    cnt=0;
   }
  }else{
   cnt++;
   y++;
  }
 }ans[i]=0;i=0;
 arrayX=x;
 *arrayY=y;
 //printf("===%d== %d===\n",arrayX,*arrayY);
 for(i=0;ans[i];i++){
  //printf("%d\n",i);
 }
 fclose( fp );
 return 0;
}

int timev(int *a){
 int i=0;
 for(i=0;a[i]!=0;i++){
  printf("%d\n",a[i]);
 }
 return 0;
}

int resultsv(int *a,int *x,int *y){
 int i=0;
 for(i=0;i<(*y);i++){
  printf("%8d\n",a[i*(*x)+0]);
 }
 //printf("%d\n",i-1);
 return 0;
}

int catfinal(int **results,int y){
  int i,j;
  //printf("%d\n",x);
  for(i=0;i<MacroCell(*results,0,i)!=0;i++){
    for(j=0;j<arrayX;j++){
      printf("%8d ",MacroCell(*results,j,i));
    }printf("\n");
  }
  return 0;
}

int judg(int **results,int y){
  int i,j,bi,bj,xmi,xma,flag=0,cnt=0,av=0,bv=0;
  //printf( "judg arrayX=%d, arrayY=%d\n", arrayX, y );
  for(i=0;MacroCell(*results,0,i)!=0;i++){//引数の比較表の最大要素数
    //printf("%d\n",MacroCell(*results,0,i));
    av++;
  }//av--;
  for(i=0;MacroCell(*results,i,0)!=0;i++){//引数の比較表の最大要素数
    bv++;
  }//bv-=3;
  
  for(i=1;i<y-1;i++){
    flag=0;//ゼロクリア
    for(j=3;j<arrayX;j++){
      if(MacroCell(*results,j,i)==0){
        if(flag==0){
          xmi=j;
        }
        flag++;
      }else if(MacroCell(*results,j,i)==1){
        if(flag!=0){
          //ここに居眠り判断処理
          if((flag*15)>=180 && (flag*15)<=2685){
            xma=j-1;
            for(cnt=xmi;cnt<=xma;cnt++){
              MacroCell(*results,cnt,i)=2;
            }//endfor
          }//endif
          flag=0;
        }//endif
      }//endif
    }//endfor_j
    //最後が0で終わったときの処理
    if(MacroCell(*results,j-1,i)==0 && (flag*15)>=180 && (flag*15)<=2685){
      for(cnt=xmi;cnt<=j-1;cnt++){
        MacroCell(*results,cnt,i)=2;
      }
    }
  }
  /*//確認
  for(i=0;i<av-1;i++){
    for(j=0;j<arrayX;j++){
      printf("%4d,",MacroCell(*results,j,i));
    }printf("\n");
  }
 */ ///////
}

int fop(int **results,int y){
  FILE *fp;
  const char *fname="fop.txt";
  int i,j,av=0,bv=0,cnt=0;
  char sys[50],s[200],ss[250];
  ss[0]='\0';
  //printf( "fop arrayX=%d, arrayY=%d\n", arrayX, y );
  for(i=1;MacroCell(*results,0,i)!=0;i++){
    av++;
  }//av--;
  for(i=3;i<arrayX;i++){
    bv++;
  }//bv-=3;
 //printf("fop av=%d bv=%d\n",av,bv);
 /*for(i=0;i<bv;i++){
  for(j=0;j<av;j++){
   printf("%d ",MacroCell(*results,j,i));
  }printf("\n");
 }*/
  fp=fopen(fname,"w");
  if(fp == NULL){
    printf("%sファイルが開けません\n",fname);
    return -1;
  }
  //printf("fop av=%d bv=%d\n",av,bv);
  for(i=3;i<arrayX;i++){
    for(j=1;j<av-1;j++){
      if(MacroCell(*results,i,j)==2 && MacroCell(*results,2,j)<100){
        if(cnt==0){
          fprintf(fp,"%d",MacroCell(*results,i,0));
        }
        cnt++;
        sprintf(s,",%d,%d,%d",MacroCell(*results,0,j),MacroCell(*results,1,j),MacroCell(*results,2,j));//,x,y,r
        strcat(ss,s);
      }
    }//jend
    if(cnt>0){
      fprintf(fp,",%d",cnt);//居眠りした人数
      fprintf(fp,"%s",ss);//居眠りした座標
      fprintf(fp,"\n");
      //cnt=0;
    }
    cnt=0;
    s[0]='\0';
    ss[0]='\0';
  }
  fclose(fp);
  /*system("cp fop.txt fop.csv");
  strcpy(sys,"tail -n ");
  sprintf(s,"%d",av);
  strcat(sys,s);
  strcat(sys," fop.csv");
  printf("%s\n",sys);
  system(sys);*/
}

int main(){
   FILE *fp;
   char *fname = "6_11.txt";
   int s=0,i=0,j=0,cnt=0,t=0,tc=0,lc=0,flag=0;
   int c;
   int *p[2];
   int cnte[500];
   int a[50][2]={{0}};
   int time[300]={0};
   int *results;
   int r_c[50]={0};
   int arrayY=0;
   cntup(cnte,&arrayY);//中間リストファイルの要素数計算
   //printf( "#### arrayX=%d, arrayY=%d\n", arrayX, arrayY );
   arrayX+=3;//番兵付与(x,y,r)
   arrayY+=1;//番兵付与
   fp = fopen( fname, "r" );
     if( fp == NULL ){
       printf( "%sファイルが開けません\n", fname );
       return -1;
     }
  //最終中間データ作成できればすすめる(番兵含む)
  if((results=(int *)calloc((arrayX*arrayY),sizeof(int)))!=NULL){
     //番兵100埋め
     MacroCell(results,0,0)=1;
     MacroCell(results,1,0)=1;
     MacroCell(results,2,0)=1;
     //中間リストデータ読み込み
     while((c=fgetc(fp)) != EOF ){
        if((c-'0')>=0){
         s=(10*s)+(c-'0');
        }
        //画像名の時は無視
        if(c=='.'){
	 if((++tc)-(tc-1)==1){
         //printf("main2 MacroCell(results,tc+2,0)=%d\n",MacroCell(results,tc+2,0));
         time[tc]=s;
	  if(tc+2<arrayX){
            MacroCell(results,tc+2,0)=s;
	  }
	 }//printf("main2 tc=%d arrayX=%d\n",tc,arrayX);
        }
        if(c=='g'){
         s=0;
        }else if(((isspace(c)) || (c=='-')) && s>0){//一区切りついた場合の処理
           if(++i==1){
            a[j][0]=s;
              if(cnt==0){
               MacroCell(results,0,++lc)=s;
              }
            s=0;
            a[j+1][0]=0;
           }else if(i==2){
            a[j][1]=s;
              if(cnt==0){
               MacroCell(results,1,lc)+=s;
              }
            s=0;
            a[j+1][1]=0;
           }else if(i==3){//半径
              if(cnt==0){
               MacroCell(results,2,lc)+=s;
              }
            r_c[j]=s;
            j++;
            r_c[j]=0;
            s=0;
            i=0;
           }
         //printf("%d\n",cnt);
        }else if(c=='-'){
           if(cnt==0){
             MacroCell(results,0,lc+1)=0;
           }
         cnt++;
         j=0;
         i=0;
         p[0]=results;
         p[1]=(int *)a;
         kyo(&p[0],p[1],r_c,&results,flag);
         flag++;
        }
     }
     fclose( fp );
     judg(&results,arrayY);
     catfinal(&results,arrayY);
     fop(&results,arrayY);
     free(results);
  }
   return 0;
}
