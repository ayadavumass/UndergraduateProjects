--
-- This is the pseudo definition of the root Class Void
--
class Void is
	name : String;

	className() : String is name end;

	copy() : SELF_TYPE is
	   prim	  (* some primitive code *)
	end;

end;
