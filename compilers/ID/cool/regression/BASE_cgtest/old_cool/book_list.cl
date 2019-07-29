-- example of static and dynamic type differing for a dispatch

Class Book inherits IO is
    title : String;
    author : String;

    initBook(title_p : String, author_p : String) : Book is
        begin
            title <- title_p;
            author <- author_p;
            self;
        end
    end;

    print() : Book is
        begin
            out_string("title:      ").out_string(title).out_string("\n");
            out_string("author:     ").out_string(author).out_string("\n");
            self;
        end
    end;
end;

Class Article inherits Book is
    per_title : String;

    initArticle(title_p : String, author_p : String,
		per_title_p : String) : Article is
        begin
            initBook(title_p, author_p);
            per_title <- per_title_p;
            self;
        end
    end;

    print() : Book is
        begin
	    self@Book.print();
            out_string("periodical:  ").out_string(per_title).out_string("\n");
            self;
        end
    end;
end;

Class BookList inherits IO is 
    (* Since abort "returns" type Object, we have to add
       an expression of type Bool here to satisfy the typechecker.
       This code is unreachable, since abort() halts the program.
    *)
    isNil() : Bool is begin abort(); true; end end;
    
    cons(hd : Book) : Cons is
        let new_cell : Cons <- new Cons in
            new_cell.init(hd,self)
        end
    end;

    (* Since abort "returns" type Object, we have to add
       an expression of type Book here to satisfy the typechecker.
       This code is unreachable, since abort() halts the program.
    *)
    car() : Book is begin abort(); new Book; end end;
    
    (* Since abort "returns" type Object, we have to add
       an expression of type BookList here to satisfy the typechecker.
       This code is unreachable, since abort() halts the program.
    *)
    cdr() : BookList is begin abort(); new BookList; end end;
    
    print_list() : Object is abort() end;
end;

Class Cons inherits BookList is
    xcar : Book;  -- We keep the car and cdr in attributes.
    xcdr : BookList; -- Because methods and features must have different names,
    -- we use xcar and xcdr for the attributes and reserve
    -- car and cdr for the features.
    
    isNil() : Bool is false end;
    
    init(hd : Book, tl : BookList) : Cons is
        begin
            xcar <- hd;
            xcdr <- tl;
            self;
        end
    end;

    car() : Book is xcar end;

    cdr() : BookList is xcdr end;
    
    print_list() : Object is
        begin
            case xcar.print() of
                dummy : Book => out_string("- dynamic type was Book -\n");
                dummy : Article => out_string("- dynamic type was Article -\n");
            esac;
            xcdr.print_list();
        end
    end;
end;

Class Nil inherits BookList is
    isNil() : Bool is true end;

    print_list() : Object is true end;
end;


Class Main is

    books : BookList;

    main() : Object is
        let a_book : Book <-
            (new Book).initBook("Compilers, Principles, Techniques, and Tools",
                                "Aho, Sethi, and Ullman")
        in
            let an_article : Article <-
                (new Article).initArticle("The Top 100 CD_ROMs",
                                          "Ulanoff",
                                          "PC Magazine")
            in
                begin
                    books <- (new Nil).cons(a_book).cons(an_article);
                    books.print_list();
                end
            end  -- end let an_article
        end  -- end let a_book
    end;
end;
