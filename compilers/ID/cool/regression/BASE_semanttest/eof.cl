Class Main inherits IO is
   main():Object is
     let s : String <- in_string() in
	out_string(s).out_int(s.length().copy())
     end
   end;
end;
