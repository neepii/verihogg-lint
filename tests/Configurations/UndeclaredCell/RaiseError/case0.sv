module top;
    memory u_mem();
    cache u_cache();
endmodule

module memory;
    // implementation
endmodule

config bad_config_1;
    design work.top;
    default liblist work;
    cell cache liblist work;
endconfig
