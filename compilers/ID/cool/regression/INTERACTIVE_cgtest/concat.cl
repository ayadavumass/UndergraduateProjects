Class Main inherits IO is
	sum : String <- "";
	s : String <- "_";
	main() : Object is
	 begin
	 while not (s = "") loop
	  begin
	    out_string("Type in a line: ");
	    s <- in_string();
	    sum <- sum.concat(s);
	  end
	 pool;
	 out_string("The sum is:\n").out_string(sum).out_string("\n");
	 end 
	end;
end;
