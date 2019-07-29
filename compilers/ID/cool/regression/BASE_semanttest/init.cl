Class A is

   a : Int <- 5;

   foo() : Int is
      a * 3
   end;
end;

Class B inherits A is

   b : Int <- self@A.foo();

end;

Class C inherits B is

   c : String <- "Init string";

   bar(io : IO) : IO is
     begin
	io.out_string("a : ").out_int(a).out_string("\n");
	io.out_string("b : ").out_int(b).out_string("\n");
	io.out_string("c : ").out_string(c).out_string("\n");
	io;
     end
   end;
end;

Class Main inherits IO is

   cObj : C <- new C;

   main() : Object is cObj.bar(self) end;
end;


