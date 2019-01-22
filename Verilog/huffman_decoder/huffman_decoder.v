
module huffman_decoder(y, x, clk, reset);

output [2:0] y;
input x, clk, reset;

reg [2:0] y;
reg [3:0] state;

parameter S0 = 4'b1000, S1 = 4'b1001, S2 = 4'b1010, S3 = 4'b1011, S4 = 4'b1100;
parameter A = 4'b0001, B = 4'b0010, C = 4'b0011, D = 4'b0100, E = 4'b0101, F = 4'b0110;

always @ (posedge clk or posedge reset)
if (reset == 1) state <= S0;
else case (state)
S0: if (x) state <= S1; else state <= A;
S1: if (x) state <= S3; else state <= S2;
S2: if (x) state <= B; else state <= C;
S3: if (x) state <= D; else state <= S4;
S4: if (x) state <= E; else state <= F;
 A: if (x) state <= S1; else state <= A;
 B: if (x) state <= S1; else state <= A;
 C: if (x) state <= S1; else state <= A;
 D: if (x) state <= S1; else state <= A;
 E: if (x) state <= S1; else state <= A;
 F: if (x) state <= S1; else state <= A;
endcase

always @(*)
begin

	if(state[3] == 1)
		y = 3'b000;

	else
		y = state[2:0];
end

endmodule
