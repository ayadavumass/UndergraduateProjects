class Main inherits IO is
  
   b : Main;

   main() : Object is 
		let a: Main <- new Main in
			begin 
			if a = b then 
				out_string("Equal\n") 
			   else out_string("Not Equal\n") fi; 
			if b = a then 
				out_string("Equal\n") 
			   else out_string("Not Equal\n") fi; 
			if b = b then 
				out_string("Equal\n") 
			   else out_string("Not Equal\n") fi; 
		      end
	     end
   end;
end;
