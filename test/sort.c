int main()
{
  	int a[10]={99,2,34,88,100,20,1,5,3,4};
	int i, j, t;
	int len=10;
    printf("原数组为：");
    for (i=0; i<len; i=i+1) {
        printf("%d  ",a[i]);
    }
    printf("\n");
    for (i=0; i<len-1; i=i+1) {	
        for (j=0; j<len-1-i; j=j+1) 
		{
            if (a[j]>a[j+1]) 
			{
                t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
        }
    }
    printf("排序后为：");
    for (i=0; i<len; i=i+1) {
        printf("%d  ",a[i]);
    }
    printf("\n");
    return 0;
}