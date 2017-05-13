#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct List
{
    char str[255];
    struct List *next;
};

void PrintFuckingList(struct List *head)
{
    printf("LIST : ");
    struct List* it = head;
    while (it != NULL)
    {
        printf("%s ", it->str);
        it =  it->next;
    }

}

struct List* trans(struct List *head, char temp[][255])
{
    int i =0;
    int cnt1 = 0;
    int cnt2= 0;
    struct List* tmp = head;
    struct List* one = malloc(sizeof(struct List));
    struct List* two = malloc(sizeof(struct List));
    struct List* it = tmp;
    int t = 0;
    (*one).next = (*two).next = NULL;
    /*(*one).str = temp[0];
    (*two).str = temp[1];*/
    strcpy((*one).str, temp[0]);
    strcpy((*two).str, temp[1]);
    printf("%s\n", one->str);
    printf("%s\n", two->str);
    for(i = 0;;++i)
    {
        if(temp[0][i] == '\0')
            break;
        if(temp[0][i] == ' ')
            ++cnt1;
    }
    for(i = 0;;++i)
    {
        if(temp[1][i] == '\0')
            break;
        if(temp[1][i] == ' ')
            ++cnt2;
    }
    for(i = 0;;++i)
    {
        if((*head).str[i] == '\0')
            break;
        if((*head).str[i] == ' ')
            ++t;
    }
    if(cnt2 == t)
    {
        (*two).next = head;
        tmp = two;
    }
    it = head;
    printf("%d %d\n", cnt1, cnt2);
    while(it != NULL)
    {
        PrintFuckingList(head);
        char t;
        scanf("%c", &t);

        int c_cur = 0;
        int c_nx = 0;
        int i = 0;
        printf("%s\n", it->str);
        printf("%s\n", it->next->str);
        for(i = 0;;++i)
        {
            if((*it).str[i] == '\0')
                break;
            if((*it).str[i] == ' ')
                ++c_cur;
        }
        if((*it).next != NULL)
        {
            for(i = 0;;++i)
            {
                if((*(*it).next).str[i] == '\0')
                    break;
                if((*(*it).next).str[i] == ' ')
                    ++c_nx;
            }
        }
        printf("%d %d\n", c_cur, c_nx);
        if(c_nx == cnt2)
        {
            (*two).next = (*it).next;
            (*it).next = two;
        }
        if(c_cur == cnt1)
        {
            (*one).next = (*it).next;
            (*it).next = one;
        }
        it = (*it).next;

        printf("%s\n", it->str);
        printf("%s\n", it->next->str);
        break;
    }
    return tmp;
}

int main()
{
    char c[2][255];
    struct List* head = malloc(sizeof(struct List));
    c[1][0] = 'a';
    c[1][1] = ' ';
    c[1][2] = 'z';
    c[0][0] = 'p';
    c[0][1] = c[1][3] = '\0';
    (*head).str[0] = 'z';
    (*head).str[1] = '\0';
    (*head).next = malloc(sizeof(struct List));
    (*(*head).next).str[0] = 'o';
    (*(*head).next).str[1] = ' ';
    (*(*head).next).str[2] = 'h';
    (*(*head).next).str[3] = '\0';
    (*(*head).next).next = NULL;
    head = trans(head, c);
    printf("%s\n", (*(*(*head).next).next).str);
    return 0;
}
