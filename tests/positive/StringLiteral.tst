
auto gA = "global string";

int main()
{
	auto str1 = "double quote";
	auto str2 = `back tick`;
	auto str3 = "žščř";

	return 0;
}

void func()
{
	auto a = "1\n23\n\n45\\6";
	auto b = "--\\\\--";
	auto c = "-\0-\a-\b-\e-\f-\n-\r-\t-\v-";
}

========

@0 = private constant [14 x i8] c"global string\00"
@gA = global [14 x i8]* @0
@1 = private constant [13 x i8] c"double quote\00"
@2 = private constant [10 x i8] c"back tick\00"
@3 = private constant [9 x i8] c"\C5\BE\C5\A1\C4\8D\C5\99\00"
@4 = private constant [11 x i8] c"1\0A23\0A\0A45\5C6\00"
@5 = private constant [7 x i8] c"--\5C\5C--\00"
@6 = private constant [20 x i8] c"-\00-\07-\08-\1B-\0C-\0A-\0D-\09-\0B-\00"

define i32 @main() {
  %str1 = alloca [13 x i8]*
  store [13 x i8]* @1, [13 x i8]** %str1
  %str2 = alloca [10 x i8]*
  store [10 x i8]* @2, [10 x i8]** %str2
  %str3 = alloca [9 x i8]*
  store [9 x i8]* @3, [9 x i8]** %str3
  ret i32 0
}

define void @func() {
  %a = alloca [11 x i8]*
  store [11 x i8]* @4, [11 x i8]** %a
  %b = alloca [7 x i8]*
  store [7 x i8]* @5, [7 x i8]** %b
  %c = alloca [20 x i8]*
  store [20 x i8]* @6, [20 x i8]** %c
  ret void
}
