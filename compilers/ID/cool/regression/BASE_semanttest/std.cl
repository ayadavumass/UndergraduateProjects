class C is
    a :Int;

    init(x : Int) : SELF_TYPE is
	begin
	   a <- x;
	   (new C).abort();
	   self;
          end
        end;

end;

class Main inherits C is

    b : Bool;
 
    init(x : Int) : SELF_TYPE is
	begin
	  b <- false;
	  self@C.init(x);
	end
    end;

    main(x : Int, y : Bool) : Main is
	begin
	   b <- y;
	   init(x);
	end
    end;
end;
