#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
struct SymbolTab;

struct TabItem
{
    char Name[32];
    char Attr[32];
    struct SymbolTab *ChildPtr;
};

struct SymbolTab
{
    struct TabItem items[128];
    struct SymbolTab *FatherTab;
    size_t size;
};

struct SymbolTab *NewTab()
{
    struct SymbolTab *tab = (struct SymbolTab *)malloc(sizeof(struct SymbolTab));
    tab->FatherTab = NULL;
    tab->size = 0;
    return tab;
}

void AddItem(struct SymbolTab *tab, char *name, char *attr)
{
    if (tab->size >= 128)
    {
        printf("SymbolTab is full.\n");
        return;
    }
    strcpy(tab->items[tab->size].Name, name);
    strcpy(tab->items[tab->size].Attr, attr);
    tab->size++;
}

void AddChildTab(struct SymbolTab *tab, struct SymbolTab *child)
{
    if (tab->size >= 128)
    {
        printf("SymbolTab is full.\n");
        return;
    }
    tab->items[tab->size].ChildPtr = child;
    child->FatherTab = tab;
    tab->size++;
}

bool IsPtrItem(struct TabItem *item)
{
    return item->ChildPtr != NULL;
}

struct TabItem *FindSymbol(struct SymbolTab *tab, char *name, bool isRecursion)
{
    if (tab == NULL)
    {
        return NULL;
    }
    int i = 0;
    for (; i < tab->size; ++i)
    {
        if (!IsPtrItem(&(tab->items[i])))
        {
            if (strcmp(name, tab->items[i].Name) == 0)
            {
                return &(tab->items[i]);
            }
        }
    }
    if (isRecursion == false)
    {
        return NULL;
    }
    return FindSymbol(tab->FatherTab, name, isRecursion);
}

void ReleaseTab(struct SymbolTab *tab)
{
    if (tab == NULL)
    {
        return;
    }
    int i = 0;
    for (; i < tab->size; ++i)
    {
        ReleaseTab(tab->items[i].ChildPtr);
    }
    free(tab);
}