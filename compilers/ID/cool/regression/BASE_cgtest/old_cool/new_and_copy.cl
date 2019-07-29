(* This is a buggy Cool program.
 * Well, it exhibits what I think is a bug in Cool.
 * It seems that, if you re-define copy in a class A, calls to "new A"
 * will have behavior affected by the definition of copy.
 *
 * Name: Michael Heyeck
 * SID:  11528896
 *)

Class Main is
	
	a : A;
	b : A;

	main() : Object is
		begin
			a <- new A;
			b <- new A;
			a.showint();     (* Prints 0 - init value *)
			b.showint();     (* Prints 0 - init value *)
			a.assignint(5);  (* Assigns 5 to a *)
			a.showint();     (* Prints 5 - a's value *)
			b.showint();     (* Prints 5 - b's value *)
		end
	end;
end;

(* Note that assignment to a changed b's value - they point to the same
 * object.  They should point to different objects - the behavior is caused
 * by the redefinition of copy in A.  I believe this is a bug. 
 *)

Class A inherits IO is

	x : Int;

	copy() : SELF_TYPE is
		self
	end;

	showint() : Object is
		out_int(x)
	end;
	
	assignint(y : Int) : Int is
		x <- y
	end;
end;
			
