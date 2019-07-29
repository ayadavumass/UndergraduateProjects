class Main inherits IO is
    a : Bool <- isvoid (1 + 2);
    main() : Object is
	let r : Main in
	    if isvoid r then abort() else out_string("OK") fi
	end
    end;
end;
