#include"sort.hpp"
#include<stdlib.h>
void Print(int *arr,int sz)
{
	for(int i=0;i<sz;i++)
		cout<<arr[i]<<" ";
	cout<<endl;
}
//插入排序
void Insertsort(int* arr,int sz)
{
	for(int i=0;i<sz;i++)
	{
		int temp=arr[i];
		int j=i-1;
		for(;j>=0;j--)
		{
			if(temp<arr[j])
				arr[j+1]=arr[j];
			else
				break;
		}
		arr[j+1]=temp;
	}	

}
//快排第一种
int _qsort1(int* arr,int left,int right)
{
	right=right-1;
	int end=right;
	int key=arr[right];
	while(left<right)
	{
		while(arr[left]<=key && left<right)
			left++;
		while(arr[right]>=key && left<right)
			right--;
		swap(arr[left],arr[right]);
	}
	swap(arr[left],arr[end]);
	return left;
}
//快排第二种
int _qsort2(int* arr,int left,int right)
{
	right=right-1;
	int key=arr[right];
	while(left<right)
	{
		while(arr[left]<=key && left<right)
			left++;
		arr[right]=arr[left];
		while(arr[right]>=key && left<right)
			right--;
		arr[left]=arr[right];
	}
	arr[left]=key;
	return left;
}
//快排第三种
int _qsort3(int* arr,int left,int right)
{
	int pre=0;
	int cur=0;
	int key=arr[right-1];
	while(cur<right)
	{
		if(arr[cur]<=key)
		{
			if(arr[pre]>key)
			{
				swap(arr[pre],arr[cur]);
			}
			pre++;
		}
		cur++;
	}
	return pre-1;
}
void qsort(int* arr,int left,int right)
{
	if(left+1<right)
	{
		int mid=_qsort3(arr,left,right);
		qsort(arr,left,mid);
		qsort(arr,mid+1,right);
	}
}

void _Merge(int* arr,int left1,int left2,int right,int* temp)
{
	int end1=left2;
	int left=left1;
	int t=0;
	while(left1<end1 && left2<right)
	{
		if(arr[left1]<=arr[left2])
			temp[t++]=arr[left1++];
		else
			temp[t++]=arr[left2++];
	}
	while(left1<end1)
		temp[t++]=arr[left1++];
	while(left2<right)
		temp[t++]=arr[left2++];
	for(int i=0;i<t;i++)
		arr[left++]=temp[i];

}
void _Mergesort(int* arr,int left,int right,int* temp)
{
	if(left+1<right)
	{
		int mid=(left+right)/2;
		_Mergesort(arr,left,mid,temp);
		_Mergesort(arr,mid,right,temp);
		_Merge(arr,left,mid,right,temp);
	}
}
void Mergesort(int* arr,int sz)
{
	int* temp=new int[sz];
	int left=0;
	int right=sz;
	_Mergesort(arr,left,right,temp);
	delete[] temp;
}
int main()
{
	srand((unsigned int)time(NULL));
	for(int j=0;j<10;j++)
	{
	int arr[20]={0};
	for(int i=0;i<20;i++)
	{
  		arr[i] = rand()%100+1;
	}
	int sz=sizeof(arr)/sizeof(arr[0]);
	Print(arr,sz);
	Mergesort(arr,sz);
	Print(arr,sz);
	}
	return 0;
}
