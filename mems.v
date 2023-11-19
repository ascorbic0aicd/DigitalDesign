`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/19 13:17:45
// Design Name: 
// Module Name: mems
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


module instr_mem(
    input clock,
    input [31:0]addr,
    output [31:0]data
    );
    instr_rom rom(.addra(addr[12:0]),.clka(clk),.douta(data),.ena(1'b1));

endmodule

module data_mem(
	input  [31:0] addr,
	output reg [31:0] dataout,
	input  [31:0] datain,
	input  rdclk,
	input  wrclk,
	input [2:0] memop,
	input we);

	wire [31:0] memin;
	reg  [3:0] wmask;
	wire [7:0] byteout;
	wire [15:0] wordout;
	wire [31:0] dwordout;
 

    assign memin = (memop[1:0]==2'b00)?{4{datain[7:0]}}:((memop[1:0]==2'b10)?datain:{2{datain[15:0]}}) ; //lb: same for all four, lh:copy twice; lw:copy

    //four memory chips	
    blk_mem_gen_0 mymem(.wea(wmask),.dina(memin), .addrb(addr[16:2]), .clkb(rdclk), .addra(addr[16:2]), .clka(wrclk), .ena(we), .doutb(dwordout), .enb(1'b1));
    //wmask,addr[16:2]
    assign wordout = (addr[1]==1'b1)? dwordout[31:16]:dwordout[15:0];

    assign byteout = (addr[1]==1'b1)? ((addr[0]==1'b1)? dwordout[31:24]:dwordout[23:16]):((addr[0]==1'b1)? dwordout[15:8]:dwordout[7:0]);


    always @(*)
    begin
      case(memop)
      3'b000: //lb
         dataout = { {24{byteout[7]}}, byteout};
      3'b001: //lh
         dataout = { {16{wordout[15]}}, wordout};
      3'b010: //lw
         dataout = dwordout;
      3'b100: //lbu
         dataout = { 24'b0, byteout};
      3'b101: //lhu
         dataout = { 16'b0, wordout};
      default:
         dataout = dwordout;
      endcase
    end

    always@(*)
    begin
    	if(we==1'b1)
    	begin
    		case(memop)
    			3'b000://sb
    			begin
    				wmask[0]=(addr[1:0]==2'b00)?1'b1:1'b0;
    				wmask[1]=(addr[1:0]==2'b01)?1'b1:1'b0;
    				wmask[2]=(addr[1:0]==2'b10)?1'b1:1'b0;
    				wmask[3]=(addr[1:0]==2'b11)?1'b1:1'b0;
    			end
    			3'b001://sh
    			begin
    				wmask[0]=(addr[1]==1'b0)?1'b1:1'b0;
    				wmask[1]=(addr[1]==1'b0)?1'b1:1'b0;
    				wmask[2]=(addr[1]==1'b1)?1'b1:1'b0;
    				wmask[3]=(addr[1]==1'b1)?1'b1:1'b0;
    			end		
    			3'b010://sw
    			begin
    				wmask=4'b1111;
    			end
    			default:
    			begin
    				wmask=4'b0000;
    			end
    		endcase
    	end
    	else
    	begin
    	   wmask=4'b0000;
    	end
    end
    
endmodule