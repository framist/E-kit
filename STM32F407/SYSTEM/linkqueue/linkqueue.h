#ifndef __LINKQUEUE_H
#define __LINKQUEUE_H

#include <stdlib.h>
#include <string.h>

//节点的数据类型仅用到为字符串的情况
typedef char* queueData_t;


typedef struct __LinkNode {
    queueData_t data;
 	struct __LinkNode *next;
} LinkNode;


typedef struct __linkQueue { 
    LinkNode *front, *rear;
} linkQueue;

void LQ_init(linkQueue *q);
int LQ_IsEmpty(linkQueue *q);
void LQ_Enqueue(linkQueue *q, queueData_t data);
int LQ_Dequeue(linkQueue *q, queueData_t *data);
int LQ_Length(linkQueue *q);

#endif

/*************************** End of file ****************************/
