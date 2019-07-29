(* hairy  . . .*)

class Foo inherits Bazz is
     a : Razz <- case self of
		      n : Razz => (new Bar);
		      n : Foo => (new Razz);
		      n : Bar => n;
   	         esac;

     b : Int <- a.doh() + g.doh() + doh() + printh();

     doh() : Int is let i : Int <- h in begin h <- h + 2; i; end end end;

end;

class Bar inherits Razz is

     c : Int <- doh();

     d : Object <- printh();
end;


class Razz inherits Foo is

     e : Bar <- case self of
		  n : Razz => (new Bar);
		  n : Bar => n;
		esac;

     f : Int <- a@Bazz.doh() + g.doh() + e.doh() + doh() + printh();

end;

class Bazz inherits IO is

     h : Int <- 1;

     g : Foo  <- case self of
		     	n : Bazz => (new Foo);
		     	n : Razz => (new Bar);
			n : Foo  => (new Razz);
			n : Bar => n;
		  esac;

     i : Object <- printh();

     printh() : Int is begin out_int(h); 0; end end;

     doh() : Int is let i: Int <- h in begin h <- h + 1; i; end end end;
end;

(* scary . . . *)
class Main is
  a : Bazz <- new Bazz;
  b : Foo <- new Foo;
  c : Razz <- new Razz;
  d : Bar <- new Bar;

  main(): String is "do nothing" end;

end;





