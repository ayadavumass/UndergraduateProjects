class Main is 
	main() : Int is 1 end;
end;

class C is
	a : Int;
	b : Bool;

	init(x: Int, y: Bool) : C is
	   begin
		a <- x;
		b <- y;
		self;
           end
	end;
end;
