int main()
{
	int i =1;
	int sum = 1;
	int n=5;
	for (i = 1; i <= n; i=i+1)
	{
		sum = sum * i;
	}
	printf("%d的阶乘是%d\n",n, sum);
	return 0;
}