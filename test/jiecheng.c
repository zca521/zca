int main()
{
    int n=5;
    int i;
    int result=1;
    for(i=0;i<n;i=i+1)
    {
        result=result*i;
    }
    printf("%d的阶乘是%d\n",n,result);
}