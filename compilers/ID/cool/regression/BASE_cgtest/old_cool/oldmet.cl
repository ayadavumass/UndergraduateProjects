class C1 is
    a : Int;
    init():SELF_TYPE is
	a<-0
    end;
end;

class C2 inherits C1 is
    b : Int;

    init():SELF_TYPE is
	begin
	    self@C1.init();
	    b<-a*2;
	end
    end;
end;

class C3 inherits C2 is
    c : Int;

    init():SELF_TYPE is
	begin
	    self@C3.init();
	    c<-a*a;
	end
    end;
end;

