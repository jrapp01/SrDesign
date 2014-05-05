//
//  Queue.h
//  QueueTest
//
//  Created by Joshua Rapp on 4/15/14.
//  Copyright (c) 2014 JRapp01. All rights reserved.
//

#ifndef QueueTest_Queue_h
#define QueueTest_Queue_h

const int MAX  = 50;
int queue_array[MAX];
int rear = - 1;
int front = - 1;

void insert(){
    int add_item;
    if (rear == MAX - 1)
        printf("Queue Overflow \n");
    else
    {
        if (front == - 1)
        /*If queue is initially empty */
            front = 0;
        printf("Inset the element in queue : ");
        scanf("%d", &add_item);
        rear = rear + 1;
        queue_array[rear] = add_item;
    }
} /*End of insert()*/

void delete(){
    if (front == - 1 || front > rear)
    {
        printf("Queue Underflow \n");
        return ;
    }
    else
    {
        printf("Element deleted from queue is : %d\n", queue_array[front]);
        front = front + 1;
    }
} /*End of delete() */
void display(){
    int i;
    if (front == - 1)
        printf("Queue is empty \n");
    else
    {
        printf("Queue is : \n");
        for (i = front; i <= rear; i++)
            printf("%d ", queue_array[i]);
        printf("\n");
    }
} /*End of display() */

#endif
