`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/21 17:30:01
// Design Name: 
// Module Name: top_module
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top_module(
    input UART_TXD_IN,
    output UART_RXD_OUT,
    input CLK100MHZ,
    input reset,
    input clk,
    input [15:0]SW,
    input PS2_CLK,
    input PS2_DATA,
    output [7:0]HEX,
    output [7:0]AN,
    output [3:0]VGA_R,
    output [3:0]VGA_G,
    output [3:0]VGA_B,
    output VGA_HS,
    output VGA_VS
    );
    wire [31:0] iaddr,idataout;
    wire iclk;
    wire [31:0] daddr,ddataout,ddatain;
    wire drdclk, dwrclk, dwe;
    wire [2:0]  dop;
    wire [31:0] cpudbgdata;
    wire [7:0]vga_in;
    wire vga_en;
    wire dmem_en;
    wire cpu_clk;

    wire [31:0]dis_data;
    wire [31:0]vga_data;
    wire [4:0]reg_addr;
    wire [31:0] reg_data;
    wire vga_offset_en;
    wire vga_color_en;
    assign reg_addr = SW[4:0];
    wire cursor_en;
    wire [11:0]cursor_data;

    
    wire [31:0] uart_idata;
    wire uart_iwe;
    wire [31:0] no_use;    
    wire [31:0] uart_iaddr;
    wire [31:0] uart_dataaddr;
    wire [31:0] uart_mdata;
    wire  uart_dwe;
    wire uart_halt;
    wire uart_rst;

    wire [31:0] cpu_read_data;
    wire [31:0] heap_data;
    wire heap_en;

    VGA v(cursor_en,cursor_data,vga_color_en,vga_offset_en,SW[11:0],vga_data,CLK100MHZ,dwrclk,clk,daddr,vga_en,vga_in,VGA_R,VGA_G,VGA_B,VGA_HS,VGA_VS);

    assign dis_data = SW[15] ? (SW[14] ?(SW[13]? reg_addr:reg_data): (SW[13]? vga_data:daddr)) : (SW[14] ? idataout : cpudbgdata);
    display_module dis(CLK100MHZ,dis_data,AN,HEX);
    
    wire [31:0] time_data;
    clock timer(CLK100MHZ,time_data);
    wire key_clk;
    wire [7:0] key_data;
    wire [7:0] board_data;
    ferquency_divider div(CLK100MHZ ,99, key_clk);
    keyboard Qkeyboard(.clk(key_clk),
	                   .clrn(1'b1),
	                   .ps2_clk(PS2_CLK),
	                   .ps2_data(PS2_DATA),
	                   .ascii_key(board_data)
                       );
    wire will_read;

    key_mem Qkey_mem(.keyclk(key_clk),.cpuclk(cpu_clk),.will_read(will_read),.ascii_key(board_data),.cpu_data(key_data));

    io_ctrl myio(.addr(daddr),
                 .timer_data(time_data),
                 .datain(ddatain),
                 .en(dwe),
                 .mem_data(ddataout),
                 .heap_data(heap_data),
                 .key_data({24'h000000,key_data}),
                 .dataout(cpu_read_data),
                 .read_key(will_read),
                 .vga_en(vga_en),
                 .vga_offset_en(vga_offset_en),
                 .vga_color_en(vga_color_en),
                 .vga_in(vga_in),
                 .dmem_en(dmem_en),
                 .heap_en(heap_en),
                 .vga_cursor_data(cursor_data),
                 .vga_cursor_en(cursor_en));
    //main CPU

    ferquency_divider f_cpu(CLK100MHZ,2,cpu_clk);
    cpu mycpu(.clock(cpu_clk&~uart_halt), 
                 .reset(reset|uart_rst), 
    				 .imemaddr(iaddr), .imemdataout(idataout), .imemclk(iclk), 
    				 .dmemaddr(daddr), .dmemdataout(cpu_read_data), .dmemdatain(ddatain), .dmemrdclk(drdclk), .dmemwrclk(dwrclk), .dmemop(dop), .dmemwe(dwe), 
    				 .dbgdata(cpudbgdata));

    //instruction memory, no writing
    instr_mem instructions(
    	.addr(uart_halt?uart_iaddr[17:2]:iaddr[17:2]),
    	.clock(uart_halt?~cpu_clk:iclk),
        .iwe(uart_iwe),
        .datain(uart_idata),
    	.data(idataout));
    //data memory	
    data_mem dmem(.addr(uart_halt?uart_dataaddr: daddr), 
                 .dataout(ddataout), 
    				 .datain(uart_halt?uart_mdata:ddatain), 
    				 .rdclk(drdclk), 
    				 .wrclk(uart_halt?~cpu_clk:dwrclk), 
    				 .memop(uart_halt?3'b010:dop), 
    				 .we(uart_halt?uart_dwe: dmem_en));

    data_mem heap_mem(.addr(daddr), 
                      .dataout(heap_data), 
    				  .datain(ddatain), 
    				  .rdclk(drdclk), 
    				  .wrclk(dwrclk), 
    				  .memop(dop), 
    				  .we(uart_halt?1'b0: heap_en));

    uart_port urat(  .clk(cpu_clk),
                     .clk100m(CLK100MHZ),
                     .rxd_out(UART_RXD_OUT),
                     .txd_in(UART_TXD_IN),
                     .dbgdata(no_use),
                     .rst(reset),
                     .instaddr(uart_iaddr),
                     .instdata(uart_idata),
                     .iwe(uart_iwe),
                     .dataaddr(uart_dataaddr),
                     .mdata(uart_mdata),
                     .dwe(uart_dwe),
                     .cpuhalt(uart_halt),
                     .cpureset(uart_rst));
endmodule

module clock(
    input CLK100MHZ,
    output reg [31:0]data = 0
    );
    wire clk;
    ferquency_divider f(CLK100MHZ,4_9999,clk);
    always @(posedge clk ) 
    begin
        data <= data + 1;    
    end 
endmodule
