extern "C" {

#include "xmp.h"

}

int main(int argc, char* argv[]) {
  int* data; 
  data = new int[5]; 
  
  xmp_fnct(data, 5); 

  delete [] data;  

  return 0; 
}
