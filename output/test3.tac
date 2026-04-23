// === Three-Address Intermediate Code ===
BEGIN:
	t0 = 0
	i = t0
	t1 = 0
	sum = t1
L0:
	t2 = 5
	t3 = i <= t2
	ifFalse t3 goto L1
	t4 = sum + i
	sum = t4
	t5 = 1
	t6 = i + t5
	i = t6
	goto L0
L1:
	print sum
	t7 = 0
	j = t7
	t8 = 10
	j = t8
L2:
	t9 = 0
	t10 = j > t9
	ifFalse t10 goto L3
	print j
	t11 = 2
	t12 = j - t11
	j = t12
	goto L2
L3:
END:
