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
    input [31:0]timer_data,
    input [31:0]addr,
    input [31:0]datain,
    input en,
    input [31:0]mem_data,
    input [31:0]key_data,
    output reg[31:0]dataout,
    output read_key,
    output dmem_en,
    output vga_en,
    output vga_offset_en,
    output vga_color_en,
    output vga_cursor_en,
    output [7:0]vga_in,
    output [11:0]vga_cursor_data
    );
    assign vga_in = datain[7:0];
    always @(*) 
    begin
        case (addr[31:20])
            12'h003:dataout = key_data;
            12'h007:dataout = timer_data; 
            default:dataout = mem_data; 
        endcase    
    end
    assign read_key = (addr[31:20] == 12'h003) ? 1'b1: 1'b0;
    assign vga_en = (addr[31:20] == 12'h002) ? en: 1'b0;
    assign vga_offset_en = (addr[31:20] == 12'h004) ? en: 1'b0;
    assign vga_color_en = (addr[31:20] == 12'h005) ? en: 1'b0;
    assign vga_cursor_en = (addr[31:20] == 12'h006) ? en: 1'b0;
    assign dmem_en = (addr[31:20] == 12'h001) ? en: 1'b0;
    assign vga_cursor_data = datain[11:0];
    
    
endmodule
