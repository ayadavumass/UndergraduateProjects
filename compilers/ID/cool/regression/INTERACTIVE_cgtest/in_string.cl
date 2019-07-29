Class Main inherits IO is
	s : String <- "_";
	main() : Object is
	 while not (s = "") loop
	  begin
	    out_string("Type in a line\n");
	    s <- in_string();
	    out_string("Entered: ").out_string(s).out_string("\n");
	    out_string("Length: ").out_int(s.length()).out_string("\n");
	  end
	 pool
	end;
end;
