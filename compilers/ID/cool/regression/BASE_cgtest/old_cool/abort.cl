class AbortClass inherits Main is end;

class Main is
   a : Int;
   b : Int <- 1;
   main() : Object is
	 if isvoid a
	 then abort()
	 else (new AbortClass).abort()
	 fi
   end;
end;
