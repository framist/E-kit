/**
 * @file linkQueue.c
 * @author framist (framist@163.com)
 * @brief 带头结点的链式队列
 * @version 0.1
 * @date 2021-12-13
 * @copyright Copyright (c) framist. All rights reserved
 * 可能存在内存泄露
 */

#include "main.h"
#include "linkqueue.h"


void LQ_init(linkQueue *q) {
    q->front = q->front = (LinkNode *)malloc(sizeof(LinkNode)); 
    q->front->next = NULL;
}


/**
 * @brief 判别队列q是否为空 
 * 
 * @param q 
 * @return int 
 * @retval 0 非空
 * @retval 1 空
 */
int LQ_IsEmpty(linkQueue *q) { 
    if (q->front==q->rear)
    	return 1; 
    else 
        return 0;
} 

/**
 * @brief  将data加入队列q的尾端
 * 
 * @param q 
 * @param data 
 */
void LQ_Enqueue(linkQueue *q, queueData_t data) {
    q->rear->next = (LinkNode *)malloc(sizeof(LinkNode)); /* 新结点插入尾端 */
    q->rear = q->rear->next;    /* 尾指针指向新结点 */
    q->rear->data = data;       /* 给新结点赋值 */
    q->rear->next = NULL;       /* 新结点的下一结点为空 */
} 

/**
 * @brief 将队头元素删除
 * 
 * @param q
 * @return queueData_t
 * @retval -1 队列为空
 * 
 */
int LQ_Dequeue(linkQueue *q, queueData_t *x) {
    if (LQ_IsEmpty(q)) {
        return -1;
    } 
    
    LinkNode *s = q->front->next;   /* 指向被删除的队头元素 */
    *x = s->data;          /* 被删除元素的值赋给x 因为是一指针所以可以直接赋值*/

    q->front->next = s->next; /* 队头指针指向被删除元素的下一结点 */
    if (q->rear == s) {
        q->rear = q->front; // 如果被删除的是队尾元素，则队尾指针指向队头
    }    
    free(s);           

    return 0; /* 返回被删除结点的值 */
    
} 

int LQ_Length(linkQueue *q) {
    int len = 0;
    LinkNode *p = q->front->next;
    while (p != NULL) {
        len++;
        p = p->next;
    }
    return len;
}


/*************************** End of file ****************************/
