Class Main inherits IO is
	a : Int <- 5;
	main() : SELF_TYPE is
	  let r : Int <- fib(a) in
	    begin
	      out_string("Fib of ");out_int(a);out_string(" is: ");
	      out_int(r); out_string("\n");
	    end
          end
	end;

	fib(x : Int) : Int is
	  if x<2 then 1
	  else
		fib(x-1)+fib(x-2)
	  fi
	end;
end;
