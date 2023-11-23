`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/19 13:11:41
// Design Name: 
// Module Name: cpu
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


module cpu(
	input 	clock,
	input 	reset,
	output [31:0] imemaddr,
	input  [31:0] imemdataout,
	output 	imemclk,
	output [31:0] dmemaddr,
	input  [31:0] dmemdataout,
	output [31:0] dmemdatain,
	output 	dmemrdclk,
	output	dmemwrclk,
	output [2:0] dmemop,
	output	dmemwe,
	output [31:0] dbgdata);

    //pc寄存器
    reg [31:0]PC = 0;
    
    //译码器
    wire [6:0]op;
    wire [4:0]rs1;
    wire [4:0]rs2;
    wire [4:0]rd;
    wire [2:0]func3;
    wire [6:0]func7;
    

    assign op = imemdataout[6:0];
    assign rs1 = imemdataout[19:15];
    assign rs2 = imemdataout[24:20];
    assign rd = imemdataout[11:7];
    assign func3 = imemdataout[14:12];
    assign func7 = imemdataout[31:25];


    wire [2:0]ExtOp;
    wire RegWr;
    wire [3:0]ALUctr;
    wire [1:0]ALUBsrc;
    wire ALUAsrc;
    wire [2:0]Branch;
    wire MemtoReg;
    wire MemWr;
    wire [2:0]MemOP;
    
    wire [31:0]Imm;

    wire [31:0] busA;
    wire [31:0] busB;
    wire [31:0] busW;
    
    wire less;
    wire zero;
    
    wire [31:0]Result;
    reg [31:0]ta;
    reg [31:0]tb;
    wire [31:0]dataa;
    wire [31:0]datab;

    ContrGen contr_gen(.op(op), .func3(func3), .func7(func7), .ExtOp(ExtOp), .RegWr(RegWr), 
                .ALUAsrc(ALUAsrc), .ALUBsrc(ALUBsrc), .ALUctr(ALUctr),.Branch(Branch), .MemtoReg(MemtoReg), .MemWr(MemWr), 
                .MemOP(MemOP));
    Immgen immGen(ExtOp,imemdataout,Imm);

    assign busW = MemtoReg ? dmemdataout: Result;
    RegFile myregfile(.Ra(rs1),
                      .Rb(rs2),
                      .Rw(rd),
                      .RegWr(RegWr),
                      .clk(clock),
                      .busW(busW),
                      .busA(busA),
                      .busB(busB));

    
    always @(*) 
    begin
        case(ALUBsrc)
            2'b00: tb = busB;
            2'b01: tb = Imm;        
            2'b10: tb = 4;
            default: tb = 0;
        endcase

        if (ALUAsrc) 
        begin
            ta = PC;    
        end
        else
            ta = busA;
    end
    assign dataa = ta;
    assign datab = tb;

    ALU myALU(dataa,datab,ALUctr,less,zero,Result);
    wire PCAsrc;
    wire PCBsrc;

    BranchCond myBranchCond(Branch,less,zero,PCAsrc,PCBsrc);

    wire [31:0]nextPC;
    NextpcGen myNextGen(Imm, busA, PC, PCAsrc, PCBsrc, nextPC);
    //test
    
    always @(negedge clock) 
    begin
        if(reset)
        begin
            PC <= 0;
        end
        else
        begin
           PC <= nextPC;
        end
    end

    //test
    assign dbgdata = PC;
    assign imemclk = ~clock;
    assign dmemwrclk = ~clock;
    assign dmemrdclk = clock;
    //test
    assign imemaddr = reset ? 0 : nextPC;
    assign dmemwe = MemWr;
    assign dmemdatain = busB;
    assign dmemaddr = Result;
    assign dmemop = MemOP;
endmodule

module NextpcGen (
    input [31:0]Imm,
    input [31:0]rs1,
    input [31:0]PC,
    input PCAsrc, 
    input PCBsrc,
    output [31:0]nextPC 
);
    wire [31:0]a;
    wire [31:0]b;
    assign a = PCAsrc ? Imm : 4;
    assign b = PCBsrc ? rs1 : PC;
    
    wire cf;
    wire of;
    wire zero;
    adder pcadder(a, b, 1'b0, nextPC, cf, zero, of);

endmodule
module BranchCond (
    input [2:0]Branch,
    input less,
    input zero,
    output reg PCAsrc,
    output PCBsrc
);
    assign PCBsrc = !(|(Branch^3'b010));
    always @(*) 
    begin
        case(Branch)
            3'b000:PCAsrc = 1'b0;
            3'b001:PCAsrc = 1'b1;
            3'b010:PCAsrc = 1'b1;
            3'b100:PCAsrc = zero;
            3'b101:PCAsrc = ~zero;
            3'b110:PCAsrc = less;
            3'b111:PCAsrc = ~less;
            default:PCAsrc = 1'b0;
        endcase    
    end
endmodule

module ContrGen(
    input [6:0]op,    
    input [2:0]func3,
    input [6:0] func7,
    output reg [2:0]ExtOp, 
    output reg RegWr,
    output reg ALUAsrc,
    output reg [1:0]ALUBsrc,
    output reg [3:0]ALUctr,
    output reg [2:0]Branch,
    output reg MemtoReg,
    output reg MemWr,
    output reg [2:0]MemOP
);
parameter [4:0]lui = 5'b01101;
parameter [4:0]auipc = 5'b00101;
parameter [4:0]I_type = 5'b00100;
parameter [4:0]R_type = 5'b01100;
parameter [4:0]B_type = 5'b11000;
parameter [4:0]L_type = 5'b00000;
parameter [4:0]S_type = 5'b01000;
parameter [4:0]jal = 5'b11011;
parameter [4:0]jalr= 5'b11001;
    always @(*) 
    begin
        
        //生成MemWr
        if(op[6:2]==S_type)
            MemWr = 1;
        else
            MemWr = 0;
        
        //生成MemtoReg
        if(op[6:2] == L_type)
            MemtoReg = 1;
        else
            MemtoReg = 0;
        
        //生成MemOP
        MemOP = func3;
        
        //生成branch
        if(op[6:2] == B_type)
        begin
            case (func3)
                3'b000: Branch = 3'b100;
                3'b001: Branch = 3'b101;
                3'b100: Branch = 3'b110;
                3'b101: Branch = 3'b111;
                3'b110: Branch = 3'b110;
                3'b111: Branch = 3'b111;
                default: Branch = 3'b000;
            endcase
        end
        else if (op[6:2] == jal) 
        begin
            Branch = 3'b001;    
        end
        else if(op[6:2]==jalr)
        begin
            Branch = 3'b010;
        end
        else
        begin
            Branch = 3'b000;
        end

        //生成RegWr

        if(op[6:2]==B_type||op[6:2]==S_type)
            RegWr = 1'b0;
        else
            RegWr = 1'b1;

        //生成ExtOP
        case (op[6:2])
            lui: ExtOp = 3'b001;
            auipc: ExtOp = 3'b001;
            I_type: ExtOp = 3'b000;
            jal: ExtOp = 3'b100;
            jalr: ExtOp = 3'b000;
            B_type: ExtOp = 3'b011;
            L_type: ExtOp = 3'b000;
            S_type: ExtOp = 3'b010;
            default: ExtOp = 3'b111;
        endcase
        
        //生成ALUAsrc
        if(op[6:2] == auipc || op[6:2] == jal|| op[6:2] == jalr)
            ALUAsrc = 1'b1;
        else
            ALUAsrc = 1'b0;
        
        //生成ALUBsrc
        case (op[6:2])
            
            R_type:ALUBsrc = 2'b00;
            B_type: ALUBsrc = 3'b00;

            lui: ALUBsrc = 2'b01;
            auipc: ALUBsrc = 2'b01;
            I_type: ALUBsrc = 2'b01;
            L_type: ALUBsrc = 2'b01;
            S_type: ALUBsrc = 2'b01;
            jal: ALUBsrc = 3'b10;
            jalr: ALUBsrc = 3'b10;
        
            default: ALUBsrc = 3'b11;
        endcase

        //生成ALUctr
        case (op[6:2])
            L_type: ALUctr = 4'b0000;
            S_type: ALUctr = 4'b0000;
            jal: ALUctr = 4'b0000;
            jalr: ALUctr = 4'b0000;
            auipc: ALUctr = 4'b0000;

            R_type:
                begin
                    if(func3 == 3'b011)
                        ALUctr = 4'b1010;
                    else
                        ALUctr = {func7[5],func3};
                end
            I_type:
                begin
                   if(func3 == 3'b011)
                        ALUctr = 4'b1010;
                    else if(func3 == 3'b101)
                        ALUctr = {func7[5],func3};
                    else
                        ALUctr = {1'b0,func3}; 
                end
            lui: ALUctr = 4'b0011;
            B_type:
                begin
                    if(func3 == 3'b110||func3 == 3'b111)
                        ALUctr = 4'b1010;
                    else
                        ALUctr = 4'b0010;
                end 
            default: ALUctr = 4'b1111;
        endcase
    end
endmodule
module ALU(
	input [31:0] dataa,
	input [31:0] datab,
	input [3:0]  ALUctr,
	output less,
	output zero,
	output reg [31:0] aluresult);
    wire [31:0]out_ALU;
    wire [31:0]out_b;
    wire cf,of,z;
    adder a(dataa,datab,ALUctr[3]|ALUctr[1],out_ALU,cf,zero,of);
    barrel b(dataa,datab[4:0],~ALUctr[2],ALUctr[3],out_b);
    wire z1;
    always @(*)
    begin
        casez(ALUctr)
        4'b?000:
        begin
        aluresult = out_ALU;
        end
        4'b0001:
        begin
            aluresult = out_b;
        end
        4'b1001:
        begin
            aluresult = out_b;
        end
        4'b0010:
        begin
            aluresult = {32{out_ALU[31]^of}} & 32'h00000001;
        end
        4'b1010:
        begin
            aluresult = {32{cf^1'b0}} & 32'h00000001;
        end
        4'b?011:
        begin
            aluresult = datab;
        end
        4'b?100:
        begin
            aluresult = dataa ^ datab;
        end
        4'b?101:
        begin
            aluresult = out_b;
        end
        4'b?110:
        begin
            aluresult = dataa | datab;
            
        end
        4'b?111:
        begin
            aluresult = dataa & datab;
        end
        default:
        aluresult = 32'h12345678;
        endcase
    end
    assign less = aluresult[0];
endmodule

module adder(
	input  [31:0] A,
	input  [31:0] B,
	input  addsub,
	output [31:0] F,
	output cf,
	output zero,
	output of

	);
    wire cout;

    wire [31:0]B1;
    assign B1 = ({32{addsub}}^B);  
    assign {cout,F} = A + B1 + addsub;
    assign cf = cout ^ addsub;
    assign zero = !(|F); 
    assign of =  (A[31] == B1[31]) && (F[31] != A[31]);
endmodule

module barrel(input [31:0] indata,
  input [4:0] shamt,
  input lr,
  input al,
  output reg [31:0] outdata);

    reg [31:0]temp1;
    reg [31:0]temp2;
    reg [31:0]temp3;
    reg [31:0]temp4;
    reg [31:0]temp5;

    always @(*)
    begin
        if(lr)
        begin
            temp1 = shamt[0]?{indata[30:0],1'b0}:indata;
            temp2 = shamt[1]?{temp1[29:0],2'b00}:temp1;
            temp3 = shamt[2]?{temp2[27:0],4'b0000}:temp2;
            temp4 = shamt[3]?{temp3[23:0],8'b0000_0000}:temp3;
            outdata = shamt[4]?{temp4[15:0],16'b0000_0000_0000_0000}:temp4;
        end
        else
        begin
            if(al)
            begin
                temp1 = shamt[0]?{{1{indata[31]}}, indata[31:1]}:indata;
                temp2 = shamt[1]?{{2{indata[31]}}, temp1[31:2]}:temp1;
                temp3 = shamt[2]?{{4{indata[31]}}, temp2[31:4]}:temp2;
                temp4 = shamt[3]?{{8{indata[31]}}, temp3[31:8]}:temp3;
                outdata = shamt[4]?{{16{indata[31]}}, temp4[31:16]}:temp4;     
            end
            else
            begin
                temp1 = shamt[0]?{{1{1'b0}}, indata[31:1]}:indata;
                temp2 = shamt[1]?{{2{1'b0}}, temp1[31:2]}:temp1;
                temp3 = shamt[2]?{{4{1'b0}}, temp2[31:4]}:temp2;
                temp4 = shamt[3]?{{8{1'b0}}, temp3[31:8]}:temp3;
                outdata = shamt[4]?{{16{1'b0}},temp4[31:16]}:temp4;
            end
        end
    end
endmodule

module Immgen(
    input [2:0]ExtOp,
    input [31:0]instr,
    output reg [31:0]Imm
);
    wire [31:0]immI;
    wire [31:0]immU;
    wire [31:0]immS;
    wire [31:0]immB;
    wire [31:0]immJ;

    assign immI = {{20{instr[31]}}, instr[31:20]};
    assign immU = {instr[31:12], 12'b0};
    assign immS = {{20{instr[31]}}, instr[31:25], instr[11:7]};
    assign immB = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
    assign immJ = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
    
    always @(*) 
    begin
        case(ExtOp)
        3'b000: Imm = immI;
        3'b001: Imm = immU;
        3'b010: Imm = immS;
        3'b011: Imm = immB;
        3'b100: Imm = immJ;
        default: Imm = 3'b000;
        endcase
    end
endmodule

module RegFile(
    input [4:0]Ra,
    input [4:0]Rb,
    input [4:0]Rw,
    input RegWr,
    input clk,
    input [31:0]busW,
    output [31:0]busA,    
    output [31:0]busB
);
    reg [31:0]regs[31:0];
    initial 
    begin
        regs[0] = 0;    
    end
    always @(negedge clk) 
    begin
        if(RegWr&&(|(Rw)))
        begin
            regs[Rw] <= busW;
        end   
    end

    assign busA = regs[Ra];
    assign busB = regs[Rb];    
endmodule