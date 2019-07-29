Class Main inherits IO is
	main() : Object is
	  let s : String <- "spim".concat("mips") in
	    begin
	      out_string(s.copy().concat("\n"));
	      out_string("Length: ").out_int(s.length()).out_string("\n");
	    end
	  end
	end;
end;
