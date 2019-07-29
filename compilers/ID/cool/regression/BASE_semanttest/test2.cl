class MAIN is
  a : Integer;

  f():Void is
	let a : Int in begin
	  g();
	  f();
	end end
  end;
  g():Void is begin a; b; end end;
end;
