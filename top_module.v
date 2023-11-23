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
    output [7:0]HEX,
    output [7:0]AN
    );
    wire [31:0] iaddr,idataout;
    wire iclk;
    wire [31:0] daddr,ddataout,ddatain;
    wire drdclk, dwrclk, dwe;
    wire [2:0]  dop;
    wire [31:0] cpudbgdata;


    display_module dis(CLK100MHZ,cpudbgdata,AN,HEX); 
    //main CPU
    cpu mycpu(.clock(clk), 
                 .reset(reset), 
    				 .imemaddr(iaddr), .imemdataout(idataout), .imemclk(iclk), 
    				 .dmemaddr(daddr), .dmemdataout(ddataout), .dmemdatain(ddatain), .dmemrdclk(drdclk), .dmemwrclk(dwrclk), .dmemop(dop), .dmemwe(dwe), 
    				 .dbgdata(cpudbgdata));

    
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
    				 .we(dwe));

        

endmodule
