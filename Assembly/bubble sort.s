
main:
	la   $t1, Input_data                    #t1 = input[n]
	la   $t4, Output_data                   #t4 = output[n]
	

bubbleSort:
	addi $s0, $s0, 1                   #s0 = 1
	addi $s3, $s3, 32                   #s3 = number of value = n
	
	
for1:					              
	slt  $t0, $s0, $s3                 #if(s0 < s3) t0 = 1 else t0 = 0
	beq  $t0, $zero, exist1            #if(t0 == 0) goto exist1 else skip
	addi $s1, $s0, -1                  #s1 = s0 - 1


for2:
	sge  $t0, $s1, $zero               #if(s1 >= 0) t0 = 1 else t0 = 0
	beq  $t0, $zero, exist2            #if(t0 == 0) goto exist2 else skip
	sll  $s2, $s1, 2                   #s2 = s1 * 4
	add  $t1, $t1, $s2                 #t1 = t1 + (s1 * 4) -> transfer to the (s2)th in the t1
	lw   $t2, 0($t1)                   #t2 = t1[i]
	lw   $t3, 4($t1)                   #t3 = t1[i+1]
	slt	 $t0, $t2, $t3                 #if(t2 < t3) t0 = 1 else t0 = 0
	beq	 $t0, $zero, exist3            #if(t0 == 0) goto exist3 else skip
	j swap                             #goto swap  


exist3:
	addi $s1, $s1, -1                  #s1 = s1 - 1
	sub  $t1, $t1, $s2                 #t1 = t1 - (s1 * 4) -> return to original position
	j for2                             #goto for2


exist2:
	addi $s0, $s0, 1                   #s0 = s0 + 1
	j for1                             #goto for1


exist1:
	j printOutput                      #goto printOutput


swap:
	lw   $t0, 0($t1)                   #t0 = t1[i]
	lw   $t2, 4($t1)                   #t2 = t1[i+1]
	sw   $t2, 0($t1)                   #t1[i] = t2 
	sw   $t0, 4($t1)                   #t1[i+1] = t0
	j exist3		                   #goto exist3


printOutput:
	lw   $s0, 0($t1)                   #s0 = input[i]
	sw   $s0, 0($t4)                   #output[i] = s0
	subu $s3, $s3, 1                   #n = n -1
	beq  $s3, 0, end                   #if(n == 0) goto end else skip
	addu $t1, $t1, 4                   #input[i++]
	addu $t4, $t4, 4                   #output[i++]
	j    printOutput                   #goto printOutput


end:
	j end                              #goto end


.data
.align 4
Input_data: .word 2, 0, -7, -1, 3, 8, -4, 10
			.word -9, -16, 15, 13, 1, 4, -3, 14
			.word -8, -10, -15, 6, -13, -5, 9, 12
			.word -11, -14, -6, 11, 5, 7, -2, -12

Output_data: .word 0, 0, 0, 0, 0, 0, 0, 0
			 .word 0, 0, 0, 0, 0, 0, 0, 0
			 .word 0, 0, 0, 0, 0, 0, 0, 0
			 .word 0, 0, 0, 0, 0, 0, 0, 0 