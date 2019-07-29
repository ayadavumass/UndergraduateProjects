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

    i : Main;
    x : Int <- 1;
    y : Bool <- false;
 
    main() : Object is
	begin
	   b <- y;
	   self@C.init(x);
           i@Main.main();
	end
    end;
end;
