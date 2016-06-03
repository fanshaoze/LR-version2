#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED
#define  AND      1
#define  ARRAY    2
#define  BEGIN    3
#define  CASE     4
#define  CONST    5
#define  DIV      6
#define  DO       7
#define  DOWNTO   8
#define  ELSE     9
#define  END      10
#define  FILEp     11
#define  FOR      12
#define  FUNC     13
#define  GOTO     14
#define  IF       15
#define  IN       16
#define  LABEL    17
#define  MOD      18
#define  NIL      19
#define  NOT      20
#define  OF       21
#define  OR       22
#define  PACKED   23
#define  PROC     24
#define  PROG     25
#define  RECORD   26
#define  REPEAT   27
#define  SET      28
#define  THEN     29
#define  TO       30
#define  TYPE     31
#define  UNTIL    32
#define  VAR      33
#define  WHILE    34
#define  WITH     35
#define  ID       36
#define  INT      37
#define  REAL_NUM     38
#define  STRING_C   39
#define  PLUS     40
#define  MINUS    41
#define  MULTI    42
#define  RDIV     43
#define  EQ       44
#define  LT       45
#define  GT       46
#define  LE       47
#define  GE       48
#define  NE       49
#define  LR_BRAC  50
#define  RR_BRAC  51
#define  COMMA    52
#define  P_MARK   53
#define  F_STOP   54
#define  RANGE    55
#define  COLON    56
#define  ASSIGN   57
#define  SEMIC    58
#define  CAP      59
#define  EXP      60
#define  LS_BRAC  61
#define  RS_BRAC  62
#define  Q_MARK   63
#define  INTEGER   64
#define  REAL      65
#define  STRING   66
#include "define1.h"

void identify_id(int* t,char** ch)
{
    int i;
    for(i=1;i<=66;i++)
    {

        if(strcmp(key_words[i],*ch)==0)
        {
            *t=i;
            *ch="\0";
            break;
        }
    }
}
int token_scan(FILE* fp,Token* token)
{
    char ch;
    char token_data[50];
    int flag;
    int flag1,flag2;
    flag1=0;
    flag2=0;
    int tokenCount=-1,ending;
    ch=getc(fp);
    fseek(fp, -1L, SEEK_CUR);

    while(ch!=EOF)
    {

        memset(token_data,0,20);
        ch=getc(fp);


        while(ch==' '||ch=='\t'||ch=='\n' || ch=='{'||ch=='}'||flag2==1)
        {
            if (ch=='{')
                flag2=1;
            if (ch=='}')
                flag2=0;
            ch=getc(fp);
        }
        if(ch!=EOF)
            tokenCount++;
        if(ch=='\'')
        {

             ch=getc(fp);
            while(ch!='\''&& ch!=EOF)
            {

                token_data[ending]=ch;
                ending++;
                ch=getc(fp);
            }
            token[tokenCount].value.c=(char*)malloc(sizeof(char)*ending);
            strcpy(token[tokenCount].value.c,token_data);
            token[tokenCount].type=STRING_C;
        }

        else if(isalpha(ch)){
            ending=0;
            token_data[ending]=ch;

            ch=getc(fp);


            while(isalnum(ch)){
                ending++;
                token_data[ending]=ch;
                ch=getc(fp);

            }
            ending++;
            token_data[ending]='\0';
            ending++;
            fseek(fp, -1L, SEEK_CUR);
            token[tokenCount].type=ID;
            token[tokenCount].value.c=(char*)malloc(sizeof(char)*ending);
            strcpy(token[tokenCount].value.c,token_data);
            identify_id(&token[tokenCount].type, &token[tokenCount].value);
        }
        else{
            if(isdigit(ch)){
                ending=0;
                token_data[ending]=ch;
                ch=getc(fp);

                ending++;
                flag=0;

                while(isdigit(ch)||(ch=='.' && flag==0)){
                    token_data[ending]=ch;
                    if (ch=='.')
                        flag=1;
                    ch=getc(fp);
                    ending++;}
                fseek(fp, -1L, SEEK_CUR);
                token_data[ending]='\0';
                if(flag==1){
                    //printf("%s ",token_data);

                    token[tokenCount].type=REAL_NUM;
                    token[tokenCount].value.f=atof(token_data);
                    //printf("%f \n",token[tokenCount].value.f);
                }
                else{
                    token[tokenCount].type=INT;
                    token[tokenCount].value.i=atoi(token_data);
                    //printf("%d \n",token[tokenCount].value.i);
                }


            }
             else{
                ending=0;
                token_data[ending]=ch;
                 switch(ch){

                    case '*':ch=getc(fp);
                            if(ch=='*')
                            {
                                ending++;
                                token_data[ending]=ch;
                                token[tokenCount].type=EXP;
                                token[tokenCount].value.c='\0';

                            }
                            else{
                                fseek(fp, -1L, SEEK_CUR);
                                token[tokenCount].type=MULTI;
                                token[tokenCount].value.c='\0';
                            }break;
                    case ':':ch=getc(fp);
                            if(ch=='=')
                            {
                                ending++;
                                token_data[ending]=ch;
                                token[tokenCount].type=ASSIGN;
                                token[tokenCount].value.c='\0';

                            }
                            else{
                                fseek(fp,-1L,SEEK_CUR);
                                token[tokenCount].type=COLON;
                                token[tokenCount].value.c='\0';

                            }break;

                    case '<':ch=getc(fp);
                            if(ch=='=')
                            {
                                ending++;
                                token_data[ending]=ch;
                                token[tokenCount].type=LE;
                                token[tokenCount].value.c='\0';

                            }
                            else if(ch=='>')
                            {
                                ending++;
                                token_data[ending]=ch;
                                token[tokenCount].type=NE;
                                token[tokenCount].value.c='\0';

                            }
                            else{
                                fseek(fp,-1L,SEEK_CUR);
                                token[tokenCount].type=LT;
                                token[tokenCount].value.c='\0';

                            }break;
                    case'=':
                            token[tokenCount].type=EQ;
                            token[tokenCount].value.c='\0';

                            break;
                    case '>':ch=getc(fp);
                            if(ch=='=')
                            {
                                ending++;
                                token_data[ending]=ch;
                                token[tokenCount].type=GE;
                                token[tokenCount].value.c='\0';
                                //token_data=">=";
                            }
                            else{
                                fseek(fp,-1L,SEEK_CUR);
                                token[tokenCount].type=GT;
                                token[tokenCount].value.c='\0';
                                //token_data=">";
                            }break;
                    case'+':
                        token[tokenCount].type=PLUS;
                        token[tokenCount].value.c='\0';
                        //token_data="+";
                        break;
                    case'-':
                        token[tokenCount].type=MINUS;
                        token[tokenCount].value.c='\0';
                        //token_data="-";
                        break;
                    case'/':
                        token[tokenCount].type=DIV;
                        token[tokenCount].value.c='\0';
                        //token_data="/";
                        break;
                    case',':
                        token[tokenCount].type=COMMA;
                        token[tokenCount].value.c='\0';
                        //token_data=",";
                        break;
                    case';':
                        token[tokenCount].type=SEMIC;
                        token[tokenCount].value.c='\0';

                        break;
                    case'(':
                        token[tokenCount].type=LR_BRAC;
                        token[tokenCount].value.c='\0';

                        break;
                     case')':
                        token[tokenCount].type=RR_BRAC;
                        token[tokenCount].value.c='\0';

                        break;
                    case'[':
                        token[tokenCount].type=LS_BRAC;
                        token[tokenCount].value.c='\0';

                        break;
                    case']':
                        token[tokenCount].type=RS_BRAC;
                        token[tokenCount].value.c='\0';
                        break;
                    case'.':
                        token_data[ending]=ch;
                        ch=getc(fp);
                        if(ch=='.')
                        {
                            ending++;
                            token_data[ending]=ch;
                            token[tokenCount].type=RANGE;
                            token[tokenCount].value.c='\0';
                        }
                        else
                        {
                            fseek(fp,-1L,SEEK_CUR);
                            token[tokenCount].type=F_STOP;
                            token[tokenCount].value.c='\0';
                        }
                        break;

                    default:break;
                 }
             }
            }
        }
        printf("tokecount=%d ",tokenCount);
    return tokenCount;
}


#endif // DEFINE_H_INCLUDED

