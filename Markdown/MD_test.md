# Data Structure
![Data Structure](img/DS_hierarchy.png)

\newpage

## Table of Contents
* [Tittle Page](#data-structure) 
* [Contents](#table-of-contents)
* [Array](#array)
* [Lists](#lists)
  + [Sequential List](#sequential-list)
  + [Linked List](#linked-list)
* [Stacks](#stacks)
  - [Code](#code)
    -  [x] [Code1: with quote](#code1-with-quote)
    -  [ ] [Code2: without quote](#code2-without-quote)
* [Queues](#queues)
* [Additional](#additional)


\newpage

## [Array](#array)
![array](img/array.png)

## [Lists](#lists)
This is **List** part...

### [Sequential List](#sequential-list)
Some content here...

### [Linked List](#linked-list)
![linked list](img/linkedlist.png)

\newpage

## [Stacks](#stacks)
A stack is ...

`Code`  

- [Code 1](#code1)
- [Code 2](#code2)

click *[<span style="color:blue">here</span>](#additional-code)* to see additional code



## [Code](#code)

### [Code1: with quote](#code1-with-quote)
> ```{#code1 .C .numberLines}
> typedef struct
> {
>	void **content;
>	int size;
>       int top;
> } Stack;
> 
> void stackInit(Stack *s, int size);
> void stackPush(Stack *s, void* val);
> void* stackTop(Stack *s);
> void* stackPop(Stack *s);
> int stackSize(Stack *s);
> void stackFree(Stack *s);
> ```

### [Code2: without quote](#code2-without-quote) 

```{#code2 .C .numberLines}
typedef struct
{
       void **content;
       int size;
       int top;
} Stack;

void stackInit(Stack *s, int size);
void stackPush(Stack *s, void* val);
void* stackTop(Stack *s);
void* stackPop(Stack *s);
int stackSize(Stack *s);
void stackFree(Stack *s);
```

## [Queues](#queues)

~~The world is flat.~~

## [Additional](#additional)
~~~~{#additional-code .haskell .numberLines startFrom="100"}
qsort []     = []
qsort (x:xs) = qsort (filter (< x) xs) ++ [x] ++
               qsort (filter (>= x) xs)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-------------------------------------------------------------
 Centered   Default           Right Left
  Header    Aligned         Aligned Aligned
----------- ------- --------------- -------------------------
   First    row                12.0 Example of a row that
                                    spans multiple lines.

  Second    row                 5.0 Here's another one. Note
                                    the blank line between
                                    rows.
-------------------------------------------------------------

Table: Here's the caption. It, too, may span
multiple lines.

---

: Sample grid table.

+---------------+---------------+--------------------+
| Fruit         | Price         | Advantages         |
+:==============+:=============:+===================:+
| Bananas       | $1.34         | - built-in wrapper |
|               |               | - bright color     |
+---------------+---------------+--------------------+
| Oranges       | $2.10         | - cures scurvy     |
|               |               | - tasty            |
+---------------+---------------+--------------------+
