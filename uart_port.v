`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/08/22 07:49:04
// Design Name: 
// Module Name: uart_port
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


module uart_port(input clk,
                 input clk100m,
                 output rxd_out,
                 input txd_in,
                 output reg [31:0] dbgdata,
                 input rst,
                 output [31:0] instaddr,
                 output reg[31:0] instdata,
                 output reg iwe,
                 output [31:0] dataaddr,
                 output[31:0] mdata,
                 output reg dwe,
                 output reg cpuhalt,
                 output reg cpureset
    );

    wire tx_ready;
    wire rx_ready;
    reg  rx_gotdata;
    reg  tx_send;
    reg  [7:0] tx_buf[15:0];
    reg  [7:0] rx_buf[15:0];
    reg  [3:0] tx_head;
    reg  [3:0] tx_tail;
    reg  [3:0] rx_head;
    reg  [3:0] rx_tail;
    reg  [3:0] uart_state;
    reg  [7:0] rst_cnt;
    wire [7:0] rx_data;
    reg  [7:0] tx_data;
    wire [7:0] temp1;
    wire [7:0] temp2;
    wire [7:0] temp3;
    reg  [31:0] instcnt;
    reg  data_inst;

initial
begin
  tx_buf[0]=8'h55; //U
  tx_buf[1]=8'h41; //A
  tx_buf[2]=8'h52; //R
  tx_buf[3]=8'h54; //T
  tx_buf[4]=8'h20;
  tx_buf[5]=8'h4f;
  tx_buf[6]=8'h4b;
  tx_buf[7]=8'h0d;
  tx_buf[8]=8'h0a;
  tx_head=4'd0; tx_tail=4'd9;
  rx_head=4'd0; rx_tail=4'd0;
  uart_state=4'd0;
  tx_send=1'b0; rx_gotdata=1'b0;
  cpuhalt=1'b0;cpureset=1'b0;iwe=1'b0;dwe=1'b0;
  data_inst=1'b0;
end


serial_tx tx_module(clk100m, tx_data, tx_send, rxd_out,tx_ready);
 
serial_rx rx_module(clk100m, txd_in, rx_data, rx_ready, rx_gotdata);

assign temp1=rx_buf[rx_head]-8'h30;
assign temp2=rx_buf[rx_head]-8'h37;
assign temp3=rx_buf[rx_head]-8'h57;
assign instaddr={instcnt[29:0],2'b0};
assign dataaddr=instaddr;   //use the same data and address for inst and data
assign mdata= instdata;     // only difference is the write enable signal
// send and receive logic
always@(posedge clk)
begin
    if(rst)
    begin
        tx_head<=4'd0;rx_tail<=4'd0;
        tx_send<=1'b0; rx_gotdata<=1'b0;
    end
    else
    begin
      if(rx_ready)
          begin
             if(rx_gotdata==1'b0)
             begin
                rx_buf[rx_tail]<=rx_data;
                rx_tail<=rx_tail+4'd1;
                //dbgdata<={tx_head,rx_head,dbgdata[15:0],rx_data};
                rx_gotdata<=1'b1;
             end
          end
      else
          begin
             rx_gotdata<=1'b0;
          end
      if(tx_head!=tx_tail)//has data to send
          begin
             if(~tx_send&tx_ready) //tx free
             begin
                tx_data <= tx_buf[tx_head];
                tx_head<= tx_head+4'd1;
                tx_send<=1'b1;
             end
             else
             begin
                  if(~tx_ready) tx_send<=1'b0;
             end
          end
       else
          begin
            if(~tx_ready)
               begin
                  tx_send<=1'b0;
               end
          end 
          
    end
end


// processing data
always @(posedge clk)
begin
    if(rst)
    begin
       tx_tail<=4'd9;
       rx_head<=4'd0; cpuhalt=1'b0;cpureset=1'b0;iwe=1'b0;dwe=1'b0; 
       uart_state<=4'd0;  
    end
    else
    case(uart_state)
    4'd0: //idle case
    begin
      if(rx_head!=rx_tail)  //get inputs
      begin
         case(rx_buf[rx_head])
         8'h69: //'i' update inst mem
         begin
           tx_buf[tx_tail]<=8'h69; tx_tail<=tx_tail+4'd1;
           data_inst<=1'b1;
           uart_state<=4'd2; cpuhalt<=1'b1;instcnt<=32'b0;instdata<=32'b0;
         end
         8'h72: //'r' reset
          begin
           tx_buf[tx_tail]<=8'h72; tx_tail<=tx_tail+4'd1;
           uart_state<=4'd5; cpureset<=1'b1; rst_cnt<=8'd0;
         end
         8'h64: //'d' update data mem
         begin
           tx_buf[tx_tail]<=8'h64; tx_tail<=tx_tail+4'd1;
           uart_state<=4'd2; cpuhalt<=1'b1;instcnt<=32'b0;instdata<=32'b0;
           data_inst=1'b0;
         end
         default:
         begin
            uart_state<=4'd0;
         end
         endcase
         rx_head<=rx_head+4'd1;
      end
    end
    
    4'd1:  //cpu reset return 
    begin
       uart_state<=4'd0; cpureset<=1'b0; cpuhalt<=1'b0;iwe<=1'b0;dwe<=1'b0;//return to normal
    end
    
    4'd2:  //inst write inputs
    begin
       if(rx_head!=rx_tail)  //get inputs
       begin
          case(rx_buf[rx_head])
          8'h71: //'q' exit
          begin
            uart_state<=4'd5; cpureset<=1'b1; cpuhalt<=1'b0;rst_cnt<=8'd0;
            /*tx_buf[tx_tail]<=8'h71; tx_tail<=tx_tail+4'd1;*/
          end
          8'h40: //'@' addr
          begin
            uart_state<=4'd3; instdata<=32'd0;instcnt<=32'd0;
          end
          8'h0a: //return
          begin
            uart_state<=4'd4; iwe<=1'd1&data_inst; dwe<=1'd1&(~data_inst);
          end
          8'h30,8'h31,8'h32,8'h33,8'h34,8'h35,8'h36,8'h37,8'h38,8'h39:
          begin
            instdata<={instdata[27:0],temp1[3:0]};
          end
          8'h41,8'h42,8'h43,8'h44,8'h45,8'h46:
          begin
            instdata<={instdata[27:0],temp2[3:0]};
          end
          8'h61,8'h62,8'h63,8'h64,8'h65,8'h66:
          begin
            instdata<={instdata[27:0],temp3[3:0]};
          end
          default:
          begin
            //do nothing  
          end
          endcase
          rx_head<=rx_head+4'd1;
       end
    end
    
    4'd3:  //inst addr
    begin
      if(rx_head!=rx_tail)  //get inputs
       begin
          case(rx_buf[rx_head])
          8'h0a: //return
          begin
            uart_state<=4'd2;
          end
          8'h30,8'h31,8'h32,8'h33,8'h34,8'h35,8'h36,8'h37,8'h38,8'h39:
          begin
            instcnt<={instcnt[27:0],temp1[3:0]};
          end
          8'h41,8'h42,8'h43,8'h44,8'h45,8'h46:
          begin
            instcnt<={instcnt[27:0],temp2[3:0]};
          end
          8'h61,8'h62,8'h63,8'h64,8'h65,8'h66:
          begin
            instcnt<={instcnt[27:0],temp3[3:0]};
          end
          default:
          begin
            //do nothing  
          end
          endcase
          rx_head<=rx_head+4'd1;
       end
    end
    
    4'd4:  //write memory data
    begin
       instcnt<=instcnt+32'd1; instdata<=32'd0; iwe<=1'd0; dwe<=1'd0;
       uart_state<=4'd2;
    end
    
    4'd5: //reset delay
    begin
       rst_cnt<=rst_cnt+8'd1;
       if(rst_cnt>=8'd120) begin uart_state<=4'd1; end
    end
    
   
    default:
    begin
       uart_state<=4'd0;
    end
    endcase
    dbgdata<={instaddr[15:0],instdata[15:0]};
end
endmodule
