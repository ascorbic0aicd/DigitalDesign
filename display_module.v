`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/21 17:30:51
// Design Name: 
// Module Name: display_module
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


module display_module(
    input CLK100MHZ,
    input [31:0]data,
    output wire[7:0]AN,
    output wire[7:0]HEX);
    
    reg [3:0]Q;
    reg [2:0]flag = 3'b00;
    wire clk;
    reg [7:0]temp;
    ferquency_divider d(CLK100MHZ ,50000,clk);
    always @(posedge clk)
    begin
        case(flag)
        3'b000:
        begin
            temp <= 8'b1111_1110;
            Q <= data[3:0];
            flag <= flag+1;
            end
        3'b001:
        begin
            temp <= 8'b1111_1101;
            Q <= data[7:4];
            
            flag <= flag+1;
            end
        3'b010:
        begin
            temp <= 8'b1111_1011;
            Q <= data[11:8];
            flag <= flag+1;
            end
        3'b011:
        begin
            temp <= 8'b1111_0111;
            Q <= data[15:12];
            flag <= flag+1;
        end
        3'b100:
        begin
            temp <= 8'b1110_1111;
            Q <= data[19:16];
            
            flag <= flag+1;
        end
        3'b101:
        begin
            temp <= 8'b1101_1111;
            Q <= data[23:20];
            
            flag <= flag+1;
        end
        3'b110:
        begin
            temp <= 8'b1011_1111;
            Q <= data[27:24];
            
            flag <= flag+1;
        end
        default:
        begin
            temp <= 8'b0111_1111;
            Q <= data[31:28];
            
            flag<=3'b000;
        end
        endcase
    end
    assign AN =  temp;
   bcd7seg b1(Q,HEX);
endmodule

module bcd7seg(
	 input  [3:0] b,
	 output reg [7:0] h
	 );
	 
	 
   //add your code here
   always @(*)
   begin 
	  case(b)
	       0: h = 8'b1100_0000;
	       1: h = 8'b1111_1001;
	       2: h = 8'b1010_0100;
	       3: h = 8'b1011_0000;
	       4: h = 8'b1001_1001;
	       5: h = 8'b1001_0010;
	       6: h = 8'b1000_0010;
	       7: h = 8'b1111_1000;
	       8: h = 8'b1000_0000;
	       9: h = 8'b1001_0000;
	       10: h = 8'b1000_1000;
	       11: h = 8'b1000_0011;
	       12: h = 8'b1100_0110;
	       13: h = 8'b1010_0001;
	       14: h = 8'b1000_0110;
	       15: h = 8'b1000_1110;
           default: h = 8'b1000_0000;
	  endcase
	end
endmodule

module ferquency_divider(
    input CLK100MHZ,
    input [31:0]divide,
    output reg clk_1s
    );
    
    reg [31:0]count_clk = 0;
     
    always @(posedge CLK100MHZ)
        if(count_clk==divide)
        begin
            count_clk <=0;
            clk_1s <= ~clk_1s;
        end
        else
            count_clk <= count_clk+1;
    
endmodule
