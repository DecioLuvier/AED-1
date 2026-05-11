#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE ((int *)buffer)[0]
#define NEXT_ID ((int *)buffer)[1]
#define PARAM_0 ((int *)buffer)[2]
#define PARAM_1 ((int *)buffer)[3]
#define PARAM_2 ((int *)buffer)[4]
#define RETURN ((int *)buffer)[5]
#define SCANF_BUFFER (buffer + 24)
#define ROM_SIZE 78

#define MEMORY_OVERHEAD ((int)(sizeof(int) * 3))
#define MEMORY_ID(pos) (*(int *)(buffer + (pos)))
#define MEMORY_SIZE(pos) (*(int *)(buffer + (pos) + 4))
#define MEMORY_TYPE(pos) (*(int *)(buffer + (pos) + 8))
#define MEMORY_DATA(pos) (buffer  + (pos) + MEMORY_OVERHEAD)

#define INT32_TYPE 0
#define CHAR_TYPE 1
#define STRING_TYPE 2
#define ARRAY_TYPE 3

char *buffer;

#define CreateHeader(ID, TYPE, SIZE) (PARAM_0 = (ID), PARAM_1 = (TYPE), PARAM_2 = (SIZE), _createHeader())
void _createHeader(void) {
    buffer = realloc(buffer, BUFFER_SIZE + MEMORY_OVERHEAD + PARAM_2);
    MEMORY_ID(BUFFER_SIZE)   = PARAM_0;
    MEMORY_SIZE(BUFFER_SIZE) = PARAM_2;
    MEMORY_TYPE(BUFFER_SIZE) = PARAM_1;
    RETURN = BUFFER_SIZE;
    BUFFER_SIZE += MEMORY_OVERHEAD + PARAM_2;
}

#define GetMemoryData(ID) (PARAM_0 = (ID), _getMemoryData())
void _getMemoryData(void) {
    RETURN = ROM_SIZE;
    while (RETURN < BUFFER_SIZE && MEMORY_ID(RETURN) != PARAM_0) 
        RETURN += MEMORY_OVERHEAD + MEMORY_SIZE(RETURN);
}

#define HasData(ID) (PARAM_0 = (ID), _getMemoryData(), RETURN < BUFFER_SIZE)
void _hasData(void) {
    GetMemoryData(PARAM_0);
    RETURN = RETURN < BUFFER_SIZE;
}

#define RemoveData(ID) (PARAM_0 = (ID), _removeData())
void _removeData(void) {
    GetMemoryData(PARAM_0);
    PARAM_1 = MEMORY_OVERHEAD + MEMORY_SIZE(RETURN);
    memmove(buffer + RETURN, buffer + RETURN + PARAM_1, BUFFER_SIZE - (RETURN + PARAM_1));
    BUFFER_SIZE -= PARAM_1;
    buffer = realloc(buffer, BUFFER_SIZE);
    RETURN = BUFFER_SIZE;
}

#define Int(ID) (*(PARAM_0 = (ID), _int(), (int *)MEMORY_DATA(RETURN)))
void _int(void) {
    if (!HasData(PARAM_0))
        CreateHeader(PARAM_0, INT32_TYPE, sizeof(int));
    GetMemoryData(PARAM_0);
}

#define String(ID) (PARAM_0 = (ID), _string(), (char *)MEMORY_DATA(RETURN))
void _string(void) {
    if (!HasData(PARAM_0))
        CreateHeader(PARAM_0, STRING_TYPE, 1);
    GetMemoryData(PARAM_0);
}

#define SetString(ID, VALUE) (strcpy(SCANF_BUFFER, (VALUE)), PARAM_0 = (ID), _setString())
void _setString(void) {
    if (!HasData(PARAM_0)) {
        CreateHeader(PARAM_0, STRING_TYPE, (int)strlen(SCANF_BUFFER) + 1);
        GetMemoryData(PARAM_0);
        memcpy(MEMORY_DATA(RETURN), SCANF_BUFFER, (int)strlen(SCANF_BUFFER) + 1);
        return;
    }

    GetMemoryData(PARAM_0);

    if (MEMORY_TYPE(RETURN) != STRING_TYPE) {
        _removeData();
        CreateHeader(PARAM_0, STRING_TYPE, (int)strlen(SCANF_BUFFER) + 1);
        GetMemoryData(PARAM_0);
        memcpy(MEMORY_DATA(RETURN), SCANF_BUFFER, (int)strlen(SCANF_BUFFER) + 1);
        return;
    }

    buffer = realloc(buffer, BUFFER_SIZE + (int)strlen(SCANF_BUFFER) + 1 - MEMORY_SIZE(RETURN));
    memmove(MEMORY_DATA(RETURN) + (int)strlen(SCANF_BUFFER) + 1, MEMORY_DATA(RETURN) + MEMORY_SIZE(RETURN), BUFFER_SIZE - (RETURN + MEMORY_OVERHEAD + MEMORY_SIZE(RETURN)));
    BUFFER_SIZE         += (int)strlen(SCANF_BUFFER) + 1 - MEMORY_SIZE(RETURN);
    MEMORY_SIZE(RETURN)  = (int)strlen(SCANF_BUFFER) + 1;
    memcpy(MEMORY_DATA(RETURN), SCANF_BUFFER, (int)strlen(SCANF_BUFFER) + 1);
}

#define Array(ID, INDEX) (*(PARAM_0 = (ID), _array(), (int *)MEMORY_DATA(RETURN) + (INDEX)))
void _array(void) {
    if (!HasData(PARAM_0))
        CreateHeader(PARAM_0, ARRAY_TYPE, 0);
    GetMemoryData(PARAM_0);
}

#define ArrayPush(ID, VALUE) (PARAM_0 = (ID), PARAM_2 = (VALUE), _arrayPush())
void _arrayPush(void) {
    *(int *)SCANF_BUFFER = PARAM_2;
    if (!HasData(PARAM_0))
        CreateHeader(PARAM_0, ARRAY_TYPE, 0);
    GetMemoryData(PARAM_0);

    buffer = realloc(buffer, BUFFER_SIZE + (int)sizeof(int));
    memmove(MEMORY_DATA(RETURN) + MEMORY_SIZE(RETURN) + (int)sizeof(int), MEMORY_DATA(RETURN) + MEMORY_SIZE(RETURN), BUFFER_SIZE - (RETURN + MEMORY_OVERHEAD + MEMORY_SIZE(RETURN)));
    *(int *)(MEMORY_DATA(RETURN) + MEMORY_SIZE(RETURN)) = *(int *)SCANF_BUFFER;
    MEMORY_SIZE(RETURN) += (int)sizeof(int);
    BUFFER_SIZE         += (int)sizeof(int);
}

#define ArrayRemove(ID, INDEX) (PARAM_0 = (ID), PARAM_2 = (INDEX), _arrayRemove())
void _arrayRemove(void) {
    *(int *)(SCANF_BUFFER + 4) = PARAM_2;
    GetMemoryData(PARAM_0);
    memmove(
        (int *)MEMORY_DATA(RETURN) + *(int *)(SCANF_BUFFER + 4),
        (int *)MEMORY_DATA(RETURN) + *(int *)(SCANF_BUFFER + 4) + 1,
        BUFFER_SIZE - (RETURN + MEMORY_OVERHEAD + MEMORY_SIZE(RETURN)) + (MEMORY_SIZE(RETURN) - (*(int *)(SCANF_BUFFER + 4) + 1) * (int)sizeof(int))
    );
    MEMORY_SIZE(RETURN) -= (int)sizeof(int);
    BUFFER_SIZE         -= (int)sizeof(int);
    buffer = realloc(buffer, BUFFER_SIZE);
}

#define ArrayLen(ID) (PARAM_0 = (ID), _arrayLen(), *(int *)(SCANF_BUFFER + 8))
void _arrayLen(void) {
    if (!HasData(PARAM_0)) {
        *(int *)(SCANF_BUFFER + 8) = 0; 
        return; 
    }
    GetMemoryData(PARAM_0);
    *(int *)(SCANF_BUFFER + 8) = MEMORY_SIZE(RETURN) / (int)sizeof(int);
}

// ------------

#define MENU_OPTION     0
#define PEOPLE_ARRAY    1
#define ARRAY_COUNTER   2
#define SEARCH_COUNTER  3

void _adicionar(void) {
    printf("Add Person\n");

    printf("  Name  : ");
    scanf(" %[^\n]", SCANF_BUFFER);
    SetString(NEXT_ID, SCANF_BUFFER);
    ArrayPush(PEOPLE_ARRAY, NEXT_ID);
    NEXT_ID++;

    printf("  Age   : ");
    scanf(" %d", (int *)SCANF_BUFFER);
    ArrayPush(PEOPLE_ARRAY, NEXT_ID);
    Int(NEXT_ID) = *(int *)SCANF_BUFFER;
    NEXT_ID++;

    printf("  Email : ");
    scanf(" %[^\n]", SCANF_BUFFER);
    SetString(NEXT_ID, SCANF_BUFFER);
    ArrayPush(PEOPLE_ARRAY, NEXT_ID);
    NEXT_ID++;

    printf("Person added successfully!\n\n");
}

void _remover(void) {
    printf("Remove Person\n");
    printf("  Name: "); scanf(" %[^\n]", SCANF_BUFFER);
    Int(ARRAY_COUNTER) = 0;
    while (Int(ARRAY_COUNTER) < ArrayLen(PEOPLE_ARRAY)) {
        if (strcmp(String(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER))), SCANF_BUFFER) == 0) {
            RemoveData(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER)));
            RemoveData(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 1));
            RemoveData(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 2));
            PARAM_2 = Int(ARRAY_COUNTER);
            ArrayRemove(PEOPLE_ARRAY, PARAM_2);
            ArrayRemove(PEOPLE_ARRAY, PARAM_2);
            ArrayRemove(PEOPLE_ARRAY, PARAM_2);
            printf("Person removed successfully!\n\n");
            break;
        }
        Int(ARRAY_COUNTER) = Int(ARRAY_COUNTER) + 3;
    }
    RemoveData(ARRAY_COUNTER);
}

void _procurar(void) {
    printf("Search Person\n");
    printf("  Name: "); scanf(" %[^\n]", SCANF_BUFFER);
    Int(ARRAY_COUNTER) = 0;
    Int(SEARCH_COUNTER) = 0;
    while (Int(ARRAY_COUNTER) < ArrayLen(PEOPLE_ARRAY)) {
        if (strcmp(String(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER))), SCANF_BUFFER) == 0) {
            Int(SEARCH_COUNTER) = Int(SEARCH_COUNTER) + 1;
            printf(" Match #%d\n",    Int(SEARCH_COUNTER));
            printf(" Name  : %s\n",   String(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER))));
            printf(" Age   : %d\n",   Int(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 1)));
            printf(" Email : %s\n\n", String(Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 2)));
        }
        Int(ARRAY_COUNTER) = Int(ARRAY_COUNTER) + 3;
    }
    if (Int(SEARCH_COUNTER) == 0)
        printf("   No results found.\n\n");
    else if (Int(SEARCH_COUNTER) == 1)
        printf("   1 person found.\n\n");
    else
        printf("   %d people found.\n\n", Int(SEARCH_COUNTER));
}

void _mostrarTodos(void) {
    printf("All People\n");
    PARAM_0 = ARRAY_COUNTER; _int();
    *(int *)MEMORY_DATA(RETURN) = 0;
    PARAM_0 = PEOPLE_ARRAY; _arrayLen();
    PARAM_0 = SEARCH_COUNTER; _int();
    *(int *)MEMORY_DATA(RETURN) = *(int *)(SCANF_BUFFER + 8);

    if (Int(SEARCH_COUNTER) == 0) {
        printf("   No people registered.\n\n");
        return;
    }

    while (Int(ARRAY_COUNTER) < Int(SEARCH_COUNTER)) {
        ((int *)SCANF_BUFFER)[0] = Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER));
        ((int *)SCANF_BUFFER)[1] = Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 1);
        ((int *)SCANF_BUFFER)[2] = Array(PEOPLE_ARRAY, Int(ARRAY_COUNTER) + 2);
        ((int *)SCANF_BUFFER)[3] = Int(ARRAY_COUNTER) / 3 + 1;
        PARAM_2 = Int(ARRAY_COUNTER) + 3;

        printf("  [%d]\n", ((int *)SCANF_BUFFER)[3]);

        PARAM_0 = ((int *)SCANF_BUFFER)[0]; _getMemoryData();
        printf("   Name  : %s\n", (char *)MEMORY_DATA(RETURN));

        PARAM_0 = ((int *)SCANF_BUFFER)[1]; _getMemoryData();
        printf("   Age   : %d\n", *(int *)MEMORY_DATA(RETURN));

        PARAM_0 = ((int *)SCANF_BUFFER)[2]; _getMemoryData();
        printf("   Email : %s\n\n", (char *)MEMORY_DATA(RETURN));

        Int(ARRAY_COUNTER) = PARAM_2;
    }
}

void _fim(void) {
    printf("Goodbye!\n\n");
}

void _invalido(void) {
    printf("   Invalid option, please try again.\n\n");
}

int main(void) {

    /*
        ID 14 -> DECIOLUVIER@gmail.com
        ID PEOPLE_ARRAY - > [12, 14, 15, 16 ,17]
    */

    buffer = malloc(ROM_SIZE);
    BUFFER_SIZE = ROM_SIZE;
    NEXT_ID = CHAR_MAX;

    Int(MENU_OPTION) = 0;
    Array(PEOPLE_ARRAY, 0);

    printf(" People Manager  \n");
    printf("1 - Add person     \n");
    printf("2 - Remove person  \n");
    printf("3 - Search person  \n");
    printf("4 - Show all       \n");
    printf("5 - Exit           \n");

    while (Int(MENU_OPTION) != '5') {
        printf("Option: ");
        scanf(" %c", SCANF_BUFFER);
        Int(MENU_OPTION) = *SCANF_BUFFER;
        printf("\n");

        switch (Int(MENU_OPTION)) {
            case '1': _adicionar(); break;
            case '2': _remover(); break;
            case '3': _procurar(); break;
            case '4': _mostrarTodos(); break;
            case '5': _fim(); break;
            default: _invalido();  break;
        }
    }

    free(buffer);
    return 0;
}