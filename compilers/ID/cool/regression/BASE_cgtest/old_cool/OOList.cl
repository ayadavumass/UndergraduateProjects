Class List is 
	isNil() : Bool is begin abort(); false; end end;

	cons(hd : Int) : Cons is
	  let new_cell : Cons <- new Cons in
		new_cell.init(hd,self)
	  end
	end;

	car() : Int is begin abort(); 1; end end;

	cdr() : List is begin abort(); new List; end end;
end;

Class Cons inherits List is
	xcar : Int;
	xcdr : List;

	isNil() : Bool is false end;

	init(hd : Int, tl : List) : Cons is
	  begin
	    xcar <- hd;
	    xcdr <- tl;
	    self;
	  end
	end;
	  
	car() : Int is xcar end;

	cdr() : List is xcdr end;
end;

Class Nil inherits List is
	isNil() : Bool is true end;
end;

Class Main is
	main() : List is new List end;
end;




