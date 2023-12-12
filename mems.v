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
    input iwe,
    input [31:0]datain,
    output [31:0]data
    );
    instr_rom rom(.addra(addr[13:0]),.clka(clock),.dina(datain),.douta(data),.ena(1'b1),.wea(iwe));
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

module VGA_mem 
(
    input CLK100MHZ,
	input [11:0]debug_addr,
	output [31:0]debug_data,
    input [7:0]ascii_key,
    input clk,
	input en,
    input offset_en,
    input color_en,
    input cursor_en,
    input [11:0] cursor_data,
	input [31:0]wraddr,
    input [6:0]read_x,
    input [4:0]read_y,
    output [7:0]data,
    output [2:0]color
);
    reg [7:0] t[2239:0];
    reg [2:0] colors[2239:0];
    reg [4:0]offset;
    wire [11:0]write_addr = wraddr[11:0];
    reg [11:0] cursor_addr = 0;

	always @(posedge clk) 
	begin
		if (en) 
		begin
			t[write_addr] <= ascii_key;	
		end
        else
        begin
        end
        if (offset_en) 
        begin
            offset <= ascii_key;    
        end
        else
        begin
            	
        end
        if (color_en) 
        begin
            colors[write_addr] <= ascii_key;
        end
        else
        begin
        end	
        if (cursor_en) 
        begin
            cursor_addr <= cursor_data;    
        end
        else
        begin
            
        end
	end
	wire [11:0]read_addr;
    assign read_addr = {read_x, (read_y+ offset)&5'b11111};
	assign debug_data = {27'h0,offset};
    

    //光标的显示
    wire clk_4HZ;
    ferquency_divider f_4HZ(CLK100MHZ,2499_9999,clk_4HZ);
    reg flag_twinkle =0;
    always @(posedge clk_4HZ) 
    begin
        flag_twinkle <= ~flag_twinkle;
    end

    assign data = (read_addr == cursor_addr) ? ((flag_twinkle) ? 8'hff:t[read_addr]):t[read_addr];
    assign color = (read_addr == cursor_addr) ? 3'h0: colors[read_addr];
endmodule

module key_mem 
(
    input keyclk,
    input cpuclk,
    input will_read,
    input [7:0]ascii_key,
    output reg[7:0]cpu_data    
);
    reg [7:0] mem[63:0];
    reg [5:0] head = 0;
    reg [5:0] tail = 0;

    reg wen = 1'b1;
    reg [31:0] cnt = 0;
    reg [31:0] cnt_fast = 0;
    reg is_con = 1'b0;
    reg [7:0] pre_ascii;

    always @(negedge keyclk) 
    begin
            if (ascii_key != 0) 
            begin
                if (wen) 
                begin
                    wen <= 0;
                    cnt <= 0;
                    cnt_fast <= 0;
                    pre_ascii <= ascii_key;
                    mem[tail] <= ascii_key;
                    if (head == tail + 1) 
                    begin
                        tail = tail;
                    end
                    else
                    begin
                    tail <= tail + 1;    
                    end
                end
                else
                begin
                    if (ascii_key!=pre_ascii) 
                    begin
                        pre_ascii <= 0;
                        cnt <= 0;
                        wen <= 1;
                        is_con <=1'b0;
                    end
                    else
                    begin
                        if (is_con) 
                        begin
                            if (cnt_fast == 30000) 
                            begin
                                cnt_fast <= 0;
                                wen <= 1;    
                            end
                            else
                            begin
                                cnt_fast <= cnt_fast + 1;
                            end
                        end
                        else
                        begin
                            if (cnt==30_0000) 
                            begin
                                cnt <= 0;
                                wen <= 1;
                                is_con <=1'b1;
                            end
                            else
                            begin
                                cnt <= cnt + 1;
                            end   
                        end
                    end
                end
            end
            else
            begin
                wen <=1;
                pre_ascii <= 0;
                cnt <= 0;
                is_con <= 0;
            end
    end

    always @(posedge cpuclk) 
    begin
        if (will_read) 
        begin
            if (head == tail) 
            begin
                cpu_data <= 0;
            end
            else
            begin
                cpu_data <= mem[head];
                head = head + 1; 
            end
        end
        else
        begin
            if (head == tail) 
            begin
                cpu_data <= 0;    
            end
            else
            begin
                cpu_data <= mem[head];
            end
                
        end       
    end 
endmodule