
module addsub_cla(A,B,S,C,M,V);
parameter W = 4;

input [W-1:0] A, B;
output [W-1:0] S;
output C;
input M;
output V;

reg [W-1:0] S;
reg [W-1:0] P;
reg [W-1:0] G;
reg [W-1:0] B_xor_M;

wire [W:0] carry;

integer i;

cla_gen #(.W(W)) CLAGEN(.P(P), .G(G), .C0(M), .C(carry));

always @(*) begin
for (i = 0; i < W; i = i + 1) begin
    B_xor_M[i] = (~B[i] & M) | (B[i] & ~M);
   	 P[i] = (~A[i] & B_xor_M[i]) | (A[i] & ~B_xor_M[i]);
    	G[i] = A[i] & B_xor_M[i];
    	S[i] = (~P[i] & carry[i]) | (P[i] & ~carry[i]);
  	end
end

assign C = carry[W];
assign V = (~carry[W] & carry[W-1]) | (carry[W] & ~carry[W-1]) ;

endmodule


module cla_gen(P,G,C0,C);
parameter W = 4;

input [W-1:0] P, G;
input C0;
output [W:0] C;

wire [W:0] temp;

genvar i;
generate
	for(i = 0; i < W; i = i + 1)
	begin: inst_cla
		cla Icla(.P(P[i]), .G(G[i]), .C0(temp[i]), .C(temp[i+1]));
	end
endgenerate

assign temp[0] = C0;
assign C = temp;

endmodule


module cla(P,G,C0,C);

input P, G, C0;
output C;

wire P;
wire G;
wire C0;

assign C = G | (P & C0);

endmodule











