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
	 begin
	  (new C).init(1,1);
	  (new C).init(1,true,3);
	  (new C).iinit(1,true);
	  (new C);
	 end
	end;
end;
