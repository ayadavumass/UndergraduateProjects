class A inherits IO is
    foo() : Int is 1 end;
end;

class Main inherits A is
    foo() : Int is 2 end;

    main() : Object is
	out_int(self@A.foo())
    end;
end;
