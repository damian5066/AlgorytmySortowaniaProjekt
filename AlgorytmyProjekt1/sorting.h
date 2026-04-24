#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include <iterator>


//  MERGE SORT
template<typename T>
// Merges two subarrays of arr[].
void merge(std::vector<T>& arr, int left, int mid, int right) {
    std::vector<T> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<T> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < (int)leftArr.size() && j < (int)rightArr.size()) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }
    while (i < (int)leftArr.size()) arr[k++] = leftArr[i++];
    while (j < (int)rightArr.size()) arr[k++] = rightArr[j++];
}

template<typename T>
// Recursive merge sort function that divides the array and calls merge to sort and combine
void mergeSort(std::vector<T>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

template<typename T>
// Main function for merge sort that checks if the array is non-empty and calls the recursive function
void mergeSort(std::vector<T>& arr) {
    if (arr.size() > 1)
        mergeSort(arr, 0, (int)arr.size() - 1);
}

// QUICKSORT
template<typename T>
// Chooses a pivot using median-of-three method and rearranges the array to place the pivot in its correct position
T medianOfThree(std::vector<T>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid])  std::swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) std::swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) std::swap(arr[mid], arr[right]);
    // arr[mid] is median – move to right-1 as pivot
    std::swap(arr[mid], arr[right - 1]);
    return arr[right - 1];
}

template<typename T>
// Partitions the array around the pivot and returns the final index of the pivot
int partition(std::vector<T>& arr, int left, int right) {
    T pivot = medianOfThree(arr, left, right);
    int i = left, j = right - 1;
    while (true) {
        while (arr[++i] < pivot) {}
        while (arr[--j] > pivot) {}
        if (i >= j) break;
        std::swap(arr[i], arr[j]);
    }
    std::swap(arr[i], arr[right - 1]);
    return i;
}

template<typename T>
// Helper function for quicksort that recursively sorts the array and uses insertion sort for small sub-arrays
void quickSortHelper(std::vector<T>& arr, int left, int right) {
    if (right - left < 3) {
        for (int i = left + 1; i <= right; ++i) {
            T key = arr[i];
            int j = i - 1;
            while (j >= left && arr[j] > key) { arr[j + 1] = arr[j]; --j; }
            arr[j + 1] = key;
        }
        return;
    }
    int pivotIdx = partition(arr, left, right);
    quickSortHelper(arr, left, pivotIdx - 1);
    quickSortHelper(arr, pivotIdx + 1, right);
}

template<typename T>
// Main function for quicksort that checks if the array is non-empty and calls the helper function
void quickSort(std::vector<T>& arr) {
    if (arr.size() > 1)
        quickSortHelper(arr, 0, (int)arr.size() - 1);
}

//  INTROSORT – QuickSort + HeapSort + InsertionSort
template<typename T>
void insertionSort(std::vector<T>& arr, int left, int right) {
	// insertion sort for small sub-arrays
    for (int i = left + 1; i <= right; ++i) {
		// Store the current element to be compared
        T key = arr[i];
        int j = i - 1;
		// Shift elements greater than key to the right
        while (j >= left && arr[j] > key) { arr[j + 1] = arr[j]; --j; }
        arr[j + 1] = key;
    }
}

template<typename T>
// Heapify a subtree rooted with node i which is an index in arr[].
void heapify(std::vector<T>& arr, int n, int i, int offset) {
    int largest = i;
	// Calculate left and right child indices based on the offset
    int l = 2 * (i - offset) + 1 + offset;
    int r = 2 * (i - offset) + 2 + offset;
	// If children are larger than root
    if (l < offset + n && arr[l] > arr[largest]) largest = l;
    if (r < offset + n && arr[r] > arr[largest]) largest = r;
	// If largest is not root, swap and continue heapifying
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest, offset);
    }
}

template<typename T>
// Main function to perform heap sort on arr[]
void heapSort(std::vector<T>& arr, int left, int right) {
    int n = right - left + 1;
    for (int i = left + n / 2 - 1; i >= left; --i)
        heapify(arr, n, i, left);
    for (int i = right; i > left; --i) {
        std::swap(arr[left], arr[i]);
        heapify(arr, i - left, left, left);
    }
}

template<typename T>
// Partition function for IntroSort, similar to QuickSort but with median-of-three pivot selection
int introPartition(std::vector<T>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid])  std::swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) std::swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) std::swap(arr[mid], arr[right]);
    T pivot = arr[mid];
    std::swap(arr[mid], arr[right - 1]);
    int i = left, j = right - 1;
    while (true) {
        while (arr[++i] < pivot) {}
        while (arr[--j] > pivot) {}
        if (i >= j) break;
        std::swap(arr[i], arr[j]);
    }
    std::swap(arr[i], arr[right - 1]);
    return i;
}

template<typename T>
// Helper function for IntroSort that recursively sorts the array and switches to HeapSort if depth limit is reached
void introSortHelper(std::vector<T>& arr, int left, int right, int depthLimit) {
    int size = right - left + 1;
    if (size <= 16) {
        insertionSort(arr, left, right);
        return;
    }
    if (depthLimit == 0) {
        heapSort(arr, left, right);
        return;
    }
    int pivotIdx = introPartition(arr, left, right);
    introSortHelper(arr, left, pivotIdx - 1, depthLimit - 1);
    introSortHelper(arr, pivotIdx + 1, right, depthLimit - 1);
}


template<typename T>
// Main function for IntroSort that calculates the depth limit and calls the helper function
void introSort(std::vector<T>& arr) {
    if (arr.size() > 1) {
        int depthLimit = 2 * (int)std::log2(arr.size());
        introSortHelper(arr, 0, (int)arr.size() - 1, depthLimit);
    }
}
