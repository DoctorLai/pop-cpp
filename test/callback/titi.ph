#ifndef _TITI_PH
#define _TITI_PH
#include "toto.ph"

parclass Titi         
{

classuid(1001);

public:
  Titi() @{ od.node(1); od.executable("./titi.obj"); };
        
  ~Titi();

  seq sync void SetIdent(int i);
  seq sync void ComputeIdent(Toto &t);	

private:
  int ident;
};
#endif
