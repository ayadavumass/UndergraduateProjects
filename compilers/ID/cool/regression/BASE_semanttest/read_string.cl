Class Main inherits IO is
  main() : Object is
	while true loop
	  begin
	    out_string("Enter a line:\n");
	    let s : String <- in_string() in
	      begin
	        out_string("Entered:\n");
		out_string(s);
	      end
	    end;
	  end
	pool
  end;
end;
