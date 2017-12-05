declblock
{
	int a,b,n,ans,t;
}
codeblock
{
	t = 0;
	while t < 1000000
	{
		ans = 1;
		read a,b,n;
		a = a % n;
		while b > 0
		{
			if (b % 2 == 1) { ans = (ans * a) % n; }
			a = (a * a) % n;
			b = b / 2;
		}
		print ans;
		t = t + 1;
	}
}
