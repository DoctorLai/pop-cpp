#ifndef _INTEGER_PH
#define _INTEGER_PH

parclass Integer
{
	classuid(1000);

public:	
  	Integer() @{ od.url("localhost"); od.executable("/Users/clementval/versioning/popc/popc2.6/examples/integer/integer.obj");};
	~Integer();

	seq async void Set(int val);
	conc int Get();
	mutex void Add([in]Integer &other);

private:
	int data;

};

#endif
