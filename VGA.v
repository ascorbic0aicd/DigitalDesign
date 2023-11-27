`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/26 15:48:18
// Design Name: 
// Module Name: VGA
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


`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/21 16:55:07
// Design Name: 
// Module Name: lab08
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


module VGA(
    input [11:0]debug_addr,
    output [31:0]debug_data,
    input CLK100MHZ,
    input wrclk,
    input clk,
    input [31:0]wraddr,
    input vga_en,
    input [7:0]ascii_key,
    output [3:0]VGA_R,
    output [3:0]VGA_G,
    output [3:0]VGA_B,
    output VGA_HS,
    output VGA_VS
    );


    wire [7:0] key_count;
    wire [7:0] cur_key;
    wire have_key;
    
    wire clk_vga;
    wire [11:0]vga_data;
    wire [9:0]h_addr;
    wire [9:0]v_addr;
    wire valid;
    wire temp;
    wire [3:0]R;
    wire [3:0]G;
    wire [3:0]B;

    wire [4:0] read_y;
    wire [6:0] read_x;
    wire [3:0] offset_x;
    assign read_y = v_addr[8:4];

    
    helper_cnt x_gen(clk_vga,valid,read_x,offset_x);
    
    wire [7:0] now_ascii;
    VGA_mem ascR(.debug_addr(debug_addr),.debug_data(debug_data),.ascii_key(ascii_key),.clk(wrclk), .en(vga_en),.wraddr(wraddr),.read_x(read_x),.read_y(read_y),.data(now_ascii));

    vga_clk myvgaclk(.clk_in1(CLK100MHZ),.reset(1'b0),.locked(temp),.clk_out1(clk_vga));
    
    vga_ctrl myvga(clk_vga, 1'b0, vga_data, h_addr, v_addr, VGA_HS, VGA_VS, valid, R, G, B);
    wire [11:0] temp_font;
    font_rom rom(.addra({now_ascii,v_addr[3:0]}),
                      .clka(~clk_vga),
                      .douta(temp_font),
                      .ena(1'b1));
    wire rom_data;
    assign rom_data = temp_font[offset_x]; 
    assign vga_data = rom_data?12'b1111_1111_1111:12'b0000_0000_0000;

    assign VGA_R = valid? R:4'b0000;
    assign VGA_G = valid? G:4'b0000;
    assign VGA_B = valid? B:4'b0000;
endmodule
module helper_cnt (
    
    input clk,
    input valid,
    output reg[6:0]x = 0,
    output reg [3:0]cnt = 0
);
    always @(posedge clk) 
    begin
        if (valid) 
        begin
            if (cnt==4'b1000) 
            begin
                x <= x + 1;
                cnt <= 0;
            end
            else
            begin
                cnt  <= cnt + 1;   
            end
        end
        else
        begin
            cnt <= 0;
            x <= 0;
        end
    end
    
endmodule

module vga_ctrl(
 input pclk, 
 input reset, 
 input [11:0] vga_data, 
 output [9:0] h_addr, 
 output [9:0] v_addr,
 output hsync, 
 output vsync,
 output valid, 
 output [3:0] vga_r, 
 output [3:0] vga_g,
 output [3:0] vga_b
 );
    parameter h_frontporch = 96;
    parameter h_active = 144;
    parameter h_backporch = 784;
    parameter h_total = 800;
    parameter v_frontporch = 2;
    parameter v_active = 35;
    parameter v_backporch = 515;
    parameter v_total = 525;
    reg [9:0] x_cnt;
    reg [9:0] y_cnt;
    wire h_valid;
    wire v_valid;
    always @(posedge reset or posedge pclk) 
    if (reset == 1'b1)
        x_cnt <= 1;
    else
    begin
        if (x_cnt == h_total)
            x_cnt <= 1;
        else
            x_cnt <= x_cnt + 10'd1;
    end
    always @(posedge pclk)
        if (reset == 1'b1)
            y_cnt <= 1;
        else
        begin
            if (y_cnt == v_total & x_cnt == h_total)
                y_cnt <= 1;
        else if (x_cnt == h_total)
                y_cnt <= y_cnt + 10'd1;
        end

    assign hsync = (x_cnt > h_frontporch);
    assign vsync = (y_cnt > v_frontporch);

    assign h_valid = (x_cnt > h_active) & (x_cnt <= h_backporch);
    assign v_valid = (y_cnt > v_active) & (y_cnt <= v_backporch);
    assign valid = h_valid & v_valid;
    assign h_addr = h_valid ? (x_cnt - 10'd145) : {10{1'b0}};
    assign v_addr = v_valid ? (y_cnt - 10'd36) : {10{1'b0}};
    assign vga_r = vga_data[11:8];
    assign vga_g = vga_data[7:4];
    assign vga_b = vga_data[3:0];
endmodule
