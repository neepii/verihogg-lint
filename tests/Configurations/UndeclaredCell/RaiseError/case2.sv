module alu;
    // implementation
endmodule

module top;
    // 'alu' module is NOT instantiated anywhere
endmodule

config bad_config_3;
    design work.top;
    default liblist work;
    cell alu liblist work;
endconfig
