#include <stdio.h>

#include <linux/input.h>

#include <JSL/Event.h>

int CallBackAbs( unsigned int abs, int value )
{
	printf("%X %i\n", abs, value);
	
	return 1;
}

int	main(int arc,char const ** argv)
{
	JSL::Event d;
	
	d.Open(argv[1]);

	printf("%i\n",(int)sizeof(input_event));

	d.Set( JSL::Event::flag::PRINT );

	d.Launch();

	d.sig_abs_.connect( CallBackAbs );
	
	d.Join();
	
	return 0;
}


