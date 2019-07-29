Class Object is 
	copy() : SELF_TYPE is self end;
	isProto() : Bool is false end;
	abort() : Bool is if false then false else true fi end;
end;

Class Int is 
	val : Prim__Int;
end;

Class Bool is 
	val : Prim__Int;
end;

Class String is
	val : Prim__String;
end;

------------------------------------------------------------------------------

Class List is 
	isNil() : Bool is abort() end;

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

------------------------------------------------------------------------------

