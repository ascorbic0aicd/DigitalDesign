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
    input CLK100MHZ,
    input reset,
    input clk,
    input [15:0]SW,
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

    assign reg_addr = SW[4:0];
    VGA v(SW[11:0],vga_data,CLK100MHZ,dwrclk,clk,daddr,vga_en,vga_in,VGA_R,VGA_G,VGA_B,VGA_HS,VGA_VS);

    assign dis_data = SW[15] ? (SW[14] ?(SW[13]? reg_addr:reg_data): (SW[13]? vga_data:daddr)) : (SW[14] ? idataout : cpudbgdata);
    display_module dis(CLK100MHZ,dis_data,AN,HEX); 

    io_ctrl myio(.addr(daddr),
                 .datain(ddatain),
                 .en(dwe),
                 .vga_en(vga_en),
                 .vga_in(vga_in),
                 .dmem_en(dmem_en));
    //main CPU

    ferquency_divider f_cpu(CLK100MHZ,4,cpu_clk);
    cpu mycpu(.clock(cpu_clk), 
                 .reset(reset), 
    				 .imemaddr(iaddr), .imemdataout(idataout), .imemclk(iclk), 
    				 .dmemaddr(daddr), .dmemdataout(ddataout), .dmemdatain(ddatain), .dmemrdclk(drdclk), .dmemwrclk(dwrclk), .dmemop(dop), .dmemwe(dwe), 
    				 .dbgdata(cpudbgdata),
                     .reg_addr(reg_addr),
                     .reg_data(reg_data));

    
    //instruction memory, no writing
    instr_mem instructions(
    	.addr(iaddr[17:2]),
    	.clock(iclk),
    	.data(idataout));
    //data memory	
    data_mem dmem(.addr(daddr), 
                 .dataout(ddataout), 
    				 .datain(ddatain), 
    				 .rdclk(drdclk), 
    				 .wrclk(dwrclk), 
    				 .memop(dop), 
    				 .we(dmem_en));

        

endmodule
