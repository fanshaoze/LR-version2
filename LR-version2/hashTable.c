#include "hashTable.h"

// hash method
int hash(char * key, int size) {
	int rect = 0;
	while (*key != '\0') {
		rect += *key;
		key++;
	}
	return rect%size;
}

// init hashtable
HashTable * hashtable_init(int size) {

	HashTable * hashtable = (HashTable*)calloc(1, sizeof(HashTable));
	hashtable->size = size;
	hashtable->item_size = 0;

	// ����󳤶ȵĿռ�
	HashNode * head = (HashNode*)calloc(size, sizeof(HashNode));

	hashtable->head = head;

	return hashtable;
}

// put a var in hashtable
HashTable * hashtable_put(HashTable *hashtable, char * key, char * value) {

	int index = hash(key, hashtable->size);

	HashNode *hashNode = hashtable->head + index;

	while (true) {
		// ����û�е���� ���� �У����ҵ�һ������
		if (hashNode->key == NULL || *key == *(hashNode->key)) {

			if (hashNode->key == NULL) {
				hashtable->item_size = hashtable->item_size + 1;
			}

			hashNode->key = key;
			hashNode->value = value;
			return 0;
		}

		// ��ͻ
		// ��һ���ڵ㻹���ǿ�
		if (hashNode->key != NULL) {
			hashNode = hashNode->next;
			// �¸��ڵ��ǿ��ˣ�������
		}
		else {
			HashNode * newNode = (HashNode*)calloc(1, sizeof(HashNode));
			newNode->key = key;
			newNode->value = value;
			hashNode->next = newNode;
			hashtable->item_size = hashtable->item_size + 1;
			return 0;
		}
	}
}

// get a var
char * hashtable_get(HashTable * hashtable, char * key) {
	int index = hash(key, hashtable->size);
	HashNode *hashNode = hashtable->head + index;

	while (hashNode != NULL) {
		if (hashNode->key != NULL && *key == *(hashNode->key)) {
			return hashNode->value;
		}
		hashNode = hashNode->next;
	}
	return NULL;
}