
void func()
{
	int a;
	@int p = a$;

	p++;
	p--;
}

void foo()
{
	vec<3,int> a;
	auto p = a$;

	p++;
	p--;
}

========

define void @func() {
  %a = alloca i32
  %p = alloca i32*
  store i32* %a, i32** %p
  %1 = load i32** %p
  %2 = getelementptr i32* %1, i32 1
  store i32* %2, i32** %p
  %3 = load i32** %p
  %4 = getelementptr i32* %3, i32 -1
  store i32* %4, i32** %p
  ret void
}

define void @foo() {
  %a = alloca <3 x i32>
  %p = alloca <3 x i32>*
  store <3 x i32>* %a, <3 x i32>** %p
  %1 = load <3 x i32>** %p
  %2 = getelementptr <3 x i32>* %1, i32 1
  store <3 x i32>* %2, <3 x i32>** %p
  %3 = load <3 x i32>** %p
  %4 = getelementptr <3 x i32>* %3, i32 -1
  store <3 x i32>* %4, <3 x i32>** %p
  ret void
}
