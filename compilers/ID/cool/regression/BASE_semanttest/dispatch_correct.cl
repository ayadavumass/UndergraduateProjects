class C is
	a : Int;
	b : Bool;
	init(x : Int, y : Bool) : C is
           begin
		a <- x;
		b <- y;
		self;
           end
	end;
end;

Class Main is
	main():C is
	  (new C).init(1,true)
	end;
end;
