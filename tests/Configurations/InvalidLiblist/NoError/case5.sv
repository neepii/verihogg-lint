module top;
    initial $display("Top module");
endmodule

module sub;
    initial $display("Sub module");
endmodule

config working_config;
    design work.top;
    default liblist work rtl_lib;
    instance top.u_sub liblist work;
endconfig
