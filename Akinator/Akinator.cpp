#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Node
{
    bool type; // 0 - это вопрос, 1 - это ответ
    char* data;
    struct Node* left; // ответ нет
    struct Node* right; // ответ да
    bool exists;
    int x, y;
};

void push(Node** node, Node* newNode);

void writeNodeToFile(FILE* file, Node* node) {
    // Записываем значение поля type в файл
    fwrite(&node->type, sizeof(bool), 1, file);

    // Записываем значение поля data в файл
    size_t data_len = strlen(node->data) + 1; // Добавляем 1 для записи нулевого символа
    fwrite(&data_len, sizeof(size_t), 1, file); // Записываем длину строки data
    fwrite(node->data, sizeof(char), data_len, file); // Записываем саму строку data

    // Записываем значение поля exists в файл
    fwrite(&node->exists, sizeof(bool), 1, file);

    // Записываем значения полей x и y в файл
    fwrite(&node->x, sizeof(int), 1, file);
    fwrite(&node->y, sizeof(int), 1, file);
}

void readNodesFromFile(FILE* file, Node** root) {
    bool type;
    size_t data_len;
    char* data;
    bool exists;
    int x, y;
    while (fread(&type, sizeof(bool), 1, file) == 1) { // Читаем поле type из файла
        // Выделяем память под новый узел
        Node* new_node = (Node*)malloc(sizeof(Node));

        // Читаем поле data из файла
        fread(&data_len, sizeof(size_t), 1, file); // Читаем длину строки data
        data = (char*)malloc(data_len * sizeof(char)); // Выделяем память под строку data
        fread(data, sizeof(char), data_len, file); // Читаем саму строку data

        // Читаем поле exists из файла
        fread(&exists, sizeof(bool), 1, file);

        // Читаем поля x и y из файла
        fread(&x, sizeof(int), 1, file);
        fread(&y, sizeof(int), 1, file);

        // Инициализируем новый узел
        new_node->type = type;
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->exists = exists;
        new_node->x = x;
        new_node->y = y;

        // Добавляем новый узел в дерево

        //printf("%s ", new_node->data);
        push(root, new_node);
    }
}


/* ------------------------------------------------------- */

void DFS(Node* node, FILE* file)
{
    if (node->left != NULL)
        DFS(node->left, file);

    writeNodeToFile(file,node);

    if (node->right != NULL)
        DFS(node->right, file);
}

void saveTree(Node* node)
{
    FILE* file = fopen("D:\\help.bin", "wb");
    if (file == NULL) {
        printf("err oprn file");
        exit(1);
    }

    DFS(node, file);

    fclose(file);
}


void push(Node** node, Node* newNode)
{
    if ((*node) == NULL)
    {
        *node = newNode;
    }

    if ((*node)->y > newNode->y)
    {
        if ((*node)->x > newNode->x)
        {
            if ((*node)->left == NULL)
            {
                Node* newNode0 = (Node*)malloc(sizeof(Node));
                newNode0->data = (char*)malloc(strlen(newNode->data) + 1);
                strcpy(newNode0->data, newNode->data);
                newNode0->exists = 1;
                newNode0->left = NULL;
                newNode0->right = NULL;
                newNode0->type = newNode->type;
                newNode0->x = newNode->x;
                newNode0->y = newNode->y;
                (*node)->left = newNode0;
                return;
            }

            push(&(*node)->left, newNode);          
        }

        if ((*node)->x < newNode->x)
        {
            if ((*node)->right == NULL)
            {
                Node* newNode0 = (Node*)malloc(sizeof(Node));
                newNode0->data = (char*)malloc(strlen(newNode->data) + 1);
                strcpy(newNode0->data, newNode->data);
                newNode0->exists = 1;
                newNode0->left = NULL;
                newNode0->right = NULL;
                newNode0->type = newNode->type;
                newNode0->x = newNode->x;
                newNode0->y = newNode->y;
                (*node)->right = newNode0;
                return;
            }
            push(&(*node)->right, newNode);
        }

    }

    if ((*node)->y < newNode->y)
    {
        if ((*node)->x < newNode->x)
        {
            Node* newNode0 = (Node*)malloc(sizeof(Node));
            newNode0->data = (char*)malloc(strlen((*node)->data) + 1);
            strcpy(newNode0->data, (*node)->data);
            newNode0->exists = 1;
            newNode0->left = (*node)->left;
            newNode0->right = (*node)->right;
            newNode0->type = (*node)->type;
            newNode0->x = (*node)->x;
            newNode0->y = (*node)->y;

            (*node)->data = (char*)malloc(strlen(newNode->data) + 1);
            strcpy((*node)->data, newNode->data);
            (*node)->left = newNode0;
            if (newNode0->right != NULL)
            {
                if (newNode0->right->x > newNode->x)
                {
                    (*node)->right = newNode0->right;
                    newNode0->right = NULL;
                }
                else
                {
                    (*node)->right = NULL;
                }
            }
            else
            {
                (*node)->right = NULL;
            }

            (*node)->type = newNode->type;
            (*node)->x = newNode->x;
            (*node)->y = newNode->y;
            return;
        }

        if ((*node)->x > newNode->x)
        {
            Node* newNode0 = (Node*)malloc(sizeof(Node));
            newNode0->data = (char*)malloc(strlen((*node)->data) + 1);
            strcpy(newNode0->data, (*node)->data);
            newNode0->exists = 1;
            newNode0->left = (*node)->left;
            newNode0->right = (*node)->right;
            newNode0->type = (*node)->type;
            newNode0->x = (*node)->x;
            newNode0->y = (*node)->y;

            (*node)->data = (char*)malloc(strlen(newNode->data) + 1);
            strcpy((*node)->data, newNode->data);
            (*node)->right = newNode0;
            if (newNode0->left != NULL)
            {
                if (newNode0->left->x < newNode->x)
                {
                    (*node)->left = newNode0->left;
                    newNode0->left = NULL;
                }
                else
                {
                    (*node)->left = NULL;
                }
            }
            else
            {
                (*node)->left = NULL;
            }
            (*node)->type = newNode->type;
            (*node)->x = newNode->x;
            (*node)->y = newNode->y;
            return;
        }
    }

}

void readStr(char** str);

void readStr(char** str)
{
    *str = (char*)malloc(100 * sizeof(char));
    if (*str == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        exit(1);
    }
    fgets(*str, 100, stdin);
    printf("\n");

    int len = strlen(*str);
    if ((*str)[len - 1] == '\n') {
        (*str)[len - 1] = '\0';
    }
}

void addQuestion(Node** node, char* question)
{
    *node = (Node*)malloc(sizeof(Node));
    (*node)->type = 0;
    (*node)->data = (char*)malloc(strlen(question) + 1);
    strcpy((*node)->data, question);
    (*node)->exists = true;
    (*node)->x = 0;
    (*node)->y = 10000000;
}

void addAnswer(Node* node, char* answer0, char* answer1)
{
    Node* newNode0 = (Node*)malloc(sizeof(Node));
    Node* newNode1 = (Node*)malloc(sizeof(Node));

    if (newNode0 == NULL || newNode1 == NULL)
    {
        printf("Ошибка выделения памяти.\n");
        return;
    }

    newNode0->type = 1;
    newNode0->data = (char*)malloc(strlen(answer0) + 1);
    strcpy(newNode0->data, answer0);
    newNode0->left = NULL;
    newNode0->right = NULL;
    newNode0->exists = true;
    newNode0->x = node->x - 1;
    newNode0->y = node->y - 1;

    node->left = newNode0;

    newNode1->type = 1;
    newNode1->data = (char*)malloc(strlen(answer1) + 1);
    strcpy(newNode1->data, answer1);
    newNode1->left = NULL;
    newNode1->right = NULL;
    newNode1->exists = true;
    newNode1->x = node->x + 1;
    newNode1->y = node->y - 2;

    node->right = newNode1;
}

void freeTree(Node* node)
{
    if (node == NULL)
    {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);

    if (node->data != NULL)
    {
        free(node->data);
    }

    free(node);
}

void replacementAnswer(Node* node)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("What did you wish for?\n");
    char* ans1 = NULL;
    readStr(&ans1);

    printf("Enter the question that was missing:\n");
    char* que = NULL;
    readStr(&que);

    char* ans0 = (char*)malloc(strlen(node->data) + 1);
    strcpy(ans0, node->data);

    node->type = 0;
    node->data = (char*)malloc(strlen(que) + 1);
    strcpy(node->data, que);

    
    addAnswer(node, ans0, ans1);
}

void DFSNum(Node* node, int* x) 
{

    if (node->left != NULL)
    {

        DFSNum(node->left,x);
    }

    node->x = *x;
    (*x)++;

    if (node->right != NULL)
    {
        DFSNum(node->right,x);
    }
}

void BFSNum(Node* node, int* y)
{
    node->y = *y;
    (*y)--;
    
    if (node->left != NULL)
        BFSNum(node->left, y);

    if (node->right != NULL)
        BFSNum(node->right, y);

}

int main()
{
    Node* node = NULL;
    FILE* file = fopen("D:\\help.bin", "rb");

    if (file != NULL)
    {
        readNodesFromFile(file, &node);
        fclose(file);
    }

    Node* node2 = NULL;
    char* str = NULL;
    char* str1 = NULL;
    int c;

    int num;
    while (1)
    {
        printf("1 - exit; 0 - continue: ");
        scanf("%d", &num);
        if (num)
            break;

        node2 = node;


        while (1)
        {
            system("cls");
            if (node == NULL)
            {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Enter question: ");
                readStr(&str);
                addQuestion(&node, str);

                printf("Answer if no: ");
                readStr(&str);
                printf("Answer if yes: ");
                readStr(&str1);
                addAnswer(node, str, str1);
                break;
            }

            if (node2->type == 0)
            {
                printf("%s\n", node2->data);
                printf("1 - yes; 0 - no: ");
                scanf("%d", &num);
                if (num == 1)
                    node2 = node2->right;
                if (num == 0)
                    node2 = node2->left;
                continue;
            }

            if (node2->type == 1)
            {
                printf("You made a wish %s?\n", node2->data);
                printf("\n1 - yes; 0 - no: ");
                scanf("%d", &num);

                if (num == 1)
                {
                    printf("Good\n");
                    break;
                }

                if (num == 0)
                {
                    replacementAnswer(node2);
                    break;

                }
            }

        }

    }
    int x = INT_MIN;
    int y = INT_MAX;

    DFSNum(node, &x);
    BFSNum(node, &y);

    saveTree(node);
    node2 = NULL;
    free(node2);
    freeTree(node);
    free(str);
    free(str1);
    return 1;
}