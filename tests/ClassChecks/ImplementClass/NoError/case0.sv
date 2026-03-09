interface class my_intf;
  pure virtual function void display();
endclass

class MyClass implements my_intf; // Correct syntax
  virtual function void display();
    $display("Implemented");
  endfunction
endclass
