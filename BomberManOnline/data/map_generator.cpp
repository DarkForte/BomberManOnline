#include<iostream>
#include<stdio.h>
using namespace std;

const int WIDTH=30, HEIGHT=20;

int main()
{
	freopen("map1.txt","w",stdout);
	int i;
	srand(unsigned(time(0)));
	for(i=1;i<=30;i++)
	{
		int x=rand()%WIDTH;
		int y=rand()%HEIGHT;
		
		cout<<x<<" "<<y<<" "<<1<<" 0"<<endl;
	}
	for(i=1; i<=30; i++)
	{
		int x=rand()%WIDTH;
		int y=rand()%HEIGHT;
		
		cout<<x<<" "<<y<<" "<<2<<" 0"<<endl;
	}
	cout<<"-1 -1 -1 -1"<<endl;
	return 0;
}