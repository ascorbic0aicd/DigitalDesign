`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/26 17:20:13
// Design Name: 
// Module Name: io_ctrl
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


module io_ctrl(
    input [31:0]addr,
    input [31:0]datain,
    input en,
    input [31:0]mem_data,
    input [31:0]key_data,
    output [31:0]dataout,
    output read_key,
    output dmem_en,
    output vga_en,
    output vga_offset_en,
    output vga_color_en,
    output [7:0]vga_in
    );
    assign vga_in = datain[7:0];

    assign dataout = (addr[31:20] == 12'h003) ? key_data : mem_data;
    assign read_key = (addr[31:20] == 12'h003) ? 1'b1: 1'b0;
    assign vga_en = (addr[31:20] == 12'h002) ? en: 1'b0;
    assign vga_offset_en = (addr[31:20] == 12'h004) ? en: 1'b0;
    assign vga_color_en = (addr[31:20] == 12'h005) ? en: 1'b0;
    assign dmem_en = (addr[31:20] == 12'h001) ? en: 1'b0;
    
    
endmodule
