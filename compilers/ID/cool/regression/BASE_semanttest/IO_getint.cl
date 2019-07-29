class Main inherits IO is
	main():SELF_TYPE is
	  begin
	    self.out_string("Enter an integer: ");
	    let i : Int <- self.in_int() in
	      begin
		self.out_string("Read: ");
		self.out_int(i);
		self.out_string("\n");
	      end
	    end;
	  end
	 end;
end;
