Class C1 inherits C2 is
	x : C1;
end;

Class C2 inherits C1 is
	y : C2;
end;

Class Main is
	main() : Int is (new C1).f() end;
end;
