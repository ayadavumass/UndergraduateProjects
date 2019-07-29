-- Lets can be nested

class Main inherits IO
{
  print(x:Int) : Object
  { {
     out_int(x);
     out_string("\n");
    }
  };

  main() : Object
  {
    let foo : Int <- 5 in 
       let foo: Int <- ~1 in
         let foo : Int <- 6 in 
	   print(foo+1)           
  };
};
