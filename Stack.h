
#pragma once
#include <stdexcept>
template <typename T>

class Stack
{
private:
    struct ListNode
    {
        T   element;
        ListNode* next;

        ListNode(const T& theElement, ListNode* n = NULL)
            : element(theElement), next(n) { }
    };

    ListNode* topOfStack;

public:
    Stack();
    Stack(const Stack& rhs);
    ~Stack();

    bool isEmpty() const;
    bool isFull() const;
    void makeEmpty();
    int number_of_cells(Stack & stack) const;
    void pop();
    void push(const T& x);
    void goDown(Stack & stack);
    T topAndPop();
    const T& top() const;

    const Stack& operator=(const Stack& rhs);
};


#include <stdexcept>
using namespace std;

template <class T>
Stack<T>::Stack()
{
    topOfStack = NULL;
}

template <class T>
bool Stack<T>::isFull() const {
    
    return false;
}

template <class T>
bool Stack<T>::isEmpty() const
{
    return topOfStack == NULL;
}

template <class T>
const T& Stack<T>::top() const {
    if (isEmpty()) {
        throw underflow_error("Stack is empty");
    }
    return topOfStack->element;
}

template <class T>
void Stack<T>::pop() {
    if (isEmpty()) {
        throw underflow_error("Stack is empty");
    }
    ListNode* oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}

template <class T>
void Stack<T>::push(const T& x)
{
    topOfStack = new ListNode(x, topOfStack);
}

template <class T>
T Stack<T>::topAndPop()
{
    if (isEmpty()) {
        throw underflow_error("Cannot perform topAndPop on an empty stack");
    }
    T topItem = top();
    pop();
    return topItem;
}

template <class T>
void Stack<T>::makeEmpty()
{
    while (!isEmpty())
        pop();
}

template <class T>
const Stack<T>& Stack<T>::
operator=(const Stack<T>& rhs)
{
    if (this != &rhs)
    {
        makeEmpty();
        if (rhs.isEmpty())
            return *this;

        ListNode* rptr = rhs.topOfStack;
        ListNode* ptr = new ListNode(rptr->element);
        topOfStack = ptr;

        for (rptr = rptr->next; rptr != NULL; rptr = rptr->next)
            ptr = ptr->next = new ListNode(rptr->element);
    }
    return *this;
}

template <class T>
Stack<T>::Stack(const Stack<T>& rhs)
{
    topOfStack = NULL;
    *this = rhs; // deep copy 
}


template <class T>
Stack<T>::~Stack()
{
    makeEmpty();
}
