; hello.ll
define i32 @main(){
entry:
	%a = add i32 2,3
	%b = mul i32 4,5
	%c = sub i32 %a,%b
	ret i32 %c
}
