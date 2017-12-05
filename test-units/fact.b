declblock
{
	int ans,i;
}
codeblock
{
	ans = 1;
	read i;
	while i > 0
	{
		ans = ans * i;
		i = i - 1;
	}
	print ans;
}
