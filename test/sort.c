void BubbleSort( int *data, int n )
{
	int i, j, t;
	for( i = 1; i <= n-1; i=i+1 )
	{
		for( j = 1; j <= n-i; j=j+1 )
		{
			if( data[j+1] < data[j] )
			{
				t = data[j+1];
				data[j+1] = data[j];
				data[j]   = t;
			}
		}
	}
}

int main()
{
  long b[5]={1,2,5,3,4};
  int cc[2]={88,100};
  b[2]=99;
  BubbleSort(b,5);
}