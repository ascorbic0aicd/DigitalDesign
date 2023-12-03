`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2017/10/17 16:56:25
// Design Name: 
// Module Name: serial_rx
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


module serial_rx(
    input clk,
    input txd_in,
    output reg [7:0] rx_data,
    output reg rx_finish,
    input rx_ready
    );

wire rx_clk;
reg [2:0] clk_sync;
reg [2:0] start_sync;
reg [3:0] rx_count;
reg [10:0] rx_buf;
wire sampling;
wire start_bit;
reg rx_busy;
reg rx_clk_rst;

initial
begin
  rx_finish =1'b0;
  rx_busy =1'b0;
  rx_clk_rst =1'b0;
end

clkgen #(115200) mytxclk(clk,rx_clk_rst,rx_busy,rx_clk); //rst the clk at negedge

always @ (posedge clk)
begin
    clk_sync <=  {clk_sync[1:0],rx_clk};
end
assign sampling = ~clk_sync[2] & clk_sync[1]; //sampling of receive clock, posedge because we start the clock at begining of the bit

always @ (posedge clk)
begin
    start_sync <=  {start_sync[1:0],txd_in};
end
assign start_bit = start_sync[2] & ~start_sync[1]; //find negedge of start bit


always @ (posedge clk) //triger the clock when find negedge
begin
    if(rx_busy) //receiving
    begin
        rx_clk_rst<=1'b0; //reset only last one clock
        if(rx_finish)
        begin
            rx_busy<=1'b0;
        end
        else
        begin
            rx_busy<=rx_busy;
            if(sampling)
            begin
                if(rx_count>=4'd10)
                begin
                    if( (rx_buf[0]==0)&&(^rx_buf[9:1]) )
                    begin
                        rx_data[7:0]<=rx_buf[8:1];
                        rx_finish<=1'b1;
                    end
                end
                else
                begin
                    rx_buf[rx_count]<= txd_in;
                    rx_count <= rx_count + 4'd1;
                end
            end
        end
    end
    else//not busy
    begin  
       if(start_bit & (~rx_finish)) //start receiving when find start bit
       begin
            rx_busy<=1'b1;
            rx_clk_rst<=1'b1;
            rx_count<=4'd0;
            rx_data<=8'd0;
            rx_finish<=1'b0;
       end
       else
       begin
            rx_busy<=1'b0;
            rx_clk_rst<=1'b0;
       end
       if(rx_ready)
       begin
           rx_finish<=1'b0; 
       end
    end
end



endmodule
