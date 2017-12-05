declblock
{
	int arr[30],it1,it2,t,temp,temp1;
}
codeblock
{
	t = 0;
	while t < 100000
	{
		it1 = 0;
		for ind = 0,9
		{
			read arr[it1];
			it1 = it1 + 1;
		}
		it1 = 0;
		for ind1 = 0,9
		{
			it2 = 0;
			temp1 = 9 - (it1);
			for ind2 = 0 , temp1
			{
				if (arr[it2] > arr[it2 + 1])
				{
					temp = arr[it2];
					arr[it2] = arr[it2 + 1];
					arr[it2 + 1] = temp;	
				}
				it2 = it2+1;
			}
			it1 = it1 + 1;
		}
		it1 = 0;
		for ind2 = 0,9 { print arr[it1]; it1 = it1 + 1; }
		t = t+1;
	}
}
