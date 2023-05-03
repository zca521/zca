int main()
{
    int m=3,n=4;
    int i = 0,j=0,k=0;
    int arr1[3][4]={1,1,1,1,2,2,2,2,3,3,3,3};
    int arr2[4][3]={2,2,2,2,2,2,2,2,2,2,2,2};
    int arr3[3][3]={0};
	for (i = 0; i < m; i=i+1)
	{
		for (j = 0; j < m; j=j+1)
		{
			for (k = 0; k < n; k=k+1)
			{
				arr3[i][j] =arr3[i][j]+ arr1[i][k] * arr2[k][j];
			}
		}
	}
    for(i=0;i<3;i=i+1)
    {
        for(j=0;j<3;j=j+1)
        {
            printf("%3d     ",arr3[i][j]);
        }
        printf("\n");
    }

    return 0;
}