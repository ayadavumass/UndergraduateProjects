class Main inherits IO is
    main() : SELF_TYPE is
	let c : Complex <- (new Complex).init(1, 1) in
	    begin
	        -- trivially equal (see CoolAid)
	        if c.reflect_X() = c.reflect_0()
	        then out_string("=)\n")
	        else out_string("=(\n")
	        fi;
		-- equal
	        if c.reflect_X().reflect_Y().equal(c.reflect_0())
	        then out_string("=)\n")
	        else out_string("=(\n")
	        fi;
	    end
	end
    end;
end;

class Complex inherits IO is
    x : Int;
    y : Int;

    init(a : Int, b : Int) : Complex is
	begin
	    x = a;
	    y = b;
	    self;
	end
    end;

    print() : Object is
	if y = 0
	then out_int(x)
	else out_int(x).out_string("+").out_int(y).out_string("I")
	fi
    end;

    reflect_0() : Complex is
	begin
	    x = ~x;
	    y = ~y;
	    self;
	end
    end;

    reflect_X() : Complex is
	begin
	    y = ~y;
	    self;
	end
    end;

    reflect_Y() : Complex is
	begin
	    x = ~x;
	    self;
	end
    end;

    equal(d : Complex) : Bool is
	if x = d.x_value()
	then
	    if y = d.y_value()
	    then true
	    else false
	    fi
	else false
	fi
    end;

    x_value() : Int is
	x
    end;

    y_value() : Int is
	y
    end;
end;
