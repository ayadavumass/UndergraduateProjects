Class A is
	a : Int;
end;

Class Main inherits A is
	set_a(fa:Int):SELF_TYPE is
	  begin
	    a <- fa;
	    self;
	  end
	end;
end;
