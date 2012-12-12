

parclass POPGroup {
public: 
  classuid(3001); 
  POPGroup(int node) @{ od.node(node); od.executable("./popgroup.obj"); }; 
  POPGroup(int node, double precision) @{ od.node(node); od.executable("./popgroup.obj"); }; 

  ~POPGroup(); 

  sync broadcast void collective(); 
  async broadcast void asynchronous_collective(); 

  sync seq void set_value(int value);
  sync seq int get_node_value(); 
  sync seq double get_precision_value(); 

private: 
  int _node; 
  double _precision; 

}; 