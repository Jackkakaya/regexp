#ifndef QUEUE
#define QUEUE
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 1000
/*
将数组封装成队列
含有方法：
pop:  出队列
create: 创建队列
push:  压入队列
clear: 清空队列
destory: 销毁队列
gethead: 获取头部元素
length: 获取长度
*/

typedef struct
{
	char* element;
	int count;
	int rear;
} Queue;

void create(Queue* elem) {
	(*elem).rear=0;
	(*elem).count = 0;
	(*elem).element = (char*)malloc(MAXSIZE*sizeof(char));
}

void push(Queue* elem, char data) {
	(*elem).element[(*elem).count] = data;
	(*elem).count++;
}

void pop(Queue* elem) {
	(*elem).element[(*elem).count] = '\0';
	(*elem).count--;
}
void clear(Queue* elem) {
	while ((*elem).count != 0) pop(elem);
	(*elem).rear = 0;
}

char gethead(Queue* elem) {
	char res = (*elem).element[(*elem).rear];
	(*elem).rear++;
	(*elem).count--;
	return res;
}
void destroy(Queue* elem) {
	clear(elem);
	free((*elem).element);
	free(elem);
}
int length(Queue* elem) {
	return (*elem).count;
}
void copy(Queue* elem,Queue* target){
	for(int i=0;i<(*elem).count;i++){
		(*target).element[i] = (*elem).element[i];
	}
	(*target).count = (*elem).count;
	(*target).rear = (*elem).rear;
}
#endif