#pragma once

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// hashnode �ڵ� ���� ʵ�ʱ������ݵĵ�����
struct HashNode {
	char * key;
	char * value;
	struct HashNode * next;
};
typedef struct HashNode HashNode;


// hash �� ���� ���� hash ����
struct HashTable {
	// hashtable ��󳤶�
	int size;
	// hashtable ���г���
	int item_size;
	HashNode * head;
};
typedef struct HashTable HashTable;

// hash ����
int hash(char * key, int size);
// ��ʼ�� hashTable
HashTable * hashtable_init(int size);
// put
HashTable * hashtable_put(HashTable *hashtable, char * key, char * value);
// get
char * hashtable_get(HashTable * hashtable, char * key);
