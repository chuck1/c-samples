#include <stdio.h>

#include <linux/input.h>

#include <JSL/mouse.h>

int CallBack(int x,int y)
{
	printf("%i %i\n",x,y);

	return 1;
}

int	main(int arc,char const ** argv)
{
	JSL::mouse d;
	
	d.Open(argv[1]);

	printf("%i\n",(int)sizeof(input_event));

	while(1)
	{
		d.Read();
	}
	
	return 0;
}


