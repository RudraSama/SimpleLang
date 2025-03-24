.text


start:
	ldi A 99
	mov M A 0
	ldi A 10
	mov M A 0
	call %if_4
	hlt
	
if_4:
	mov A M 0
	ldi B 1
	adc
	push A
	mov A M 0
	ldi B 90
	adc
	push A
	pop A
	pop B
	cmp
	jnz %if_body_17
	ret
	
if_body_17:
	ldi A 9
	mov M A 0
	ret
