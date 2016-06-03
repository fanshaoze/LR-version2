#include <stdio.h>
#include <stdlib.h>


#include "tokens.h"
#include "structs.h"

#define N 1000

Stack  stack1[100];
int stack_size = 100;

table tables[200][200];//table表
int Goto[200][200];//goto表

pexp p[100];
int count;//产生式的个数
pexp_state* state; //   int exp; int state;
s_char end_char, nend_char;//    char ch[50][10]; int length;
entire_projects LR;//    projects pro[200]; int number;

				   //终结符非终结符函数：
int is_in(s_char an, char *ch);                  //检查字符串是否在终结符或者非终结符中
void get_t();                                   //得到终结符
void get_v();                                   //得到非终结符
												//文件读取函数
void read_pro(FILE* fp);                        //在文件中读取产生式
												//展望符操作函数
s_char insert_ch(s_char an, char *ch);//
s_char search_t(char *ch);//
int find_char(s_char an, char* ch);              //在展望符集合中查找某一个展望符
s_char combine(s_char s1, s_char s2);            //链接两个展望符集合
												 //first集合函数
s_char first(char *ch);                         //求first集合
s_char firsts(s_char a);                        //整合左右first集合
												//项目操作函数
int is_initems(projects ans, item element);      //查找是否在项目集中
projects item_insert(projects ans, item ite);    //项目集中插入一个
												 //项目集合操作函数
projects closure(projects r);                   //closure函数
projects GO(projects I, char* X);                //GO函数求下一个状态
												 //
entire_projects pro_insert(entire_projects s, projects pro);//所有状态集中插入一个项目集
int is_inprojects(entire_projects an, projects x);//查找某一个项目集是否在状态集中
int find_project(entire_projects s, projects p);//查找某一个项目集

entire_projects Enpro();//

						//分析函数
void pro_table();//构造分析表
int analyze();//LR分析

int main()
{
	//词法分析
	int k, i, j, state_count;
	//语法分析
	FILE *fp;
	s_char s, s1;
	char *ch1;
	item sample;
	projects an;
	if ((fp = fopen("test.c", "r")) == NULL) printf("File cannot be opened/n");

	read_pro(fp);
	fclose(fp);
	state_count = 0;
	//printf("11111%s\n",p[0].S);
	for (i = 0; i<count; i++)
	{
		state_count = (p[i].count + 1) + state_count;
		printf("%s->", p[i].S);
		for (j = 0; j<p[i].count; j++) printf("%s", p[i].right[j]);
		printf("\n");
	}
	get_v();
	get_t();
	//printf("here");
	entire_projects r;
	//printf("%d ",k);
	r = Enpro();//求当前的状态集
	printf("r=%d\n", r.number);

	pro_table();
	k = analyze();//分析
	printf("k=%d\n", k);
	//printf("Hello world!\n");
	//printf("%d",search_t("A").length);
	printf("here1");
	//an=closure(sample);
	return 0;
}
int is_in(s_char an, char *ch)//函数功能：检查字符串是否在终结符或者非终结符中
{
	int i, flag;
	flag = 0;
	for (i = 0; i<an.length; i++)
	{
		if (strcmp(an.ch[i], ch) == 0)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 1) return 1;
	return 0;
}
void get_t()           //得到终结符
{
	int i, char_count, j, k, n, flag;
	char temp[30][10];
	k = 0;
	for (i = 0; i<count; i++)
	{
		for (j = 0; j<p[i].count; j++)
		{
			flag = 0;
			for (n = 0; n<k; n++)
			{
				if (strcmp(temp[n], p[i].right[j]) == 0)
				{
					flag = 1;
					break;
				}//重复项忽略
			}
			for (n = 0; n<nend_char.length; n++)
			{
				if (strcmp(nend_char.ch[n], p[i].right[j]) == 0)
				{
					flag = 1;
					break;
				}//非终结符忽略
			}
			if (flag == 0)
			{
				strcpy(temp[k], p[i].right[j]);
				if (strlen(temp[k]) != 0) k++;
			}
		}
	}
	strcpy(temp[k], "#");//吧#放到非终结符中
	k++;
	end_char.length = k;
	//end_char.ch=(char **)malloc((k)*sizeof(char));
	for (i = 0; i<k; i++)
	{
		//end_char.ch[i]=(char*)malloc(10*sizeof(char));

		//printf("here\n");
	}
	for (i = 0; i<k; i++)
	{
		strcpy(end_char.ch[i], temp[i]);
		printf("%s ", end_char.ch[i]);
	}
	printf("\n");
}
void get_v()           //得到非终结符
{
	int i, char_count, j;
	char temp[30][10];
	for (i = 0; i<count; i++)
	{
		strcpy(temp[i], p[i].S);//得到所有的左部的产生式，存储
	}
	char_count = count;
	for (i = 0; i<count; i++)
	{
		for (j = 0; j<i; j++)
		{
			if (j != i)
			{
				if (strcmp(temp[i], temp[j]) == 0)
				{
					//printf("%d ",char_count);
					strcpy(temp[j], "#");
					char_count -= 1;
				}
			}
		}
	}//找到所有的重复项并且把其中一个标记为#
	nend_char.length = char_count;//确定非终结符的个数
	j = 0;
	for (i = 0; i<count; i++)
	{
		if (strcmp(temp[i], "#") != 0)
		{
			memset(nend_char.ch[j], 0, sizeof(nend_char.ch[j]));//初始化的作用？
			strcpy(nend_char.ch[j], temp[i]);
			j = j + 1;
		}
		strcpy(nend_char.ch[0], "S1");
	}
	for (i = 0; i<j; i++)
	{
		printf("%s\n", nend_char.ch[i]);
	}
}
void read_pro(FILE* fp)   //在文件中读取产生式
{
	int i, j, k, flag, count1;
	char temp[100], ch;
	count = 1;//count初始化
	while (!feof(fp))
	{
		memset(temp, 0, sizeof(temp));
		k = 0;
		ch = getc(fp);
		while (ch != EOF && ch != '\n')
		{
			temp[k] = ch;
			k++;
			ch = getc(fp);
		}
		//temp里面存储当前读取的产生式
		if (strlen(temp) == 0)
			continue;
		printf("%s\n", temp);
		for (i = 0; i<strlen(temp); i++)
		{
			if (temp[i] == '-' && temp[i + 1] == '>')
			{
				j = 0;
				for (j = 0; j<i; j++)
				{
					p[count].S[j] = temp[j];
				}
				//把前面的部分存储到产生式的左端
				p[count].S[j] = '\0';
				k = i + 2;//k当前是右部产生式的开头
				break;
			}
		}
		flag = 0;
		j = 0;
		count1 = 0;
		for (i = k; i<strlen(temp); i++)
		{
			if (temp[i] != ' ')//没有遇到空格，表示在读入一个符号
			{
				p[count].right[count1][j] = temp[i];
				j++;
			}
			else if (temp[i] == ' ')//遇到空格，表示一个符号读完了
			{
				p[count].right[count1][j] = '\0';
				count1 = count1 + 1;
				j = 0;
			}
			if (i == strlen(temp) - 1)//表示读完了一个产生式
			{
				p[count].right[count1][j] = '\0';
			}
		}
		p[count].count = count1 + 1;//右部符号的个数
		count++;
	}
	memset(p[0].S, 0, sizeof(p[0].S));
	strcpy(p[0].S, "S1");
	strcpy(p[0].right[0], p[1].S);//设置扩展的产生式
	p[0].count = 1;
	printf("%s\n", p[0].S);
	printf("%d\n", count);

}
s_char combine(s_char s1, s_char s2)
{
	int i, j, k, len, flag;
	s_char answer;
	char temp[20][10];
	len = 0;
	k = s1.length + s2.length;
	for (i = 0; i<s1.length; i++)
	{
		strcpy(temp[i], s1.ch[i]);
		len++;
	}
	for (j = 0; j<s2.length; j++)
	{
		flag = -1;
		for (k = 0; k<len; k++)
		{
			if (strcmp(s2.ch[j], temp[k]) == 0)
			{
				flag = 1;
				break;
			}
			flag = 0;
		}
		if (flag != 1)
		{
			strcpy(temp[len], s2.ch[j]);
			len++;
		}
	}
	answer.length = len;
	for (i = 0; i<len; i++)
	{
		strcpy(answer.ch[i], temp[i]);
	}
	return answer;
}
s_char insert_ch(s_char an, char *ch)
{
	s_char answer;
	int i, j;
	answer.length = an.length + 1;
	for (i = 0; i<an.length - 1; i++)
	{
		strcpy(answer.ch[i], an.ch[i]);
	}
	strcpy(answer.ch[answer.length - 1], ch);

	return answer;
}
s_char search_t(char *ch)
{
	int i, j;
	s_char answer;
	answer.length = 0;
	for (i = 0; i<count; i++)
	{
		if (strcmp(p[i].S, ch) == 0 && p[i].count == 1 && is_in(end_char, p[i].right[0]))
			answer = insert_ch(answer, p[i].right[0]);
	}
	return answer;
}
s_char first(char *ch)
{
	s_char answer, temp;
	answer.length = 0;
	int i, j, len, flag, k;
	len = 0;
	if (is_in(end_char, ch))
	{

		return insert_ch(answer, ch);
	}

	if (is_in(nend_char, ch)) {
		for (i = 0; i<count; i++)
		{
			//
			if (strcmp(p[i].S, ch) == 0)
			{

				if (is_in(end_char, p[i].right[0]) == 1)
				{
					flag = 0;
					for (j = 0; j<answer.length; j++)
					{
						if (strcmp(answer.ch[j], p[i].right[0]) == 0)
						{

							flag += 1;
							break;
						}
					}

					if (flag == 0) {
						answer = insert_ch(answer, p[i].right[0]);
					}
				}
			}
		}
		//printf("ch=%s\n",ch);
		do {
			k = answer.length;

			for (i = 0; i<count; i++)
			{
				if (strcmp(p[i].S, ch) == 0)
				{
					if (is_in(nend_char, p[i].right[0]) == 1 && strcmp(ch, p[i].right[0]))
					{
						//printf("AN %s anlen=%d  an2len=%d ",p[i].right[0],answer.length,first(p[i].right[0]).length);
						answer = combine(answer, first(p[i].right[0]));
					}
				}
			}
			for (i = 0; i<count; i++)
			{
				if (strcmp(p[i].S, ch) == 0)
				{
					for (j = 0; j<p[i].count; j++)
					{
						if (is_in(nend_char, p[i].right[j]) && is_in(search_t(p[i].right[j]), "$") && j<p[i].count - 1)
							answer = combine(answer, first(p[i].right[j + 1]));
						if (is_in(search_t(p[i].right[j]), "$") == 0) break;
					}
				}
			}
			//printf("answer.lenght=%d\n",answer.length);
		} while (answer.length>k);
		//printf("over\n");

		return answer;
	}
}
s_char firsts(s_char a)
{
	int i, j, k;
	s_char temp;
	temp = first(a.ch[0]);
	k = 0;
	while (is_in(first(a.ch[k]), "$") == 1 && k<a.length - 1)
	{
		temp = combine(temp, first(a.ch[k + 1]));
		k++;
	}
	return temp;
}
int is_initems(projects ans, item element)
{
	int i;
	for (i = 0; i<ans.length; i++)
	{
		if (ans.items[i].state.exp == element.state.exp &&strcmp(ans.items[i].a, element.a) == 0 && ans.items[i].state.state == element.state.state)
			return 1;
	}
	return 0;
}
projects item_insert(projects ans, item ite)
{//每一个都操作一遍，，，，，
	int i;
	projects answer;
	answer.length = 0;
	for (i = 0; i<ans.length; i++)
	{
		answer.items[i].state = ans.items[i].state;
		strcpy(answer.items[i].a, ans.items[i].a);
		answer.length++;
		//printf("hello\n");
	}
	//printf(" %d\n",num);
	answer.items[answer.length].state = ite.state;
	strcpy(answer.items[answer.length].a, ite.a);
	answer.length++;

	return answer;
}
projects closure(projects r)
{
	int i, j, k, index, sta, length, last, index1, flag[100], flag1;
	item temp;
	s_char an;
	for (i = 0; i<100; i++)
		flag[i] = 0;
	projects answer;
	answer = r;
	//printf("answer=%d\n",answer.length);
	//printf("hello%d\n",answer.length);
	i = 0;
	do
	{
		flag1 = 0;
		flag[i] = 1;
		last = answer.length;
		index = answer.items[i].state.exp;
		sta = answer.items[i].state.state;
		if (sta<p[index].count && is_in(nend_char, p[index].right[sta]) == 1)
		{//假如还没有扫描到产生式的尾部，并且当前的这个产生式是非终结符
		 //printf("sss=%s %d %d\n",p[index].right[sta],p[index].count,sta);
			an.length = 0;
			if ((sta + 1)<p[index].count)
			{//当前的是倒数第二个之前的
				an = first(p[index].right[sta + 1]);
			}//找到下一个的first集合
			 //printf("length=%d\n",an.length);
			if (an.length == 0 || (is_in(an, "$") == 1))
			{//an当前是空的或者到结束了
				strcpy(an.ch[an.length], answer.items[i].a);
				an.length++;
			}

			for (j = 0; j<count; j++)
			{
				if (strcmp(p[j].S, p[index].right[sta]) == 0)//扫描每个非终结符号和当前看到的这个右部的符号
				{
					index1 = j;
					//printf("index=%d ",j);
					for (k = 0; k<an.length; k++)
					{
						strcpy(temp.a, an.ch[k]);
						temp.state.exp = index1;
						temp.state.state = 0;
						//printf("temp=%d %d %s",temp.state.exp,temp.state.state,temp.a);

						if (is_initems(answer, temp) == 0) {
							//printf("is_in ");
							answer = item_insert(answer, temp);
						}
					}
				}
			}

		}
		for (j = 0; j<answer.length; j++)
		{
			if (flag[j] == 0)
			{
				i = j;
				flag1 = 1;
			}
		}
	} while (flag1 == 1);
	/*for(i=0;i<answer.length;i++)
	{
	printf("%d %d %s\n",answer.items[i].state.exp,answer.items[i].state.state,answer.items[i].a);
	}*/
	return answer;
}

projects GO(projects I, char* X)
{//求法：获取第一个项目（状态和展望符），然后求closure
	projects J;
	J.length = 0;
	char temp[10];
	int i, j, k, index, sta;

	for (i = 0; i<I.length; i++)
	{
		index = I.items[i].state.exp;
		sta = I.items[i].state.state;
		if (sta<p[index].count && strcmp(p[index].right[sta], X) == 0)
		{//点还没有移动到最后并且点的位置和X是匹配的，
		 //printf("X=%s",X);
			I.items[i].state.state++;//点往后挪动一位
			J = item_insert(J, I.items[i]);//新的J中添加这个项目
		}
	}
	//printf("J.length=%d ",J.length);
	if (J.length == 0)
		return J;
	return closure(J);//返回closure集合
}
entire_projects pro_insert(entire_projects s, projects pro)
{
	int i, j, k, num, len;
	s.number = s.number + 1;
	num = s.number;
	//answer.pro=(projects*)malloc(num*sizeof(projects));
	len = pro.length;
	//printf("len=%d\n",len);
	//answer.pro[num-1].items=(item*)malloc(len*sizeof(item));
	i = num - 1;
	s.pro[i].length = pro.length;
	for (j = 0; j<len; j++)
	{
		s.pro[i].items[j].state = pro.items[j].state;
		strcpy(s.pro[i].items[j].a, pro.items[j].a);
	}
	return s;
}
int is_inprojects(entire_projects an, projects x)
{
	int i, j, k, flag;
	for (i = 0; i<an.number; i++)
	{
		flag = 0;
		for (j = 0; j<x.length; j++)
		{
			if (is_initems(an.pro[i], x.items[j]) == 1)
				flag++;
		}
		if (flag == x.length && x.length == an.pro[i].length)
			return 1;
	}
	return 0;
}
entire_projects Enpro()
{
	//printf("here is\n");
	projects answer, temp, prt;//定义三种状态
	s_char temp1;
	entire_projects g;

	g.number = 0;

	int i, j, k, index, sta, last;

	temp.length = 1;
	answer.length = 0;
	//    temp.items=(item*)malloc(sizeof(item));
	temp.items[0].state.exp = 0;
	temp.items[0].state.state = 0;//设置第一个点的位置

	strcpy(temp.items[0].a, "#");

	answer = closure(temp);

	g = pro_insert(g, answer);//添加一个闭包
	do
	{//每一项都计算并返回闭包集合
		last = g.number;
		for (i = 0; i<g.number; i++)
		{
			temp1.length = 0;
			for (j = 0; j<g.pro[i].length; j++)
			{
				sta = g.pro[i].items[j].state.state;
				index = g.pro[i].items[j].state.exp;
				if (sta<p[index].count) prt = GO(g.pro[i], p[index].right[sta]);
				//printf("prt.num=%d\n",prt.length);
				if (prt.length>0 && is_inprojects(g, prt) == 0) g = pro_insert(g, prt);
				//
				if (g.number>200)
				{
					printf("stack overflow");
					return g;
				}
			}

		}
	} while (last<g.number);
	return g;
}
int find_project(entire_projects s, projects p)
{
	int index, sta, index1, sta1, i, k, n, flag;
	n = s.number;
	for (k = 0; k<n; k++)
	{
		flag = 0;
		for (i = 0; i<p.length; i++)
		{
			index = p.items[i].state.exp;
			index1 = s.pro[k].items[i].state.exp;
			sta = p.items[i].state.state;
			sta1 = s.pro[k].items[i].state.state;
			if (index == index1 && sta == sta1 && strcmp(s.pro[k].items[i].a, p.items[i].a) == 0) flag++;
			else break;
		}
		if (flag == p.length&&s.pro[k].length == p.length) return k;
	}
	return -1;
}
int find_char(s_char an, char* ch)
{
	int i, k;
	for (i = 0; i<an.length; i++)
	{
		if (strcmp(an.ch[i], ch) == 0)
			return i;
	}
	return -1;
}
void pro_table()       //构造分析表
{
	entire_projects r;
	projects pr;
	pr.length = 0;
	r.number = 0;
	int n, m, m1, i, j, k, index, sta, num, h;
	r = Enpro();
	n = r.number;
	m = nend_char.length;
	m1 = end_char.length;
	for (i = 0; i<200; i++)
	{
		for (j = 0; j<200; j++) Goto[i][j] = -1;
		for (j = 0; j<200; j++)//全部初始化为error
		{
			tables[i][j].type = 'e';
			tables[i][j].value = -1;
		}
	}
	//printf("02=%d\n",Goto[0][2]);
	for (k = 0; k<n; k++)
	{
		for (i = 0; i<r.pro[k].length; i++)
		{
			index = r.pro[k].items[i].state.exp;
			sta = r.pro[k].items[i].state.state;
			if (sta<p[index].count)
			{
				if (is_in(end_char, p[index].right[sta]) == 1)
				{
					pr = GO(r.pro[k], p[index].right[sta]);
					j = find_project(r, pr);
					num = find_char(end_char, p[index].right[sta]);
					if (j >= 0 && num >= 0)
					{
						tables[k][num].type = 's';
						tables[k][num].value = j;
					}
				}
				else if (is_in(nend_char, p[index].right[sta]) == 1)
				{
					pr = GO(r.pro[k], p[index].right[sta]);
					j = find_project(r, pr);
					num = find_char(nend_char, p[index].right[sta]);
					if (j >= 0 && num >= 0)
					{
						Goto[k][num] = j;
						//printf("num=%d %d %d\n",k,num,Goto[k][num]);
					}
				}
			}
			else if (p[index].count == sta)
			{
				if (index == 0 && strcmp(r.pro[k].items[i].a, "#") == 0)
				{
					num = find_char(end_char, "#");
					tables[k][num].type = 'a';
					tables[k][num].value = -2;
				}
				else if (index>0)
				{
					num = find_char(end_char, r.pro[k].items[i].a);
					tables[k][num].type = 'r';
					tables[k][num].value = index;
				}
			}
		}
	}
}
int analyze()
{
	char w[10][10];
	int tokcount, i, j;
	FILE *fp1;
	Token tok[N];

	if ((fp1 = fopen("test0.c", "r")) == NULL) printf("File cannot be opened/n");
	tokcount = token_scan(fp1, tok);//扫描一次得到所有的token？？？如何优化
	tokcount++;
	tok[tokcount].type = 0;
	tok[tokcount].value.i = -1;
	//printf("#=%s\n",words[tok[tokcount].type]);
	int index, n, num, h, n1, num1;
	tokcount + 1;
	//将#加入到输入串中
	strcpy(stack1[stack_size - 1].ch, "#");
	stack1[stack_size - 1].j = 0;
	stack_size--;
	i = 0;
	do
	{
		num = find_char(end_char, words[tok[i].type]);
		//printf("word=%s\n",words[tok[i].type]);
		n = stack1[stack_size].j;
		if (tables[n][num].type == 's' && tables[n][num].value >= 0)
		{
			strcpy(stack1[stack_size - 1].ch, words[tok[i].type]);
			stack1[stack_size - 1].j = tables[n][num].value;
			stack_size--;

			i++;
			continue;
		}
		else if (tables[n][num].type == 'r' && tables[n][num].value >= 0)
		{
			index = tables[n][num].value;
			printf("\nreduce to   ：%s-", p[index].S);
			for (h = 0; h<p[index].count; h++)
			{
				stack_size = stack_size + 1;
				printf("    %s ", p[index].right[h]);
			}
			printf("\n");

			strcpy(stack1[stack_size - 1].ch, p[index].S);
			num1 = find_char(nend_char, p[index].S);
			n1 = stack1[stack_size].j;
			stack1[stack_size - 1].j = Goto[n1][num1];
			stack_size--;
			continue;
		}
		else if (tables[n][num].type == 'a')
		{
			printf("reduce success\n");
			return 0;
		}
		else
		{
			printf("reduce fail…………\n");
			return -1;
		}
	} while (i <= tokcount);
	return -1;
}
