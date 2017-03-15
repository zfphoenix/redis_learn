#include <iostream>


void print(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << "\t"; 
    }
    std::cout << std::endl;
}

int parent_index(int index)
{
    if (index==0)
        return -1;
    if (index%2==0)
        return index/2 -1;
    else
        return index/2;
}

void heap_sort(int arr[], int begin_index, int size)
{
    if (begin_index < 0)
        return;
    if (begin_index >= size-1)
        return ;
    int end_index = size-1;
   // std::cout << "111 begin: " << begin_index<<" sie: "<<  size << std::endl;
    while (end_index > begin_index)
    { 
        int parent = parent_index(end_index);
        if (parent >= 0)
        {
            if (arr[parent] < arr[end_index])
            {
                int temp = arr[parent];
                arr[parent] = arr[end_index];
                arr[end_index] = temp;
            }
        }
        --end_index;
    }
    heap_sort(arr, ++begin_index, size);
}

int main(int argc, char* argv[])
{
    int arr[5];
    arr[0] = 10;
    arr[1] = 50;
    arr[2] = 30;
    arr[3] = 20;
    arr[4] = 110;
    print(arr, 5);
    heap_sort(arr, 0, 5);
    print(arr, 5);
}

