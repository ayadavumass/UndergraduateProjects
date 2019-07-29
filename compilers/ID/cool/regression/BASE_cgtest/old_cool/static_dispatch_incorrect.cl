class C is
    a :Int;

    init(x : Int) : SELF_TYPE is
	begin
	   a <- x;
	   self;
          end
        end;

end;

class Main inherits C is

    b : Bool;
 
    main(x : Int, y : Bool) : SELF_TYPE is
	begin
	   b <- y;
	   self@D.init(x);
	end
    end;
end;

class B is

    main(x : Int) : SELF_TYPE is
      self@C.init(x)
    end;
end;
