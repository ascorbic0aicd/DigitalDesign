`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/27 17:39:06
// Design Name: 
// Module Name: keyboard
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

module keyboard(input clk,
	input clrn,
	input ps2_clk,
	input ps2_data,
	output [7:0] ascii_key
    );

    reg [7:0] cur_key = 8'h00;
    reg nextdata_n = 1'b1;
    wire overflow;
    wire [7:0] keydata;
    wire ready;
    wire [23:0]test;

    reg CTRL = 1'b0;
    reg SHIFT = 1'b0;
    reg CapsLock = 1'b0;

    reg in_UP = 1'b0;
    //----DO NOT CHANGE BEGIN----
    //scancode to ascii conversion, will be initialized by the testbench
    scancode_ram myram(clk, in_UP ^ SHIFT,cur_key, ascii_key);
    //PS2 interface, you may need to specify the inputs and outputs
    ps2_keyboard mykey(clk, clrn, ps2_clk, ps2_data, keydata, ready, nextdata_n, overflow,test);
    
    //---DO NOT CHANGE END-----

    parameter [7:0]ctrl_d = 8'h14;
    parameter [7:0]shift_d = 8'h12;
    parameter [7:0]CapsLock_d = 8'h58;

    //parameter [7:0]enter_d = 8'h5a;

    reg will_release = 1'b0;
     
    
    always @(posedge clk) 
    begin
        //nextdata_n <=1'b1;
        if(ready&&nextdata_n == 1'b1)
        begin
                nextdata_n <=1'b0;
                if(keydata!=16'hF0)
                begin
                    if(!will_release)
                    begin
                        case (keydata)
                            shift_d: 
                            begin                            
                                SHIFT <= 1'b1;
                            end
                            ctrl_d:
                            begin
                                CTRL <= 1'b1;
                            end
                            CapsLock_d:
                            begin
                                if (!CapsLock) 
                                begin
                                    in_UP <= ~in_UP;
                                end
                                CapsLock <= 1'b1; 
                            end
                            default:
                            begin
                                cur_key<=keydata;    
                            end
                        endcase
                    end
                    else
                    begin
                        case (keydata)
                            shift_d: SHIFT <= 1'b0;
                            ctrl_d: CTRL <= 1'b0;
                            CapsLock_d: CapsLock <= 1'b0;
                            default:cur_key <= 16'h00; 
                        endcase
                        will_release <=1'b0;
                    end
                end
        
                else
                begin
                    will_release <= 1'b1;
                end       
        end
        else
        begin
            nextdata_n = 1'b1;
        end
    end
    //assign LED[3:0] = {in_UP^SHIFT,CTRL,SHIFT,CapsLock};
endmodule
//standard ps2 interface, you can keep this
module ps2_keyboard(clk,clrn,ps2_clk,ps2_data,data,ready,nextdata_n,overflow,test);
    input clk,clrn,ps2_clk,ps2_data;
	input nextdata_n;
    output [7:0] data;
    output reg ready;
    output reg overflow;     // fifo overflow  
    output reg [23:0]test = 24'h000000;
    // internal signal, for test
    reg [9:0] buffer;        // ps2_data bits
    reg [7:0] fifo[7:0];     // data fifo
	reg [2:0] w_ptr,r_ptr;   // fifo write and read pointers	
    reg [3:0] count;  // count ps2_data bits
    // detect falling edge of ps2_clk
    reg [2:0] ps2_clk_sync;
    
    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];
    
    always @(posedge clk) begin
        if (clrn == 0) begin // reset 
            count <= 0; w_ptr <= 0; r_ptr <= 0; overflow <= 0; ready<= 0;
        end 
		else if (sampling) begin
            if (count == 4'd10) begin
                if ((buffer[0] == 0) &&  // start bit
                    (ps2_data)       &&  // stop bit
                    (^buffer[9:1])) begin      // odd  parity
                    fifo[w_ptr] <= buffer[8:1];  // kbd scan code
                    w_ptr <= w_ptr+3'b1;
                    ready <= 1'b1;
                    test = {test[15:0],buffer[8:1]};
                    overflow <= overflow | (r_ptr == (w_ptr + 3'b1));
                end
                count <= 0;     // for next
            end else begin
                buffer[count] <= ps2_data;  // store ps2_data 
                
                count <= count + 3'b1;
            end      
        end
        if (ready) begin // read to output next data
				if(nextdata_n == 1'b0) //read next data
				begin
				   r_ptr <= r_ptr + 3'b1; 
					if(w_ptr==(r_ptr + 3'b1)) //empty
					     ready <= 1'b0;
				end           
        end
    end

    assign data = fifo[r_ptr];
endmodule 
//simple scancode converter
module scancode_ram(input clk, 
                    input is_shift, 
                    input [7:0]addr,
                    output reg[7:0] outdata);
    
    reg [7:0] ascii_tab[255:0];
    reg [7:0] ascii_tab_shift[255:0];
    initial 
    begin
        $readmemh("./data/scancode.txt", ascii_tab, 0, 255);
        $readmemh("./data/scancode_shift.txt", ascii_tab_shift, 0, 255);
    end
    always @(posedge clk)
    begin
        if(is_shift) 
        begin
            outdata <= ascii_tab_shift[addr];  
        end
        else
        begin
            outdata <= ascii_tab[addr];
        end
          
    end
endmodule
