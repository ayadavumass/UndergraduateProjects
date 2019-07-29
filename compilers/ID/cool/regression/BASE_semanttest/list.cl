(*
 *  Example 1 uses the syntax, where only local or inherited
 *  attributes of self can be directly accessed.
 *  Therefore the need for the init method.
 *)
class Int_List1 is
   car : Integer;
   cdr : SELF_TYPE;
   
   -- void is used for nil

   init(hd : Integer, tl : SELF_TYPE) : SELF_TYPE is
	begin
	  car <- hd;
	  cdr<-tl;
	  self;
	end
   end;

   cons(new_elem : Integer) : SELF_TYPE is
	let a : SELF_TYPE <- new SELF_TYPE in
	  a.init(new_elem, self)
	end
   end;

   hd() : Integer is car end;
				-- runtime error if void
   tl() : SELF_TYPE is cdr end;
				-- runtime error if void
   length() : Integer is
	if self = void 
	  then 0
	  else 1 + self.tl().length()
	fi
   end;

   (* a length function written using a loop *)
   iter_length() : Integer is
        let list : Int_List1 <- self in 
          let count : Int <- 0 in
            begin
	      while not (list = void) 
               loop
		 begin
		   count <- count + 1;
		   list <- list.cdr();
                 end
               pool;
              count;
            end
          end
        end
   end;

end;

