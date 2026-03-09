class my_intf;
  function void display();
     $display("Implemented");
  endfunction
endclass

class MyClass implements my_intf; // Incorrect syntax
  virtual function void display();
    $display("Overridden");
  endfunction
endclass
