#include "PriorityQueue.h"
#include <stdexcept>

template <typename T>
PriorityQueue<T>::PriorityQueue(int capacity, bool mode) : _size(0), _capacity(capacity), _mode(mode)
{
    _heap = new T[_capacity];
}

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
    delete[] _heap;
}

template <typename T>
void PriorityQueue<T>::swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void PriorityQueue<T>::heapifyUp(int index)
{
    if (_mode)
    {
        if (index && _heap[parent(index)] > _heap[index])
        {
            swap(_heap[index], _heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
    else
    {
        if (index && _heap[parent(index)] < _heap[index])
        {
            swap(_heap[index], _heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }
}

template <typename T>
void PriorityQueue<T>::heapifyDown(int index)
{
    if (_mode)
    {
        int smallest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < _size && _heap[left] < _heap[smallest])
            smallest = left;

        if (right < _size && _heap[right] < _heap[smallest])
            smallest = right;

        if (smallest != index)
        {
            swap(_heap[index], _heap[smallest]);
            heapifyDown(smallest);
        }
    }
    else
    {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);

        if (left < _size && _heap[left] > _heap[largest])
            largest = left;

        if (right < _size && _heap[right] > _heap[largest])
            largest = right;

        if (largest != index)
        {
            swap(_heap[index], _heap[largest]);
            heapifyDown(largest);
        }
    }
}

template <typename T>
int PriorityQueue<T>::parent(int index) { return (index - 1) / 2; }

template <typename T>
int PriorityQueue<T>::leftChild(int index) { return (2 * index + 1); }

template <typename T>
int PriorityQueue<T>::rightChild(int index) { return (2 * index + 2); }

template <typename T>
void PriorityQueue<T>::push(T data)
{
    if (_size == _capacity)
    {
        throw std::out_of_range("Priority Queue is full.");
    }
    _heap[_size] = data;
    heapifyUp(_size);
    _size++;
}

template <typename T>
void PriorityQueue<T>::forcePush(T data)
{
    _heap[0] = data;
    heapifyUp(_size);
    _size++;
}

template <typename T>
T PriorityQueue<T>::top()
{
    if (_size <= 0)
    {
        throw std::out_of_range("Priority Queue is empty.");
    }
    return _heap[0];
}

template <typename T>
void PriorityQueue<T>::pop()
{
    if (_size <= 0)
    {
        throw std::out_of_range("Priority Queue is empty.");
    }
    _heap[0] = _heap[--_size];
    heapifyDown(0);
}

template <typename T>
bool PriorityQueue<T>::empty()
{
    return _size == 0;
}

template <typename T>
int PriorityQueue<T>::size()
{
    return _size;
}

template <typename T>
void PriorityQueue<T>::toggleMode()
{
    _mode = !_mode;
    for (int i = _size / 2 - 1; i >= 0; i--)
    {
        heapifyDown(i);
    }
}

// Especifica a implementação do template para o tipo utilizado nos algoritmos.
template class PriorityQueue<Pair<double, Point>>;
template class PriorityQueue<Pair<double, long>>;