class Main inherits IO is
	main():SELF_TYPE is
	  begin
	    out_string("Hello world\n");
	    out_string("This_\b was a typo\n");
	    out_string("And a ff\f\n");
	    out_string("And one with a \"doublequote\n");
	  end
	end;
end;
