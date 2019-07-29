class B_Wrong inherits Main is end;
class AB_Wrong inherits Main is end;
class A_Wrong inherits Main is end;
class Correct inherits Main is end;

class Main is
   a : IO;
   b : Int <- 1;
   main() : Object is
	 if isvoid b
	 then begin
		if ISVOID a
		then (new B_Wrong).abort()
		else (new AB_Wrong).abort()
		fi;
	      end
	else begin
		if iSvOiD a
		then (new Correct).abort()
		else (new A_Wrong).abort()
	     	fi;
	     end
	 fi
   end;
end;
