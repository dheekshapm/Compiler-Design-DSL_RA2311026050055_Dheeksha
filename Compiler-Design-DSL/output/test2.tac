// === Three-Address Intermediate Code ===
BEGIN:
	t0 = 50
	x = t0
	t1 = 30
	y = t1
	t2 = 0
	max = t2
	t3 = x > y
	ifFalse t3 goto L0
	max = x
	goto L1
L0:
	max = y
L1:
	print max
END:
