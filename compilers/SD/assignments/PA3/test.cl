class B {
	s : String <- "Hello";
	g (y:String) : Int {
	x+1	
	};
	f (x:Int) : Int {
	
	if position = 0 then
            cell(num_cells() - 1)
        else
            cell(position - 1)
        fi
	
	};

	h (x:Int) : Int {
	
	while countdown < 0 loop
                    {
                        cells.evolve();
                        cells.print();
                        countdown <- countdown - 1;
		   }
                    
                pool
	
	};
};


class A inherits B {
	a : Int;
	b : String <- "Hello";
	f(x:Int) : Int {
	x+a
	};
};


