class String is
   c : Char;
   r : String;
   
   -- es() returns an empty string, which is represented as a string
   -- object with second component void.
   es() : String is void end;
   
   cs(x: Char, y: String) : String is
       let a : String <- new String in
         begin
           a.c <- x; 
           a.r <- y;
           a;
         end
       end
   end;

   prints(s: String) : Void is
       while not (s.r = void) 
       loop
	begin
	  s.c.printc();
	  s <- s.r;
	end
       pool
    end;

end;
