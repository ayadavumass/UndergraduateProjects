Class List is 
	isNil() : Object is abort() end;

	cons(hd : Int) : Cons is
	  let new_cell : Cons <- new Cons in
		new_cell.init(hd,self)
	  end
	end;

	car() : Int is abort() end;

	cdr() : List is abort() end;
end;

Class Cons inherits List is
	car : Int;
	cdr : List;

	isNil() : Bool is false end;

	init(hd : Int, tl : List) : Cons is
	  begin
	    car <- hd;
	    cdr <- tl;
	    self;
	  end
	end;
	  
	car() : Int is car end;

	cdr() : List is cdr end;
end;

Class Nil inherits List is
	isNil() : Bool is true end;
end;





