#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRSIZE 512
#define DELTA 1024
#define BUF_SIZE 1024
#define MAP_SIZE 128
#define KEY_SIZE 64
#define VALUE_SIZE 512


typedef struct
{
	char * start;
	unsigned int length;
}node;

typedef struct
{
	node key;
	node value;
} conf_node;

typedef struct
{
	conf_node nodes[MAP_SIZE];
	unsigned int now;
} conf_map;

void appendStr(char ** des , char * src);
char * createStr(unsigned int size);
void loadConf(char **src , const char* fileName);
void print_cmap(conf_map * cmap);
void get_map(conf_map *cmap , const char * fileName);

void main()
{
	conf_map cmap;
	cmap.now = 0;
	get_map(&cmap , "config.conf");
	getchar();
}

void appendStr(char ** des , char * src)
{
	unsigned int des_size = 0;
	unsigned int src_size = 0;
	unsigned int max_size = 0;
	char * temp;

	des_size = strlen(*des);//strlen计算的时候会到'\0'结束，所以这里要尤其注意
	src_size = strlen(src);
	max_size = (des_size / STRSIZE + 1) * STRSIZE;

	if(max_size <= des_size + src_size)
	{
		while (max_size <= des_size + src_size) max_size += DELTA;
		temp = (char *)malloc(max_size);
		memcpy(temp , *des , des_size);
		memcpy(temp + des_size , src , src_size);
		temp[des_size + src_size] = '\0';
		free(*des);
		*des = temp;
	}
	else
	{
		memcpy(*des + des_size , src , src_size);
		(*des)[des_size + src_size] = '\0';
	}

}

char * createStr(unsigned int size)
{
	char * temp = (char *) malloc(size);
	temp[0] = '\0';
	return temp;
}

void loadConf(char **src , const char* fileName)
{

	char buf[BUF_SIZE] = "";
	unsigned int read_size = 0;

	FILE *file = NULL;
	if(fopen_s(&file , fileName , "r"))
	{
		printf("找不到%s\n" , fileName);
		return;
	}
	while(1)
	{
		//每次少读一个字符，最后一个字符给'\0'
		read_size = fread(buf , 1 , BUF_SIZE - 1 , file);
		buf[read_size] = '\0';
		if( BUF_SIZE - 1 > read_size )
		{
			appendStr(src , buf);
			break;
		}
		appendStr(src , buf );
	}
	fclose(file);
}

void get_map(conf_map *cmap , const char * fileName)
{
	char * src = createStr(STRSIZE);
	char * iter1 = src;
	char * iter2 = src;
	char * temp = src;
	unsigned int length = 0;
	int src_size = 0;
	loadConf(&src , fileName);

	//printf("%s" , src);

	src_size = strlen(src);
	iter1 = src;
	iter2 = src;
	while(src_size > 0 && iter1 != '\0')
	{

		//去掉注释、空字符
		if(*iter1 == '#')
			while( *( ++ iter1) != '\n' && (-- src_size) > 0);
		//if( *iter1 == ' ' || *iter1 == '	'  || *iter1 == '\n')
		if( (*iter1 < 36  || *iter1 == '\n')&& *iter1 != '#')
		{
			while(( (*iter1 < 36  || *iter1 == '\n')&& *iter1 != '#')  && (-- src_size) > 0) ++ iter1;
			continue;
		}

		cmap->nodes[cmap->now].key.start = iter1;
		length = 0;
		while(*iter1 != '=' && *iter1 >36 && (-- src_size) != 0){ ++ iter1; ++ length;}
		cmap->nodes[cmap->now].key.length = length;

		length = 0;
		while((*iter1 == '=' || *iter1 == ' ' || *iter1 == '	' ) && (-- src_size) > 0){ ++ iter1;}
		cmap->nodes[cmap->now].value.start = iter1;
		while(*iter1 != '=' && *iter1 >36 && (-- src_size) > 0){ ++ iter1; ++ length;}
		cmap->nodes[cmap->now].value.length = length;
		++ cmap->now;

	}


	printf("finally ,src_size = %d \n" , src_size );
	print_cmap(cmap);

	free(src);
}

void print_cmap(conf_map * cmap)
{
	char temp[128];
	unsigned int i;
	printf("\n");
	for(i = 0 ; i < cmap->now ; i ++ )
	{
		//temp[0] = cmap->nodes[i].key.start;
		memcpy(temp , cmap->nodes[i].key.start , cmap->nodes[i].key.length);
		temp[cmap->nodes[i].key.length] = '\0';
		printf("%s---" , temp);

		memcpy(temp , cmap->nodes[i].value.start , cmap->nodes[i].value.length);
		temp[cmap->nodes[i].value.length] = '\0';
		printf("%s\n" , temp);
	}
}


