
double other()
{
	uint64 a = 7;
	int32 b = 3;
	int16 c = 5;

	a += c;

	return a / b;
}

int main()
{
	uint8 a = 4;
	uint b = 6;
	uint16 c = 7;
	uint32 d = 9;

	d += b + c;

	return a % b;
}

========

define double @other() {
  %a = alloca i64
  %1 = sext i32 7 to i64
  store i64 %1, i64* %a
  %b = alloca i32
  store i32 3, i32* %b
  %c = alloca i16
  %2 = trunc i32 5 to i16
  store i16 %2, i16* %c
  %3 = load i16* %c
  %4 = load i64* %a
  %5 = sext i16 %3 to i64
  %6 = add i64 %4, %5
  store i64 %6, i64* %a
  %7 = load i64* %a
  %8 = load i32* %b
  %9 = sext i32 %8 to i64
  %10 = udiv i64 %7, %9
  %11 = uitofp i64 %10 to double
  ret double %11
}

define i32 @main() {
  %a = alloca i8
  %1 = trunc i32 4 to i8
  store i8 %1, i8* %a
  %b = alloca i32
  store i32 6, i32* %b
  %c = alloca i16
  %2 = trunc i32 7 to i16
  store i16 %2, i16* %c
  %d = alloca i32
  store i32 9, i32* %d
  %3 = load i32* %b
  %4 = load i16* %c
  %5 = zext i16 %4 to i32
  %6 = add i32 %3, %5
  %7 = load i32* %d
  %8 = add i32 %7, %6
  store i32 %8, i32* %d
  %9 = load i8* %a
  %10 = load i32* %b
  %11 = zext i8 %9 to i32
  %12 = urem i32 %11, %10
  ret i32 %12
}
