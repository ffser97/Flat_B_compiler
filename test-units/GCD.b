declblock
{
	int a,b,t1,t2,ans,t;
}
codeblock
{
	t = 0;
	while (t < 100000)
	{
		read a,b;
		if (b == 0)
		{
			b = a;
			a = 0;
		}
		if(b != 0)
		{
L1:			t1 = b;
			t2 = a % b;
			a = t1;
			b = t2;
			goto L1 if b != 0;
			ans = a;
L2:			print ans;
		}
		else { print 0; }
		t = t+1;
	}
}
