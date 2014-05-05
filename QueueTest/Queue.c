//
//  Queue.c
//  QueueTest
//
//  Created by Joshua Rapp on 4/15/14.
//  Copyright (c) 2014 JRapp01. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

int main()
{
    int choice;
    while (1)
    {
        printf("1.Insert element to queue \n");
        printf("2.Delete element from queue \n");
        printf("3.Display all elements of queue \n");
        printf("4.Quit \n");
        printf("Enter your choice : ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                insert();
                break;
            case 2:
                delete();
                break;
            case 3:
                display();
                break;
            case 4:
                exit(1);
            default:
                printf("Wrong choice \n");
        } /*End of switch*/
    } /*End of while*/
} /*End of main()*/
